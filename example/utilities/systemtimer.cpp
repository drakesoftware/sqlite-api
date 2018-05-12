#include <stddef.h>
#include <cstdlib>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "utilities/systemtimer.h"

CSystemTimer CSystemTimer::g_obGlobalTimer;


CSystemTimer::_timer_event::_timer_event():nCountDown(TIMER_NOT_SET),nInitialTime(0),pCallback(NULL),userParam(0),bOneShot(false){

};


CSystemTimer::CSystemTimer():m_nTimerPeriod(TIMER_NOT_SET)
{
	
	m_nTimerId=(void*)TIMER_NOT_SET;

}

CSystemTimer::~CSystemTimer()
{
	KillOSTimer();
}

CSystemTimer& CSystemTimer::GetInstance()
{

	return(g_obGlobalTimer);

}
unsigned int CSystemTimer::GetNextTimeTick()
{
	unsigned int nRetVal(TIMER_NOT_SET);
	

	m_obLock.lock();
	for(TIMER_MAP_T::iterator Iterator(m_TimerMap.begin());m_TimerMap.end()!=Iterator;Iterator++)
	{
		_timer_event *pTimer=Iterator->second();

		if(nRetVal==TIMER_NOT_SET || pTimer->nCountDown < nRetVal)
			nRetVal=pTimer->nCountDown;
		
	}
	m_obLock.unlock();

	return(nRetVal);
}


void CSystemTimer::StartOSTimer(unsigned int nTime)
{
	sigevent 	stSigEvent;
	itimerspec 	stTimerSpec;

	////__android_log_print(ANDROID_LOG_DEBUG, "WifiTalke","+StartOSTimer %d",nTime);
	m_obLock.lock();
	if(m_nTimerId==TIMER_NOT_SET)
	{
		memset(&stSigEvent,0,sizeof(stSigEvent));
		stSigEvent.sigev_notify = SIGEV_THREAD;
		stSigEvent.sigev_value.sival_ptr = this;
		stSigEvent.sigev_notify_function= CSystemTimer::StaticTimerCallbackTick;
		stSigEvent.sigev_notify_attributes = NULL;

		if(timer_create(CLOCK_MONOTONIC, &stSigEvent, &m_nTimerId)==-1)
		{
			m_nTimerId=(void*)TIMER_NOT_SET;
		}
	}
	stTimerSpec.it_value.tv_sec = nTime/1000;
	stTimerSpec.it_value.tv_nsec = (nTime%1000)*1000000;
	timer_settime(m_nTimerId, 0, &stTimerSpec, 0);
	clock_gettime(CLOCK_MONOTONIC, &m_nStart);
	m_nTimerPeriod=nTime;
	m_obLock.unlock();

}

void CSystemTimer::KillOSTimer()
{
	m_obLock.lock();
	if(m_nTimerId!=TIMER_NOT_SET)
	{
		timer_delete(m_nTimerId);
		m_nTimerId=TIMER_NOT_SET;
	}
	m_nTimerPeriod=TIMER_NOT_SET;
	m_obLock.unlock();
}



size_t	CSystemTimer::ScheduleTimer(unsigned int nTime,TimerEventCallback pCallback,void* userParam,bool oneShot)
{
	_timer_event	*pNewTimer(NULL);
	int				nRetVal(0);
	unsigned int	nNextTime(TIMER_NOT_SET);


	if(nTime!=0 && nTime!=TIMER_NOT_SET)
	{
		pNewTimer=new _timer_event();
		pNewTimer->bOneShot=oneShot;
		pNewTimer->nCountDown=pNewTimer->nInitialTime=nTime;
		pNewTimer->pCallback=pCallback;
		pNewTimer->userParam=userParam;

		m_obLock.lock();


		nRetVal=(size_t)pNewTimer;

		if(m_nTimerPeriod!=TIMER_NOT_SET)
		{
			unsigned int nTimerPeriod=nTime;
			struct timespec endTime;


			for(TIMER_MAP_T::iterator Iterator(m_TimerMap.begin()); Iterator!=m_TimerMap.end();Iterator++)
			{
				_timer_event *pTimer=Iterator->second();
				if(pTimer)
				{
					nTimerPeriod=std::min(pTimer->nCountDown,nTimerPeriod);
				}
			}

			clock_gettime(CLOCK_MONOTONIC, &endTime);
			struct timespec deltaTime;
			timespec_subtract(&endTime,&m_nStart,&deltaTime);
			int nElapsed=((deltaTime.tv_sec*1000)+(deltaTime.tv_nsec/ 1000000));
			if(nTimerPeriod < nTime)
			{
				pNewTimer->nCountDown+=nElapsed;
			}
			else
			{

				KillOSTimer();
				OnTimerTick((unsigned int)nElapsed);
				nNextTime=nTime;
			}
			m_TimerMap.insert(std::make_pair((size_t)pNewTimer,pNewTimer));

		}
		else
		{
			m_TimerMap.insert(std::make_pair((size_t)pNewTimer,pNewTimer));
			nNextTime=nTime;

		}

		m_obLock.unlock();


		if(nNextTime!=TIMER_NOT_SET)
			StartOSTimer(nNextTime);

	}

	if(pNewTimer)
		pNewTimer->Release();
	return(nRetVal);

}

void CSystemTimer::DestoryTimer(TIMER_MAP_T::iterator timer)
{
	m_obLock.lock();

	if(timer!=m_TimerMap.end())
	{
		m_TimerMap.erase(timer);
		if(m_TimerMap.size()==0)
		{
			KillOSTimer();
		}

	}
	m_obLock.unlock();

}

void CSystemTimer::DestoryTimer(size_t nTimerId)
{
	TIMER_MAP_T::iterator Iterator;

	m_obLock.lock();
	Iterator=m_TimerMap.find(nTimerId);
	if(Iterator!=m_TimerMap.end())
	{
		DestoryTimer(Iterator);
	
	}
	m_obLock.unlock();
}
		
void CSystemTimer::OnTimerTick(unsigned int nTimerPeriod)
{
	struct timespec 				deltaTime,endTime;
	CSystemTimer::TIMER_QUEUE_T		firedQueue;

	clock_gettime(CLOCK_MONOTONIC, &endTime);
	m_obLock.lock();
	timespec_subtract(&endTime,&m_nStart,&deltaTime);
	int nElapsed=((deltaTime.tv_sec*1000)+(deltaTime.tv_nsec/ 1000000));
	for(TIMER_MAP_T::iterator Iterator(m_TimerMap.begin());m_TimerMap.end()!=Iterator;)
	{

		_timer_event *pTimer=Iterator->second();
		try
		{
			if(nTimerPeriod <= pTimer->nCountDown)
				pTimer->nCountDown-=nTimerPeriod;
			else
				pTimer->nCountDown=0;

			if(pTimer->nCountDown==0)
			{
				firedQueue.push_back(pTimer);

				if(pTimer->bOneShot)
				{
					m_TimerMap.erase(Iterator);
					Iterator++;

				}
				else
				{
					pTimer->nCountDown=pTimer->nInitialTime;
					Iterator++;
				}
			}
			else
			{
				Iterator++;
			}
		}
		catch(...)
		{
			Iterator++;
		}
	}
	m_obLock.unlock();

	while(!firedQueue.empty())
	{
		_timer_event *p=firedQueue.front()();
		if(p)
		{

			try
			{
				if(p->pCallback)
					p->pCallback(p->userParam,(size_t)p);
			}
			catch(...)
			{

			}

		}
		firedQueue.pop_front();
	}



}
void CSystemTimer::TimerCallbackTick()
{
	unsigned int nTimerPeriod;
	unsigned int nNextTime(TIMER_NOT_SET);

	m_obLock.lock();
	nTimerPeriod=m_nTimerPeriod;
	KillOSTimer();
	m_obLock.unlock();
	OnTimerTick(nTimerPeriod);
	m_obLock.lock();

	if((nNextTime=GetNextTimeTick())!=TIMER_NOT_SET)
		StartOSTimer(nNextTime);
	m_obLock.unlock();


}

void CSystemTimer::StaticTimerCallbackTick(sigval si)
{
	CSystemTimer *pInstance=reinterpret_cast<CSystemTimer*>(si.sival_ptr);

	if(pInstance)
	{
		pInstance->TimerCallbackTick();
	}
	

}


int CSystemTimer::timespec_subtract (struct timespec *result,struct timespec *x,struct timespec *y)
{
  /* Perform the carry for the later subtraction by updating Y. */
  if (x->tv_nsec < y->tv_nsec) {
    int num_sec = (y->tv_nsec - x->tv_nsec) / 1000000000 + 1;
    y->tv_nsec -= 1000000000 * num_sec;
    y->tv_sec += num_sec;
  }
  if (x->tv_nsec - y->tv_nsec > 1000000000) {
    int num_sec = (x->tv_nsec - y->tv_nsec) / 1000000000;
    y->tv_nsec += 1000000000 * num_sec;
    y->tv_sec -= num_sec;
  }

  /* Compute the time remaining to wait.
     `tv_nsec' is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_nsec = x->tv_nsec - y->tv_nsec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;
}

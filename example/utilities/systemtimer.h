#ifndef GLOBALTIMER_H_
#define GLOBALTIMER_H_
#include <map>
#include "Mutex.h"
#include <time.h>
#include <deque>
#include <signal.h>
#include "refcountedbase.h"

#define TIMER_NOT_SET (0)


class CSystemTimer
{
	public:

		typedef void (*TimerEventCallback)(void* param,size_t nTimerId);
	protected:
		class  _timer_event:public ReferenceCountedBase
		{
			public:
				unsigned int    	nCountDown;
				unsigned int    	nInitialTime;
				TimerEventCallback  pCallback;
				void           		*userParam;
				bool            	bOneShot;
				_timer_event();



        };
		typedef SmartPointer<_timer_event>TimerEvent_SMART_PTR;
		typedef std::map<size_t,TimerEvent_SMART_PTR>TIMER_MAP_T;
		typedef std::deque<TimerEvent_SMART_PTR>TIMER_QUEUE_T;
	
	protected:
		RecursiveMutex				m_obLock;
		TIMER_MAP_T					m_TimerMap;
		unsigned int				m_nTimerPeriod;
		timer_t						m_nTimerId;
		struct timespec 			m_nStart;
	protected:
		  static CSystemTimer 		g_obGlobalTimer;
	protected:
									~CSystemTimer();
									CSystemTimer();
	public:
		static	CSystemTimer&		GetInstance();
		size_t						ScheduleTimer(unsigned int nTime,TimerEventCallback pCallback,void* userParam,bool oneShot);
		void						DestoryTimer(size_t nTimerId);

	protected:
		void						DestoryTimer(TIMER_MAP_T::iterator timer);
		void						StartOSTimer(unsigned int nTime);
		void						KillOSTimer();
		void						OnTimerTick(unsigned int nTimerPeriod);
		unsigned int				GetNextTimeTick();
		void						TimerCallbackTick();
		static void					StaticTimerCallbackTick(sigval si);
		static int 					timespec_subtract (struct timespec *result,struct timespec *x,struct timespec *y);
};
#endif

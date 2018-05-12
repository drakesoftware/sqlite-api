#include "Thread.h"
#include <stdio.h>
#include <errno.h>

//Thread::~Thread() {
//   //printf("Thread destructor\n");
//   //   stop();
//}

pthread_t * Thread::getThread() {
   return &m_thread;
}


//Call overriddent ThreadFunction
void* Thread::entryPoint(void *arg ){
   //printf("In Thread\n");
	Thread *p=(Thread*)arg;

	if(p){
		if(p->mFunc)
			p->mFunc(p->mArg);
		//printf("Exiting Thread %s\n",mName);
		fflush(stdout);
		p->stop();
	}
   return NULL;
}

int Thread::start(THREAD_FUNC pFunc,void * pArg) {
   //printf("Starting thread\n");
	mFunc=pFunc;
	mArg=pArg;
   int rval= pthread_create(&m_thread, NULL, (void * (*) (void*)) &Thread::entryPoint, this);
   
   if (rval)
   {
       printf("Unable to start Thread\n");
   }
   switch (rval)
   {
       case 0:
           break;
       case EAGAIN:
           printf("EAGAIN: Insufficient resources to start Thread\n");
           break;

       case EINVAL:
            printf("EINVAL: Invalid attribute settings\n");
            break;

        case EPERM:
            printf("EPERM: Permissions to set scheduling policy or parameters\n");
            break;
    }
   return rval;
}

void Thread::stop() {
      pthread_exit(NULL);
}



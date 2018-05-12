
#ifndef __Thread_h__
#define __Thread_h__

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <signal.h>

const int THREAD_NAME_LENGTH_MAX=20;
class Thread 
{
public:
	typedef void(*THREAD_FUNC)(void*);

protected:
	THREAD_FUNC	mFunc;
	void*		mArg;
    public:
        Thread(const char *name):mFunc(NULL),mArg(0)
        {
        if (name)
            {
            strncpy(mName,name,THREAD_NAME_LENGTH_MAX);
            }
        else
            {
            mName[0]=(char)NULL;
            }
       }
//      ~Thread();
      int start(THREAD_FUNC pFunc,void * pArg);

      //Called by involing thread to wait until thread ends
      void join()
      {
         void* retval; 
         int jval=pthread_join(m_thread,&retval);
         printf("Thread %s joined, returned %d\n",mName,jval);
         if (retval==PTHREAD_CANCELED)
         {
             printf("Join returned PTHREAD_CANCELED\n");
         }
      }
      pthread_t * getThread();
      int getThreadId();


     static void* entryPoint( void* );




      void cancel(void)
      {
          int rval=pthread_cancel(m_thread);
          printf("Pthread cancel returned: %d\n",rval);
      }

      char *getThreadName()
      {
          return mName;
      }
   private:
      char mName[THREAD_NAME_LENGTH_MAX];
      void stop();
      pthread_t m_thread;
};

#endif

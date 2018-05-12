
#ifndef __Semaphore_h__
#define __Semaphore_h__

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>


class Semaphore {

   public:

   sem_t m_Semaphore;

   Semaphore(unsigned int val=0,  int shared=0)
      {
      sem_init(&m_Semaphore,shared,val);
      }

   ~Semaphore()
   {
     sem_destroy(&m_Semaphore);
   }

   int wait()
   {
      return sem_wait(&m_Semaphore); 
   }

   int trywait()
   {
      return sem_trywait(&m_Semaphore); 
   }

   int timedwait(time_t seconds, long nsec )
   {
      struct timespec t={seconds,nsec};

      return sem_timedwait(&m_Semaphore, &t); 
   }




   int post()
   {
      return sem_post(&m_Semaphore); 
   }

};

#endif

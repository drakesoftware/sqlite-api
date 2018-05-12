
#ifndef __Mutex_h__
#define __Mutex_h__

#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

class Mutex {
   public:

   pthread_mutex_t m_Mutex;


   Mutex(void)
   {
      memset(&m_Mutex,0,sizeof(m_Mutex));
      pthread_mutex_init(&m_Mutex,NULL); 
//      printf("mutex init \n");
   }
   Mutex(int p)
   {
      memset(&m_Mutex,0,sizeof(m_Mutex));
      pthread_mutex_init(&m_Mutex,NULL); 
   }

   virtual  ~Mutex()
   {
      pthread_mutex_destroy(&m_Mutex);
   }

   //Provide access to raw mutex
   virtual pthread_mutex_t* getMutex()
   {
      return &m_Mutex;
   }
   virtual int lock()
   {
      return pthread_mutex_lock(&m_Mutex); 
   }

   virtual  int trylock()
   {
      return pthread_mutex_trylock(&m_Mutex); 
   }

   virtual  int unlock()
   {
      return pthread_mutex_unlock(&m_Mutex); 
   }

   protected:
   explicit Mutex(bool bInit)
      {
          memset(&m_Mutex,0,sizeof(m_Mutex));
          if(bInit){
              pthread_mutex_init(&m_Mutex,NULL);
              printf("mutex init expl\n");
          }
      }

};

class RecursiveMutex: public Mutex{

    public :
    RecursiveMutex():Mutex(false){

        pthread_mutexattr_t Attr;
        pthread_mutexattr_init(&Attr);
        pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&m_Mutex, &Attr);
        printf("mutex init\n");
    }

};

#endif

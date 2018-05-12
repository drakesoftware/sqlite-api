// \file utils.h
/// \brief  UAT ADSB Latitude testing values



#ifndef __utils_h
#define __utils_h

#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include "json.h"
#include <string>



typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;




#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

#define SET_BIT(val, bitoffset) val |= (1 << bitoffset)
#define CLEAR_BIT(val, bitoffset) val &= ~(1 << bitoffset)
#define TOGGLE_BIT(val, bitoffset) val ^= (1 << bitoffset)
#define BIT_IS_SET(val, bitoffset) (val & (1 << bitoffset)) 





//Generate 'count' sequence of ones starting at LSB
//E.g. BITMASK_LSBS(3)= 0111 = 0x7
//Handle 32 bit values w/o overflowing
#define BITMASK_LSBS(count)  (((uint64_t)1 << count) - 1)


//Set 'count'  high order bits in byte
#define BITMASK_BYTE_MSBS(count) ~(BITMASK_LSBS(7-(count-1)))

///MSB of unsigned int set
#define MSB_UINT (UINT_MAX ^ INT_MAX)

//Sets 'count' # of bits shifted left 'shift'
//e.g. BITMASK_LSBS(0x3,4) =  01110000 (binary) = 0x7 << 'shift'
#define BITMASK_SHIFT(size, shift)  (BITMASK_LSBS(size) << shift)

//Create an alias for BITMASK_SHIFT
#define BITMASK(size,shift) BITMASK_SHIFT(size,shift)

//Compare to floats
bool compare(float a, float b,float precision=0.0);

//Given 'var', set 'size' # of bits at 'bitoffset', to
//val
#define SETBITFIELD(var,val,bitoffset,size) (var & ~BITMASK_SHIFT(size,bitoffset)\
                                         | (val&BITMASK_LSBS(size))<<bitoffset)




//Get the bit field of 'size' bits at 'bitoffset'  (bits)
//
#define GETBITFIELD(var,bitoffset,size) ((var & BITMASK_SHIFT(size,bitoffset)) >> bitoffset)

#define CHARPTR(array) &array[0]

///Return number of bits needed to represent the value
int bitsize(unsigned int val);



#define NUM_ELEMENTS(array)  (sizeof(array) / sizeof(array[0]))

//TODO: use only above implementation for determining # elements in array
#define ELEMENTCOUNT(array, type) (sizeof(array)/sizeof(type))


//Create string formatted with binary representation
//sep>0 separate string every 'sep' bits (normally 4 or 8)
char *binarystring(unsigned int value, char *str,int sep=0);
char *binarystring(unsigned long value, char *str,int sep=0);

int msleep(unsigned long milisec);


char* getTimeStamp(char* bufstring);

static const int STRINGSIZE=50;
//Simple to_string utility functions
inline std::string toFormattedString(double v,int w, int p)
{
    char str[STRINGSIZE];
    snprintf(str,STRINGSIZE, "%*.*f",w,p,v);
    return std::string(str);
}

inline std::string toFormattedString(int v,int w)
{
    char str[STRINGSIZE];
    snprintf(&str[0],STRINGSIZE,"%*d",v,w);
    return std::string(str);
}



//********************************************************
//Timer macros

#define PRINT_TIMEVAL(s,tv)\
    { \
    char timestr[12]; \
    sprintTime(&timestr[0],tv,12); \
    printf(" %s\n",timestr); \
    }


unsigned long UT_GetCurMsec(void);


inline bool inRange(int i, int low, int high)
{
    if(i < low || i > high)
    {
        return false;
    }
    else
    {
        return true;
    }
}

uint16_t toUnsigned(int16_t val);


//#define ENABLE_TIMERS   //<=====

#ifdef ENABLE_TIMERS
//Place outside of loop
//'id' is a unique id used to generate unique internal variable names,all related macro calls must use the same id
//'count' is the number of iterations that will be collected for statistics (min,max,avg)
//define the instance
#define LOOPTIMER(id,count) \
    unsigned int id ## StartLoopStopWatch=0; \
    unsigned int id ## LoopStopWatch=0; \
    unsigned int id ## LoopTotal; \
    unsigned int id ## LoopTime=0; \
    unsigned int id ## LoopMinTime=UINT_MAX; \
    unsigned int id ## LoopMaxTime=0; \
    int id ## LoopIndex=0;  \
    int id ## LoopCount=count;

//Within a loop, either LOOPTIMER_PRINTTIME, or LOOPTIMER_STAT can be used, not both
#define LOOPTIMER_PRINTTIME(id,maxtime) \
    id ## LoopStopWatch = UT_GetCurMsec(); \
    id ## LoopTime=id ## LoopStopWatch - id ## StartLoopStopWatch; \
    if (id ## LoopTime>maxtime){ \
        DEBUGP("==>Loop " #id " elapsed time= %u msec\n", id ## LoopTime); \
    } \
    id ## StartLoopStopWatch = UT_GetCurMsec() ; 

//gathers average, max, and mins stats for loop defined with LOOPTIMER_CREATE and
//initialized with LOOPTIMER, and prints every 'count' iterations
//if 'maxtime' is non-zero, a print will occur whenever the loop elapased time exceeds the value
#define LOOPTIMER_STAT(id,maxtime) \
	    id ## LoopStopWatch = UT_GetCurMsec(); \
        id ## LoopTime = id ## LoopStopWatch - id ## StartLoopStopWatch ; \
        id ## LoopTotal+=id ## LoopTime;\
        if (id ## LoopTime>id ## LoopMaxTime)  id ## LoopMaxTime=id ## LoopTime; \
        if (id ## LoopTime<id ## LoopMinTime)  id ## LoopMinTime=id ## LoopTime; \
        id ## LoopIndex++;\
        if (id ## LoopIndex==id ## LoopCount){ \
            DEBUGP("==>Loop"#id " average %u Max: %u  Min: %u\n", id ## LoopTotal/id ## LoopCount,id ## LoopMaxTime,id ## LoopMinTime); \
            id ## LoopIndex=0; \
            id ## LoopTotal=0; \
            id ## LoopMaxTime=0; \
            id ## LoopMinTime=UINT_MAX; \
        } \
        id ## StartLoopStopWatch=UT_GetCurMsec();\
        if (maxtime > 0 && (id ## LoopTime>maxtime)){ \
             DEBUGP("==>Loop " #id " elapsed time= %u msec\n", id ## LoopTime); \
        } 


#define TIMER_ELAPSED(id) \
    unsigned int id ## ExecStopWatchStart=0;\
    unsigned int id ## ExecStopWatchLast=0; 


//Allow access across  modules
#define TIMER_ELAPSED_EXTERN(id) \
    extern unsigned int id ## ExecStopWatchStart; \
    extern unsigned int id ## ExecStopWatchLast; 

#define TIMER_ELAPSED_START(id){ \
        id ## ExecStopWatchStart= UT_GetCurMsec();\
        id ## ExecStopWatchLast=id ## ExecStopWatchStart;\
        }

//Print elapsed time and reset 
//point is added to the print to identify the specific mark
#define TIMER_ELAPSED_MARK(id,point)  {\
        unsigned int marktime= UT_GetCurMsec(); \
        DEBUGP("==>Elapsed "#id ":" #point "-%5u delta %5u total\n",\
                marktime-id ## ExecStopWatchLast, \
                marktime-id ## ExecStopWatchStart); \
        id ## ExecStopWatchLast=marktime; \
        }


//Timer for tracking the execution time between two points for a specified
//number of iterations. Tracks min,max, and average for the specified # of iterations. 
//EXECTIMER(id, count)  Defines a timer uniquely identified by 'id'
//     id=unique id for this timer
//     count=# of iterations between prints (see EXECTIMER_STAT) 
//EXECTIMER_START(id)  begins the timer
//EXECTIMER_STAT stores the intermediate time and prints collected and stats
//  if 'count' iterations have been
//EXECTIMER_PRINT prints the elapsed time since the last EXECTIMER_START if 'maxtime'
//   was exceeded, does not affect operation EXECTIMER_STAT

//define the instance
#define EXECTIMER(id,count) \
    unsigned int id ## ExecStopWatchStart=0; \
    unsigned int id ## ExecStopWatch=0; \
    unsigned int id ## ExecTotal=0; \
    unsigned int id ## ExecTime=0; \
    unsigned int id ## ExecMinTime=UINT_MAX; \
    unsigned int id ## ExecMaxTime=0; \
    int id ## ExecIndex=0;  \
    int id ## ExecCount=count;

//gathers average, max, and mins stats for for execution time
//created with EXECTIME_CREATE
//'id' is a unique identifier that will appear in the print
#define EXECTIMER_START(id) \
        id ## ExecStopWatchStart= UT_GetCurMsec();

//Print time since EXECTIME_START if greater than 'maxtime'
#define EXECTIMER_PRINT(id,maxtime)  { \
    int execTime = UT_GetCurMsec() - id ## ExecStopWatchStart; \
    if (execTime >= maxtime) { \
        DEBUGP("==>Exec "#id " elapsed time %u msec\n", execTime);\
    }\
}

//Print time in minutes and seconds
#define EXECTIMER_PRINT_MINSEC(id,maxtime)  { \
    int execTime = UT_GetCurMsec() - id ## ExecStopWatchStart; \
    if (execTime >= maxtime) { \
        DEBUGP("==>Exec "#id " elapsed time %3d.%02d.%03d sec\n",\
                (execTime/1000)/60,\
                (execTime/1000)%60,\
                execTime%1000);\
    }\
}

#define EXECTIMER_EXTERN(id) \
    extern unsigned int id ## ExecStopWatchStart; \
    extern unsigned int id ## ExecStopWatch; \
    extern unsigned int id ## ExecTotal; \
    extern unsigned int id ## ExecTime; \
    extern unsigned int id ## ExecMinTime; \
    extern unsigned int id ## ExecMaxTime; \
    extern int id ## ExecIndex;  \
    extern int id ## ExecCount;




#define EXECTIMER_PRINT_MINSEC_STRING(str,id,maxtime) {\
    int execTime = UT_GetCurMsec() - id ## ExecStopWatchStart; \
    if (execTime >= maxtime) { \
        sprintf(str,"==>Exec "#id " elapsed time %3d.%02d.%03d sec\n",\
                (execTime/1000)/60,\
                (execTime/1000)%60,\
                execTime%1000);\
    }\
}

   




//Return value since last EXECTIMER_START (note semicolon needed)
#define EXECTIMER_ELAPSED(id) UT_GetCurMsec() - id ## ExecStopWatchStart 
    
//Print min, max, and average times that have been collected between EXECTIMER_START
#define EXECTIMER_STAT(id) \
    id ## ExecStopWatch = UT_GetCurMsec(); \
    id ## ExecTime = id ## ExecStopWatch - id ## ExecStopWatchStart; \
    id ## ExecTotal += id ## ExecTime; \
    id ## ExecIndex++;\
    if (id ## ExecTime>id ## ExecMaxTime)  id ## ExecMaxTime=id ## ExecTime; \
    if (id ## ExecTime<id ## ExecMinTime)  id ## ExecMinTime=id ## ExecTime; \
    if (id ## ExecIndex==id ## ExecCount){ \
        id ## ExecIndex=0; \
        DEBUGP("==>Exec "#id " average %u Max: %u  Min: %u\n", id ## ExecTotal/id ## ExecCount,id ## ExecMaxTime,id ## ExecMinTime); \
       id ## ExecTotal=0; \
       id ## ExecIndex=0; \
       id ## ExecMaxTime=0; \
       id ## ExecMinTime=UINT_MAX; \
    } 

//Same as EXECTIMER but timing in nSec
//define the instance
#define EXECTIMER_NS(id,count) \
    struct timespec id ## ExecStopWatchStart; \
    unsigned long int id ## ExecTotal=0; \
    unsigned long int id ## ExecMinTime=UINT_MAX; \
    unsigned int id ## ExecMaxTime=0; \
    int id ## ExecIndex=0;  \
    int id ## ExecCount=count;

//gathers average, max, and mins stats for for execution time
//created with EXECTIME_CREATE
//'id' is a unique identifier that will appear in the print
#define EXECTIMER_START_NS(id) \
        clock_gettime(CLOCK_MONOTONIC,&id##ExecStopWatchStart); 


//Print time since EXECTIME_START if greater than 'maxtime'
#define EXECTIMER_PRINT_NS(id,maxtime)  { \
    struct timespec t;\
    clock_gettime(CLOCK_MONOTONIC,&t); \
    long int secs; \
    long int nsecs; \
    secs=t.tv_sec-id ## ExecStopWatchStart.tv_sec; \
    nsecs=(t.tv_nsec-id ## ExecStopWatchStart.tv_nsec)+(secs*1000000000l); \
    if (nsecs >= maxtime) { \
        DEBUGP("==>Exec "#id " elapsed time %u nSec\n", execTime);\
    }\
}

//Return value since last EXECTIMER_START (note semicolon needed)
//Not implemented
//#define EXECTIMER_ELAPSED_NS(id) UT_GetCurMsec() - id ## ExecStopWatchStart 
    
//Print min, max, and average times that have been collected between EXECTIMER_START
#define EXECTIMER_STAT_NS(id) \
    { \
    struct timespec t; \
    clock_gettime(CLOCK_MONOTONIC,&t); \
    long int secs; \
    long int nsecs; \
    secs=t.tv_sec-id ## ExecStopWatchStart.tv_sec; \
    nsecs=(t.tv_nsec-id ## ExecStopWatchStart.tv_nsec)+(secs*1000000000l); \
    id ## ExecTotal+=nsecs; \
    id ## ExecIndex++;\
    if (nsecs > id ## ExecMaxTime)  id ## ExecMaxTime=nsecs; \
    if (nsecs < id ## ExecMinTime)  id ## ExecMinTime=nsecs; \
    }\
    if (id ## ExecIndex==id ## ExecCount){ \
        id ## ExecIndex=0; \
        DEBUGP("==>Exec "#id " average (nS) %lu Max: %lu  Min: %lu\n", id ## ExecTotal/id ## ExecCount,id ## ExecMaxTime,id ## ExecMinTime); \
       id ## ExecTotal=0; \
       id ## ExecIndex=0; \
       id ## ExecMaxTime=0; \
       id ## ExecMinTime=UINT_MAX; \
    } 






#else

#define STOPWATCH_TIMER
#define STOPWATCH_START
#define STOPWATCH(a,b,c) {}


#define TIMER_ELAPSED(id) 
#define TIMER_ELAPSED_EXTERN(id) 
#define TIMER_ELAPSED_START(id) 
#define TIMER_ELAPSED_MARK(id,point)

#define LOOPTIMER(id,count)
#define LOOPTIMER_PRINTTIME(id,maxtime) 
#define LOOPTIMER_STAT(id,maxtime) 

#define EXECTIMER(id,count)  
#define EXECTIMER_ELAPSED(id)
#define EXECTIMER_PRINTTIME(id,maxtime) 
#define EXECTIMER_START(id) 
#define EXECTIMER_STAT(id) 

#define EXECTIMER_PRINT(id,maxtime)  
#define EXECTIMER_PRINT_MINSEC(id,maxtime)  
#define EXECTIMER_NS(id,count)  
//#define EXECTIMER_ELAPSED(id) //Implement later
#define EXECTIMER_PRINT_NS(id,maxtime) 
#define EXECTIMER_START_NS(id) 
#define EXECTIMER_STAT_NS(id) 












#if 1

//Keep track of the min,max, and average of an integer value (i.e. number of queue elements)
//'id' is a unique id used to generate unique internal variable names, all related macro calls must use the same id
//'count' is the number of iterations that will be collected for statistics (min,max,avg)
//Define instance
#define STATS(id,count) \
    unsigned int id ## StatCount=count; \
    unsigned int id ## StatIndex=0; \
    unsigned int id ## StatTotal=0; \
    unsigned int id ## StatMax=0; \
    unsigned int id ## StatMin=UINT_MAX; 

//Add a value to the collection 
#define STATS_COLLECT(id,val) \
    if (val>id ##StatMax)  id ##StatMax=val;  \
    if (val < id ## StatMin)  id ## StatMin=val; \
    id ## StatTotal += val; \
    id ## StatIndex++; \
    if (id ## StatIndex==id ## StatCount){ \
        id ## StatIndex=0; \
        DEBUGP("==> "#id " average %u Max: %u  Min: %u\n", id ## StatTotal/id ## StatCount,id ## StatMax,id ## StatMin); \
       id ## StatTotal=0; \
       id ## StatIndex=0; \
       id ## StatMax=0; \
       id ## StatMin=UINT_MAX; \
    }

#else

#define STATS(id,count)
#define STATS_COLLECT(id,val) 

#endif

#endif   //ENABLE_TIMERS


#endif


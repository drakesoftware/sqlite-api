/// \file utils.cpp
/// \brief  miscellaneous functions

#include "utils.h"
#include <errno.h>
//Return true if a and b are < 1%  different
bool compare(float a, float b, float precision)
{
    float c=a-b;
    if (c  == 0.0)
    {
        return true;
    }


    //Ratio >= 100/1 are OK
    if (precision == 0.0)
    {
        if (((fabs(a)/fabs(c))<100) || (fabs(b)/fabs(c))<100)
        {
            return true;
        }
        return false;
    }
    else if(fabs(c) < fabs(precision))
    {
        return  true;
    }
    return false;
}


int bitsize(unsigned int val)
{
    int count=1;
    if (!val)
    {
        return 0;
    }

    //If MSB is set, will never exit
    //So, check and handle
    if (val & MSB_UINT) //Is the top bit set
    {
        //Figure out how big UINTS are. (INT_MAX is one bit less) 
        return bitsize((unsigned int) INT_MAX)+1;
    }
    while (val >> count)
    {
        count++;
    }

  return count;
} 

char *binarystring(unsigned int value, char *str, int sep)
{
    int index=0;
    for (int i=(sizeof(unsigned int)*8)-1;i>=0;i--)
    {
        if (value & (1<<i))
        {
           str[index]='1';
        }
        else
        {
           str[index]='0';
        }
        index++;
        if (sep && !(i%sep))
        {
            str[index]=' ';
            index++;
        }
    }
    str[index]=0; //Terminate string
    return str; 
}

char *binarystring(unsigned long value, char *str, int sep)
{
    int index=0;
    for (int i=(sizeof(unsigned long)*8)-1;i>=0;i--)
    {
        if (value & (1<<i))
        {
           str[index]='1';
        }
        else
        {
           str[index]='0';
        }
        index++;
        if (sep && !(i%sep))
        {
            str[index]=' ';
            index++;
        }
    
    }
    str[index]=0; //Terminate string
    return str; 
}

int msleep(unsigned long milisec)
{
    struct timespec req={0,0},rem={0,0};
    time_t sec=(int)(milisec/1000);
    milisec=milisec-(sec*1000);
    req.tv_sec=sec;
    req.tv_nsec=milisec*1000000L;
    nanosleep(&req,&rem);
     return 1;
}


//#define LOG_TIME_FORMAT "%d-%m-%Y %I:%M:%S"
#define LOG_TIME_FORMAT "%I:%M:%S %d-%m-%Y "

char* getTimeStamp(char* bufstring)
{

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(bufstring,80,LOG_TIME_FORMAT,timeinfo);

    return bufstring;
}


unsigned long UT_GetCurMsec()
{
//   struct timeval tv;
//   if ( gettimeofday(&tv, NULL) < 0 )
//      return 0;
	
//   return (unsigned long)((tv.tv_sec * 1000) + (tv.tv_usec/1000));

   unsigned long ms;
	struct timespec t1;

   if (0 == clock_gettime(CLOCK_MONOTONIC,&t1)){
      ms = t1.tv_sec * 1000 + t1.tv_nsec / 1000000;
   }else{
      ms = 0;
   }

	return ms;
}



//For converting signed integer to raw bit fields
union SignedInt
{
    int16_t signedval;
    uint16_t rawval;
};

//Convert a two's completement number to raw, unsigned value
uint16_t toUnsigned(int16_t val)
{
    SignedInt a;
    a.signedval=val;
    return a.rawval;
}

#if 0
void ut_rtrim(char *src, char *dst)
{
	int i;
	int len = strlen(src);
	memcpy(dst, src, len);
	for(i=len-1;i>0;i--){
		if(!isspace(*(dst+i))){
			*(dst+i+1) = 0x00;
			break;
		}
	}
}


void ut_ltrim(char *src, char *dst)
{
	int len;

	while(*src){
		if(isspace(*src))
			break;
		else
			src++;
	}

	len = strlen(src);
	memcpy(dst, src, len);
}

void ut_trim(char *src, char *dst)
{
	int i, len;

	while(*src){
		if(isspace(*src))
			break;
		else
			src++;
	}

	len = strlen(src);
	memcpy(dst, src, len);
	for(i=len-1;i>0;i--){
		if(!isspace(*(dst+i))){
			*(dst+i+1) = 0x00;
			break;
		}
	}
}

void UT_makeUpper(char *src)
{
	int i;
	int len = strlen(src);
	for(i=0;i<len;i++){
		*(src+i) = toupper(*(src+i));
	}
}

int UT_getFileSize(int fd)
{
	int fsize = 0;
	
	if(fd>0){
		fsize = lseek(fd, 0L, SEEK_END);
	}

	return fsize;
}



#endif


/// \file CapabilityCode.h
/// \brief  CapabilityCode  DO-282 2.2.4.5.4.12
/// UAT IN Capability DO-282 2.2.4.5.4.12.1
/// 1090ES IN Capability DO-282 2.2.4.5.4.12.2
/// TCAS/ACAS Operational DO-282 2.2.4.5.4.12.3

#ifndef __CapabilityCode__h
#define __CapabilityCode__h
#include "RegressionTests.h"

class CapabilityCodes:public Validity
{

    unsigned char mEncodedValue;
	public:

    static const unsigned char UATIN = 0x1;

    static const unsigned char ESIN =  0x2; //1090 ES in 

    static const unsigned char TCAS =  0x4;

    static const unsigned char NONE = 0x0;
    //Aircraft has UAT Receive Capability

   
    CapabilityCodes()=default; 
   
    CapabilityCodes(unsigned char v)
    {
        mEncodedValue=v;
        setValid();
    }

    static bool Test(bool verbose);

    void set(unsigned int options)
    {
        mEncodedValue = options;
        setValid();
    }

   bool get(void)
    {
      return mEncodedValue;
    }

  
};

#endif

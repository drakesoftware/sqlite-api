/// \file OperationalMode.h
/// \brief  OperationalMode  DO-282 2.2.4.5.4.13
/// TCAS/ACAS Resolution Advisory Flag DO-282 2.2.4.5.4.13.1
/// IDENT Switch Active DO-282 2.2.4.5.4.13.2

#ifndef __OperationalModes__h
#define __OperationalModes__h
#include "RegressionTests.h"

class OperationalModes:public Validity
{

    public:
        unsigned int mEncodedValue;
        //TCSS Resolution Advisory
        static const unsigned char TCAS_RA_ADVISORY = 0x1;

        //Ident swtich active
        static const unsigned char IDENT_ACTIVE = 0x2;

        //ATC (Unused)
        static const unsigned char RECEIVING_ATC = 0x4;


    void setEncoded(uint8_t val)
    {
        mEncodedValue=val;
        setValid();
    }

    static bool Test(bool verbose);

    void set(unsigned int options)
    {
        mEncodedValue = options & (TCAS_RA_ADVISORY|IDENT_ACTIVE); //ATC reserved, 
        setValid();
    }

    uint8_t get(void)
    {
      return mEncodedValue;
    }

    OperationalModes()=default;

    OperationalModes(uint8_t val)
    {
        setEncoded(val);
    }
};

#endif

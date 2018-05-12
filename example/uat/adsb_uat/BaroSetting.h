// \file BaroSetting.h
/// \brief Encode/decode ADSB barometricPressureSetting
#ifndef __baroSetting_h
#define __baroSetting_h

#include <stdio.h>
#include <limits.h>
#include "Validity.h"

class BaroSetting:public Validity
{
    int mBPS;  //Values in mB * 10


    unsigned int encode(int baroSetting);
    unsigned int decode( unsigned int value);


    public:

    static const unsigned int BAROMETRIC_PRESSURE_SETTING_UNAVAILABLE = 0x0;      //Encode  barometricPressureSetting unavailable.
    static const unsigned int BAROMETRIC_PRESSURE_SETTING_ENCODED_MAX=0x1ff;      //Maximum ncoded barometricPressureSetting value, 
    static const int BAROMETRIC_PRESSURE_SETTING_MINIMUM_ENCODEABLE= 8000;        //Minimum encodeable barometricPressureSetting
    
    //Maximum value that can be encoded (mB * 10) 
    static const int BAROMETRIC_PRESSURE_SETTING_MAXIMUM_ENCODEABLE= BAROMETRIC_PRESSURE_SETTING_MINIMUM_ENCODEABLE + 4080;
                                                                           
    static const int BAROMETRIC_PRESSURE_SETTING_BIT_RESOLUTION= 8;              //barometricPressureSetting encoding mB/bit (* 10)



    int setUnavailable(void)
    {
        mBPS=BAROMETRIC_PRESSURE_SETTING_UNAVAILABLE;
        setValid();
    }
    ///Decode the state vector longitude to integer feet
   

    ///Set standard units millibars (after substracting 800mB offset)
    ///\param barometric_millibars  //Millibars *10   
    bool setStandard(int barometric_millibars)
    {
        if (barometric_millibars >BAROMETRIC_PRESSURE_SETTING_MAXIMUM_ENCODEABLE)
        {
            setUnavailable();
        }
        else
        {
           mBPS=encode(barometric_millibars);
        }
        setValid();
    } 
   
    bool setEncoded(unsigned int encoded_barometric)
    {
        mBPS=encoded_barometric;
        setValid();

    } 
    int getEncoded(void)
    {
        return mBPS;
    }

    unsigned int get(void)
    {
        return decode(mBPS);
    }

    BaroSetting()=default;

    BaroSetting(int mb)
    {
        setStandard(mb);
    }


    ///Regression test UAT_TEST must be enabled in Makefile
    static bool Test(bool verbose);
    static bool TestDecode(bool verbose);
    static bool TestEncode(bool verbose);


};


#endif


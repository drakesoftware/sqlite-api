// \file SelectedAltitude.h
/// \brief Encode/decode ADSB selected altitude DO-282 2.2.4.5.6.2
#ifndef __selected_altitude_h
#define __selected_altitude_h

#include <stdio.h>
#include <limits.h>
#include "Validity.h"


class SelectedAltitude: public Validity
{
    int mAltitude;     


    unsigned int encode(int altitude);
    int decode( unsigned int value);


    public:

    static const unsigned int ALTITUDE_SETTING_UNAVAILABLE = 0x0;      //Encode altitude unavailable.
    static const unsigned int ALTITUDE_SETTING_ENCODED_MAX=0xfff;      //Maximum encoded value, 
    static const int ALTITUDE_SETTING_MINIMUM_ENCODEABLE= 0;        //Minimum encodeable altitude
    static const int ALTITUDE_SETTING_MAXIMUM_ENCODEABLE= 65472;       //Maximum encodeable altitude
    static const int ALTITUDE_SETTING_BIT_RESOLUTION= 32;              //Altitude encoding feet/bit



    int setUnavailable(void)
    {
        mAltitude=ALTITUDE_SETTING_UNAVAILABLE;
    }
    ///Decode the state vector longitude to integer feet
   
   
    bool setStandard(int altitude_feet)
    {
        if (altitude_feet >ALTITUDE_SETTING_MAXIMUM_ENCODEABLE)
        {
            setUnavailable();
        }
        else
        {
           mAltitude=altitude_feet;
        }
        setValid();
    } 
   
    bool setEncoded(unsigned int encoded_altitude)
    {
        mAltitude=decode(encoded_altitude);
        setValid();
    } 
    int getEncoded(void)
    {
        return encode(mAltitude);
    }

    unsigned int get(void)
    {
        return mAltitude;
    }

    SelectedAltitude()=default;

    SelectedAltitude(int feet)
    {
        setStandard(feet);
    }


    ///Regression test UAT_TEST and  UAT_ALTITUDE_TEST must be enabled in Makefile
    static bool Test(bool verbose);
    static bool TestDecode(bool verbose);
    static bool TestEncode(bool verbose);

};


#endif


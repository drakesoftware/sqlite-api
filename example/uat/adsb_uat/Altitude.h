// \file Altitude.h
/// \brief Encode/decode ADSB altitude
#ifndef __altitude_h
#define __altitude_h

#include <stdio.h>
#include <limits.h>
#include "utils.h"
#include "Validity.h"
#include <string>

///2.2.4.5.2.3
struct Altitude: public Validity
{
    int mAltitude;     

    private:
    unsigned int encode(int altitude);
    int decode(void);


    public:

    static const unsigned int ALTITUDE_UNAVAILABLE = 0x0;      //Encode altitude altitude unavailable.
    static const unsigned int ALTITUDE_ENCODED_MAX=0xfff;      //Maximum encoded altitude value, 
    static const int ALTITUDE_MINIMUM_ENCODEABLE= -1000;        //Minimum encodeable altitude
    static const int ALTITUDE_MAXIMUM_ENCODEABLE= 101337;       //Maximum encodeable altitude
    static const int ALTITUDE_BIT_RESOLUTION= 25;              //Altitude encoding feet/bit
/*
#define ALTITUDE_UNAVAILABLE  0x0      //Encode altitude altitude unavailable.
#define ALTITUDE_ENCODED_MAX 0xfff      //Maximum encoded altitude value, 
#define ALTITUDE_MINIMUM_ENCODEABLE -1000        //Minimum encodeable altitude
#define ALTITUDE_MAXIMUM_ENCODEABLE 101337       //Maximum encodeable altitude
#define ALTITUDE_BIT_RESOLUTION 25              //Altitude encoding feet/bit
*/


    Altitude(void)
    {
        clear();
    }
    ///Constructor for encoded value
    Altitude(int val)
    {
        encode(val);
        setValid();
    }


    Altitude(unsigned int val)
    {
        mAltitude=val;
        setValid();
    }

    int setUnavailable(void)
    {
        mAltitude=ALTITUDE_UNAVAILABLE;
        setValid();
    }
    ///Decode the state vector longitude to integer feet
   
   
    bool setStandard(int altitude_feet)
    {
        if (altitude_feet >ALTITUDE_MAXIMUM_ENCODEABLE)
        {
            setUnavailable();
        }
        else
        {
            encode(altitude_feet);
            setValid();
        }
    } 
   
    bool setEncoded(unsigned int encoded_altitude)
    {
        mAltitude=encoded_altitude;
        setValid();

    } 
    int getEncoded(void)
    {
        return mAltitude;
    }

    int getStandard(void)
    {
        return decode();
    }

    std::string toString(void)
    {
        return toFormattedString(getStandard(),10,6);
    }

    ///Regression test UAT_TEST and  UAT_ALTITUDE_TEST must be enabled in Makefile
#ifdef UAT_TEST
    static bool Test(bool verbose=false);
    static bool TestDecode(bool verbose);
    static bool TestEncode(bool verbose);
#endif

};


#endif


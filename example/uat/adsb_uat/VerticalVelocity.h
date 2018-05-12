// \file VerticalVelocity.h
/// \brief Encode/decode ADSB vertical velocity
#ifndef __vvelocity_h
#define __vvelocity_h

#include <stdio.h>
#include <limits.h>

#include "ADSB_Fields.h"

#include "utils.h"



//DO-282 2.2.4.5.2.7
///Vertical velocity field
///11 bits total
///Bit definitions:
///0-8  bits are vertical velocity 1 bit = 64feet/min
///9    VV Source 1=Barometric, 0=GNSSS or INS
///10   VV Sign 1=DOWN, 0=UP
class VerticalVelocity:public Validity
{
    unsigned int mVerticalVelocity_encoded;     
    bool mAvailable;
    ///Convert signed velocity (feet/min)
    unsigned int encodeVerticalVelocity(int vvel);
    int decodeVerticalVelocity( unsigned int value);


    public:

    CONST_INT VV_UNAVAILABLE = 0; 
    CONST_INT VV_SIGN_SHIFT = 9;    
    CONST_INT VV_SIGN_MASK =  1 << VV_SIGN_SHIFT;
    CONST_INT VV_SOURCE_SHIFT = 10;
    CONST_INT VV_SOURCE_MASK = 1 << VV_SOURCE_SHIFT;
    CONST_INT VV_VALUE_SIZE = 11;
    CONST_INT VV_VALUE_MASK = BITMASK_LSBS(VV_VALUE_SIZE);
    CONST_INT VV_BIT_RESOLUTION = 64;
    CONST_INT VV_MAX_ENCODEABLE = 32608;  //Encode values >= to all 1's
    CONST_INT VV_ENCODED_MAX = 0x1ff;    //Maximum value > 32,608 feet
    int setUnavailable(void)
    {
        mVerticalVelocity_encoded=VV_UNAVAILABLE;
        mAvailable=false;
    }
    ///Decode the state vector longitude to integer feet
   
 

    ///Set value from standard units
    ///\param bSourceBaro   True if source is barometric, 
    ///                     false otherwise (GNSS/INS)
    void setStandard(bool bSourceBaro, bool bDown, int vvelocity_feet)
    {
        //TODO:  Test handling of out of range. should be capped at VV_ENCODED_MAX
        if (vvelocity_feet >VV_MAX_ENCODEABLE) 
        {
            mVerticalVelocity_encoded=VV_ENCODED_MAX; //Out of range
        }
        else
        {
           mVerticalVelocity_encoded=encodeVerticalVelocity(vvelocity_feet);
        }
        if (bDown)
        {
            mVerticalVelocity_encoded |= VV_SIGN_MASK;  //True is down (negative)
        }

        if (bSourceBaro)
        {
            mVerticalVelocity_encoded |= VV_SOURCE_MASK; //True is barometric
        }
        mAvailable=true;
        setValid();

    } 
  

    void setEncoded(unsigned int encoded_vvelocity)
    {
        setValid();
        mVerticalVelocity_encoded=encoded_vvelocity;
    }

    VerticalVelocity()=default;
    VerticalVelocity(bool bSourceBaro, bool bDown, int vvelocity_feet)
    {
        setStandard(bSourceBaro, bDown, vvelocity_feet);
    }

    VerticalVelocity(unsigned int encoded_velocity)
    {
        setEncoded(encoded_velocity);
    }

    int getEncoded(void)
    {
        return mVerticalVelocity_encoded;
    }

    unsigned int getStandard(void)
    {
        return decodeVerticalVelocity(mVerticalVelocity_encoded);
    }

    bool isUpward(void)
    {
        return !(mVerticalVelocity_encoded  &  VV_SIGN_MASK);
    }

    bool isBaroSource(void)
    {
        return (mVerticalVelocity_encoded & VV_SOURCE_MASK);
    }

    bool isAvailable(void)
    {
        return mAvailable;
    }


    ///Regression test UAT_TEST and  UAT_ALTITUDE_TEST must be enabled in Makefile
    static bool Test(bool verbose=false);
    static bool TestDecode(bool verbose);
    static bool TestEncode(bool verbose);


};


#endif


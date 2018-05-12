/// \file Latitude.h
/// \brief  ADSB UAT Latitude encode/decode

#ifndef __latitude_h
#define __latitude_h

#include <stdio.h>

#include "ADSB_Types.h"

////This class stores the Latitude value in the encoded form
///which differs from most other field implementations
struct Latitude:public Validity
{

    uint64_t  mLatitude;                     

   
    unsigned int encodeLat(PRECISION_FLOAT lat)
    {
        const uint64_t FIXEDMULT=100000000000l;
        int64_t lat_integer=(uint64_t) (lat * (uint64_t) FIXEDMULT );
        const int64_t LSB_INT= LSB * FIXEDMULT;
        if (lat_integer==90*FIXEDMULT)
        {
            mLatitude=0x400000;
        }
        else if (lat_integer==0)
        {
            mLatitude=0;
        }
        else if (lat_integer==-90*FIXEDMULT)
        {
            mLatitude=0x400000;
        }
        else// if (lat_integer > 0)
        {
            mLatitude=lat_integer/LSB_INT;
        }
        mLatitude &=LATITUDE_FIELD_MASK;
        setValid();
        //printf("Latitude: %8x: %15.12f\n",mLatitude,lat);
        return mLatitude;
    }

    Latitude(PRECISION_FLOAT lat)
    { 
        setValid();
        encodeLat(lat);
    }

    Latitude(unsigned int val)
    {
        setValid();
        mLatitude=val;  //TODO error checking
    }


    Latitude(void)
    {
        clear();
    }



    PRECISION_FLOAT decodeLat()
    {
        PRECISION_FLOAT latitude_float=0.0;
        unsigned int value=mLatitude;
        unsigned int msbs=value >> 22;
        if (value == 0x0)
        {
            latitude_float = 0.0;
        }
        else if (value == 0x400000)
        {
            latitude_float= 90.0;
        }
        else if (value == 0xc00000)
        {
            latitude_float= -90.0;
        }
        else if ( value == 0x1)
        {
            latitude_float=LSB;
        }
        else if (value  == 0xffffff)
        {
            latitude_float=-LSB;
        }
        else if (msbs == 0x0)
        {
            latitude_float=value * LSB;
        }
        else if (msbs == 1)
        {   
            value &= 0x3fffff;
            latitude_float= -((PRECISION_FLOAT) 90.0 - (PRECISION_FLOAT)(value * LSB));
        }
        else if (msbs == 1 || msbs == 2)
        {
            clear();
            printf("Latitude in quadrants 2 or 3: %8x\n",value);
        }
        //printf("Latitude: %8x, %15.12f \n",value, latitude_float);
        return latitude_float; 
    }

    PRECISION_FLOAT getStandard(void)
    {
        return  decodeLat();
    }
 


    ///Gets the last encoded or decoded Latitude
    /// @param value state vector value as defined in 2.24.5.2.1
    PRECISION_FLOAT getDecoded(void)
    {
        return decodeLat();
    }

    unsigned int getEncoded(void)
    {
        return mLatitude;
    }

    ///Set with encoded value
    bool setEncoded(unsigned int latitude)
    {

        mLatitude=latitude;
        setValid();
    }

    ///unique set* function needed. Types of encoded and decoded
    ///values are similar
    bool setStandard(PRECISION_FLOAT lat)
    {
        setValid();
        mLatitude=encodeLat(lat);
    }

    std::string toString(void)
    {
        return toFormattedString(getStandard(),10,6);
    }


    ///Convert UAT state vector latitude field
    ///Note North and south pole are both encoded as 0x400000 
    /// (Per DO-282 2.2.4.5.2.1 Note 1)
    ///@param value 23 bit value from ADSB state vector as defined in 2.24.5.2.1
    ///@returns Floating point (in decimal degrees)latitude from   where -90 < latitude <= 90
 
    ///Encode the floating point value into the latiude state vector field format
    ///@params lat   Latitude where -90 <= latitude <= 90
    ///@returns Encoded value per DO-282 2.2.4.5.2.1 (-90 degrees has same encoding as +90- Note 1) 

    ///Regression test UAT_TEST and  UAT_LATLON_TEST must be enabled in Makefile
    static bool Test(bool verbose);

};


#endif

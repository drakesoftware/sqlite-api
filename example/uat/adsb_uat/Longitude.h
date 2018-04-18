/// \file Longitude.h
/// \brief  ADSB UAT Longitude encode/decode

#ifndef __longitude_h
#define __longitude_h

#include <stdio.h>

#include "ADSB_Types.h"
#include "Validity.h"

struct Longitude:public Validity
{
    //Longitutde is encoded in encoded form, converted 
    //to standard form as necessary.
    unsigned int mLongitude;

    ///Decode the state vector longitude to a float
    ///@param value  Longitude value
    ///@returns Floating point value (in decimal degrees) 
    /// -180 <= longitude <= 180
    PRECISION_FLOAT decodeLon(void)
    {
        PRECISION_FLOAT longitude_float=0.0;
        unsigned int value=mLongitude;
        unsigned int msbs=value >> 22;
        unsigned int lsbs = value & LONGITUDE_LSB_MASK;
        if (value == 0x0)
        {
            longitude_float = 0.0;
        }
        else if (value == 0x400000)
        {
            longitude_float= 90.0; //East
        }
        else if ( value == 0x1)
        {
            longitude_float=LSB;
        }
        else if (value  == 0xffffff)
        {
            longitude_float=-LSB; //West
        }
        else if (msbs == 0x0)
        {
            longitude_float = lsbs * LSB;
        }
        else if (msbs == 0x1)
        {
            longitude_float = 90.0 + (lsbs * LSB);
        }
        else if (msbs == 0x2)
        {
            longitude_float = -((PRECISION_FLOAT) 180.0 - (lsbs * LSB));
        }
        else if (msbs == 0x3)
        {
            longitude_float = -((PRECISION_FLOAT) 90.0 - (lsbs * LSB));
        }
        setValid();
//        printf("Longitude %8x: %17.12f\n",mLongitude,longitude_float);
        return longitude_float; 
    }

    Longitude(PRECISION_FLOAT lon)
    {
        encodeLon(lon);
        setValid();
    }

    Longitude(unsigned int val)
    {
        mLongitude = val;
        setValid();
    }
       

    Longitude(void)
    {
        clear();
    }

    ///Encode the floating point value into the longitude state vector field format
    ///@params lon   Longitude where -180 <= longitude <= 180
    ///@returns Encoded value per DO-282 2.2.4.5.2.1 
    void  encodeLon(PRECISION_FLOAT lon)
    {
        const uint64_t FIXEDMULT=100000000000;
        int64_t lon_integer=(int64_t) (lon * FIXEDMULT );
        const int64_t LSB_INT= LSB *FIXEDMULT;
        if (lon_integer==90000000)
        {
            mLongitude=0x400000;
        }
        else if (lon_integer==0)
        {
            mLongitude=0;
        }
        else if (lon_integer==-90000000)
        {
            mLongitude=0x400000;
        }
        else if (lon_integer > 0)
        {
            mLongitude=lon_integer/LSB_INT;
        }
        //printf("Longitude %8x: %17.12f\n",mLongitude,lon);
    }

    public:

    ///Input is encoded value
    void setEncoded(unsigned int lon)
    {
        mLongitude=lon;
        setValid();
    }

    ///Input is standard units 
    void setStandard(PRECISION_FLOAT lon)
    {
        encodeLon(lon);
        setValid();
    }


    ///Retrieve value in encoded form
    unsigned int getEncoded(void)
    {
        return mLongitude;
    }


    ///Retrieve value in standard form
    PRECISION_FLOAT getStandard(void)
    {
        return decodeLon();
    }

    std::string toString(void)
    {
        return toFormattedString(getStandard(),10,6);
    }


    ///Regression test UAT_TEST and  UAT_LON_TEST must be enabled in Makefile
    static bool Test(bool verbose);

};


#endif

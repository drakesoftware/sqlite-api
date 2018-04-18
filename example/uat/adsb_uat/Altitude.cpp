/// \file Altitude.cpp
/// \brief  Regression test routines for ADSB (UAT) Altitudefield

#include "Altitude.h"
#include "utils.h"

#include "RegressionTests.h"

struct altitude_test
{
    unsigned int encoded_value;
    int standard_value;
};



int Altitude::decode(void)
{
    int altitude_feet;
    if (mAltitude > 0)
    {
        altitude_feet = ALTITUDE_MINIMUM_ENCODEABLE + ((mAltitude-1) * ALTITUDE_BIT_RESOLUTION);
    }
    else
    {
        altitude_feet=INT_MIN;
    }
    return altitude_feet;
}


unsigned int Altitude::encode(int altitude)
{
    unsigned int encoded_altitude;
    if (altitude < ALTITUDE_MINIMUM_ENCODEABLE || altitude > ALTITUDE_MAXIMUM_ENCODEABLE)
    {
        mAltitude= -INT_MAX;
        encoded_altitude=ALTITUDE_UNAVAILABLE;
    }
    else
    {
        mAltitude=(((altitude - ALTITUDE_MINIMUM_ENCODEABLE)+(ALTITUDE_BIT_RESOLUTION/2))/ALTITUDE_BIT_RESOLUTION) + 1;
    }
    //printf("Altitude: %d feet: value:%3x\n",mAltitude, encoded_altitude); 
    return encoded_altitude;
}


///Values from DO-282 Table 2-79
altitude_test DO282TestValues_Altitude[]
{
    {0x1,    -1000},
    {0x5,    -900},
    {0xa,    -775},
    {0x15,   -500},
    {0x2a,   25},
    {0x55,   1100},
    {0xaa,   3225},
    {0x155,  7500},
    {0x2aa,  16025},
    {0x555,  33100},
    {0xaaa,  67225},
    {0xdaa,  86425},
    {0xeaa,  92825},
    {0xfda,  100425},
    {0xfea,  100825},
    {0xffa,  101225},
    {0xffd,  101300},
    {0xffe,  101325}
};


#ifdef UAT_TEST
bool Altitude::TestDecode(bool verbose)
{
    bool pass=true;
#ifdef UAT_TEST
    Altitude alt;

    for(int i=0;i<sizeof(DO282TestValues_Altitude)/sizeof(altitude_test);i++)
    {
        int altitude;

        alt.setStandard(DO282TestValues_Altitude[i].standard_value);

        altitude=alt.getStandard();

        if (altitude !=  DO282TestValues_Altitude[i].standard_value)
        {
            printf("Decode discrepancy at %3x .  %d ft expected:%d ft\n", DO282TestValues_Altitude[i].encoded_value,altitude, DO282TestValues_Altitude[i].standard_value);
            pass=false;
        }
        
    }
#else
    printf("Altitude regression test support not enabled\n");
#endif 
    return !pass;
}


bool Altitude::TestEncode(bool verbose)
{

    bool pass=true;
#ifdef UAT_TEST
    Altitude alt;
    for(int i=0;i<sizeof(DO282TestValues_Altitude)/sizeof(altitude_test);i++)
    {
        alt.setStandard(DO282TestValues_Altitude[i].standard_value);

        unsigned int altitude_enc=alt.getEncoded();

        if (altitude_enc != DO282TestValues_Altitude[i].encoded_value)
        {
            printf("Encode fail: Altitude: %7d , expected:%03x \n", 
                    DO282TestValues_Altitude[i].standard_value,
                    DO282TestValues_Altitude[i].encoded_value);
            

            pass=false;
        }
    }
#endif 
    return !pass;

}


static TestEntry testlist[]=
{
    {Altitude::TestDecode,"Altitude Decode Test"},
    {Altitude::TestEncode,"Altitude Encode Test"},
    {NULL,NULL}
};


bool Altitude::Test(bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef UAT_VELOCITY_TEST
    pass=!RunTestList(testlist,verbose);
#endif
    return !pass;

}

#endif


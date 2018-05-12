/// \file SelectedAltitude.cpp
/// \brief  Regression test routines for ADSB (UAT) Altitudefield

#include "SelectedAltitude.h"
#include "utils.h"

#include "RegressionTests.h"

struct altitude_setting_test
{
    unsigned int encoded_value;
    int standard_value;
};



int SelectedAltitude::decode( unsigned int value)
{
    if (value > 0)
    {
        mAltitude = ALTITUDE_SETTING_MINIMUM_ENCODEABLE + ((value-1) * ALTITUDE_SETTING_BIT_RESOLUTION);
    }
    else
    {
        mAltitude = ALTITUDE_SETTING_UNAVAILABLE;
    }
    return mAltitude;
}


unsigned int SelectedAltitude::encode(int altitude_setting)
{
    unsigned int encoded_altitude_message;
    if (altitude_setting < ALTITUDE_SETTING_MINIMUM_ENCODEABLE || altitude_setting > ALTITUDE_SETTING_MAXIMUM_ENCODEABLE)
    {
        mAltitude= -INT_MAX;
        encoded_altitude_message=ALTITUDE_SETTING_UNAVAILABLE;
    }
    else
    {
        mAltitude= altitude_setting;
        encoded_altitude_message=(((altitude_setting - ALTITUDE_SETTING_MINIMUM_ENCODEABLE)+(ALTITUDE_SETTING_BIT_RESOLUTION/2))/ALTITUDE_SETTING_BIT_RESOLUTION) + 1;
    }
    //printf("Altitude: %d feet: value:%3x\n",mAltitude, encoded_altitude_message); 
    return encoded_altitude_message;
}


///Values from DO-282 Table 2-79
altitude_setting_test AltitudeValues[]
{
    {0x01,      0},
    {0x2,       32},
    {0x3,      64},
    {0x7fe,  65440},
    {0x7ff,   65472}
};


bool SelectedAltitude::TestDecode(bool verbose)
{
    bool pass=true;
#ifdef UAT_TEST
    SelectedAltitude alt;

    for(int i=0;i<sizeof(AltitudeValues)/sizeof(altitude_setting_test);i++)
    {
        int altitudeSetting;

        alt.setStandard(AltitudeValues[i].standard_value);

        altitudeSetting=alt.getStandard();

        if (altitudeSetting !=  AltitudeValues[i].standard_value)
        {
            printf("Decode discrepancy at %3x .  %d ft expected:%d ft\n", AltitudeValues[i].encoded_value,altitudeSetting, AltitudeValues[i].standard_value);
            pass=false;
        }
        
    }
#else
    printf("SelectedAltitude regression test support not enabled\n");
#endif 
    return !pass;
}


bool SelectedAltitude::TestEncode(bool verbose)
{

    bool pass=true;
#ifdef UAT_TEST
    SelectedAltitude alt;
    for(int i=0;i<sizeof(AltitudeValues)/sizeof(altitude_setting_test);i++)
    {
        alt.setStandard(AltitudeValues[i].standard_value);

        unsigned int altitude_enc=alt.getEncoded();

        if (altitude_enc != AltitudeValues[i].encoded_value)
        {
            printf("Encode fail: Altitude Setting: %7d , expected:%03x \n", 
                    AltitudeValues[i].standard_value,
                    AltitudeValues[i].encoded_value);
            

            pass=false;
        }
    }
#endif 
    return !pass;

}


static TestEntry testlist[]=
{
    {SelectedAltitude::TestDecode,"Altitude Setting Decode Test"},
    {SelectedAltitude::TestEncode,"Altitude Setting Encode Test"},
    {NULL,NULL}
};


bool SelectedAltitude::Test(bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef UAT_TEST
    pass=!RunTestList(testlist,verbose);
#endif
    return !pass;

}



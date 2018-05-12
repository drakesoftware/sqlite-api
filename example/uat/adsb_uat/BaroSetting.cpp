/// \file BaroSetting.cpp
/// \brief  Regression test routines for ADSB (UAT) Altitudefield

#include "BaroSetting.h"
#include "utils.h"

#include "RegressionTests.h"

struct barometric_pressure_setting_test
{
    unsigned int encoded_value;
    int standard_value;
};



unsigned int BaroSetting::decode( unsigned int value)
{
    unsigned int retval;
    if (value > 0)
    {
        retval = BAROMETRIC_PRESSURE_SETTING_MINIMUM_ENCODEABLE + ((value-1) * BAROMETRIC_PRESSURE_SETTING_BIT_RESOLUTION);
    }
    else
    {
        retval= BAROMETRIC_PRESSURE_SETTING_UNAVAILABLE;
    }
    return retval;
}

//TODO review against standard
unsigned int BaroSetting::encode(int barometric_pressure_setting)
{
    unsigned int encoded_pressure_setting_message;
    if (barometric_pressure_setting < BAROMETRIC_PRESSURE_SETTING_MINIMUM_ENCODEABLE || barometric_pressure_setting > BAROMETRIC_PRESSURE_SETTING_MAXIMUM_ENCODEABLE)
    {
        encoded_pressure_setting_message=BAROMETRIC_PRESSURE_SETTING_UNAVAILABLE;
    }
    else
    {
        encoded_pressure_setting_message=(
                                         ((barometric_pressure_setting - BAROMETRIC_PRESSURE_SETTING_MINIMUM_ENCODEABLE) +  //Subtract 800(*10) minimum value
                                          (BAROMETRIC_PRESSURE_SETTING_BIT_RESOLUTION/2))
                                         /BAROMETRIC_PRESSURE_SETTING_BIT_RESOLUTION) 
                                         + 1; //0mB = 1
    }
    //printf("Altitude: %d feet: value:%3x\n",mBPS, encoded_pressure_setting_message); 
    return encoded_pressure_setting_message;
}


///Values from DO-282 Table 2-79
barometric_pressure_setting_test BaroSettingValues[]
{
    {0x01,       0},
    {0x2,        8},
    {0x3,       16},
    {0x1fe,   4072},
    {0x1ff,   4080}
};


bool BaroSetting::TestDecode(bool verbose)
{
    bool pass=true;
#ifdef UAT_TEST
    BaroSetting alt;

    for(int i=0;i<sizeof(BaroSettingValues)/sizeof(barometric_pressure_setting_test);i++)
    {
        int BaroSetting;

        alt.setStandard(BaroSettingValues[i].standard_value);

        BaroSetting=alt.getStandard();

        if (BaroSetting !=  BaroSettingValues[i].standard_value)
        {
            printf("Decode discrepancy at %3x .  %d ft expected:%d ft\n", BaroSettingValues[i].encoded_value,BaroSetting, BaroSettingValues[i].standard_value);
            pass=false;
        }
        
    }
#else
    printf("BaroSetting regression test support not enabled\n");
#endif 
    return !pass;
}


bool BaroSetting::TestEncode(bool verbose)
{

    bool pass=true;
#ifdef UAT_TEST
    BaroSetting alt;
    for(int i=0;i<sizeof(BaroSettingValues)/sizeof(barometric_pressure_setting_test);i++)
    {
        alt.setStandard(BaroSettingValues[i].standard_value);

        unsigned int baro_enc=alt.getEncoded();

        if (baro_enc != BaroSettingValues[i].encoded_value)
        {
            printf("Encode fail: Barometric Pressure Setting: %7d , expected:%03x \n", 
                    BaroSettingValues[i].standard_value,
                    BaroSettingValues[i].encoded_value);
            

            pass=false;
        }
    }
#endif 
    return !pass;

}


static TestEntry testlist[]=
{
    {BaroSetting::TestDecode,"Barometric Pressure Setting Decode Test"},
    {BaroSetting::TestEncode,"Barometric Pressure Setting Encode Test"},
    {NULL,NULL}
};


bool BaroSetting::Test(bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef UAT_TEST
    pass=!RunTestList(testlist,verbose);
#endif
    return !pass;

}



/// \file VerticalVelocity.cpp
/// \brief  Regression test routines for ADSB (UAT) VerticalVelocityfield

#include "RegressionTests.h"
#include "VerticalVelocity.h"
#include "utils.h"



int VerticalVelocity::decodeVerticalVelocity( unsigned int value)
{
    int vvel;
    if (value > 0)
    {
        vvel =   ((value-1) * VV_BIT_RESOLUTION);
    }
    else
    {
        mAvailable=false;
        vvel=0;  //Also a valdi value
    }
    
    return vvel;
}


//*******************************************************
//Regression test functions

struct vvel_test
{
    unsigned int encoded_value;
    int standard_value;
};


unsigned int VerticalVelocity::encodeVerticalVelocity(int vvel)
{
    unsigned int encoded_vvel;
    if (vvel > VV_MAX_ENCODEABLE) //TODO: Handle corner case
    {
        encoded_vvel=VV_ENCODED_MAX;
    }
    else
    {
        encoded_vvel=((vvel +(VV_BIT_RESOLUTION/2))/VV_BIT_RESOLUTION) + 1; //TODO:  Encoding for vvel
    }
    //printf("VerticalVelocity: %d feet: value:%3x\n",mVerticalVelocity_encoded, encoded_vvel); 
    mAvailable=true;
    return encoded_vvel;
}

//*******************************************************************************
//Test data and functions
//*****
///Values from DO-282 Table 2-79
vvel_test DO282TestValues_VerticalVelocity[]
{
    {0x5,   256},
    {0xa,   576},
    {0xf,   896},
    {0x50,  5056},
    {0x5f,  6016},
    {0xa0,  10176},
    {0xaf,  11136},
    {0xff,  16256},
    {0x100, 16320},
    {0x155, 21760},
    {0x1aa, 27200},
    {0x1fe, 32576}   
};


bool VerticalVelocity::TestDecode(bool verbose)
{
    bool pass=true;
#ifdef UAT_TEST
    VerticalVelocity vv;

    for(int i=0;i<sizeof(DO282TestValues_VerticalVelocity)/sizeof(vvel_test);i++)
    {
        unsigned int vvel;
        vv.setEncoded(DO282TestValues_VerticalVelocity[i].encoded_value);

        vvel=vv.getStandard();

        if (vvel !=  DO282TestValues_VerticalVelocity[i].standard_value)
        {
            printf("Decode discrepancy at %3x .  %d ft expected:%d ft\n", DO282TestValues_VerticalVelocity[i].encoded_value,vvel, DO282TestValues_VerticalVelocity[i].standard_value);
            pass=false;
        }
        
    }
#else
    printf("VerticalVelocity regression test support not enabled\n");
#endif 
    return !pass;
}


bool VerticalVelocity::TestEncode(bool verbose)
{

    bool pass=true;
#ifdef UAT_TEST
    VerticalVelocity vv;
    for(int i=0;i<sizeof(DO282TestValues_VerticalVelocity)/sizeof(vvel_test);i++)
    {
        vv.setStandard(false,false,DO282TestValues_VerticalVelocity[i].standard_value); //Add sign/source combinations

        unsigned int vvel_enc=vv.getEncoded();

        if (vvel_enc != DO282TestValues_VerticalVelocity[i].encoded_value)
        {
            printf("Encode fail: VerticalVelocity: %7d -> %03x expected:%03x \n", 
                    DO282TestValues_VerticalVelocity[i].standard_value,
                    vvel_enc,
                    DO282TestValues_VerticalVelocity[i].encoded_value);

            pass=false;
        }
    }
#endif 
    return !pass;

}


static TestEntry testlist[]=
{
    {VerticalVelocity::TestDecode,"VerticalVelocity Decode Test"},
    {VerticalVelocity::TestEncode,"VerticalVelocity Encode Test"},
    {NULL,NULL}
};

bool VerticalVelocity::Test(bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef UAT_VELOCITY_TEST

    pass=!RunTestList(testlist,verbose);
#endif
    return !pass;

}



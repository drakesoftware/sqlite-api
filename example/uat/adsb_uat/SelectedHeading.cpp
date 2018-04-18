/// \file SelectedHeading.cpp
/// \brief  Regression test routines for ADSB (UAT) Altitudefield

#include "SelectedHeading.h"
#include "utils.h"

#include "RegressionTests.h"

const float SelectedHeading::DEGREES_PER_BIT=((float)180/256); 

struct selected_heading_test
{
    unsigned int encoded_value;
    float standard_value;  //heading -180 to +180 (179.3)
};



PRECISION_FLOAT SelectedHeading::decode(unsigned int value)
{
    PRECISION_FLOAT heading=(DEGREES_PER_BIT * (value & BITMASK_LSBS(SELECTED_HEADING_SIZE_BITS))); 
    if (value &  1 << SELECTED_HEADING_SIGN_OFFSET_BIT)
    {
        heading*=-1.0;
    }
    return heading;
}


unsigned int SelectedHeading::encode(PRECISION_FLOAT selected_heading, bool heading_valid)
{
    PRECISION_FLOAT heading=(fabs(selected_heading)/DEGREES_PER_BIT);
    unsigned int encoded_value=floor(heading);

    if(selected_heading < 0)
    {
        encoded_value |= (1 << SELECTED_HEADING_SIGN_OFFSET_BIT);
    }

    if (heading_valid)
    {
        encoded_value |= (1 << SELECTED_HEADING_STATUS_OFFSET_BIT);
    }

    return encoded_value;

}


///Values from DO-282 Table 2-79
selected_heading_test SelectedHeadingValues[]
{
    {0x200,        0.0},
    {0x201,        0.703125},
    {0x202,        1.406250},
    {0x2ff,        179.296875},
    {0x302,       -1.406250},
    {0x3ff,       -179.296875}
};


bool SelectedHeading::TestDecode(bool verbose)
{
    bool pass=true;
#ifdef UAT_TEST
    SelectedHeading sh;

    for(int i=0;i<sizeof(SelectedHeadingValues)/sizeof(selected_heading_test);i++)
    {
        float heading;

        sh.setEncoded(SelectedHeadingValues[i].encoded_value);

        heading=sh.getStandard();

        if (!compare(heading,SelectedHeadingValues[i].standard_value,.005))
        {
            printf("Decode discrepancy at %03x .  %3.4f degrees expected:%3.4f degrees\n", SelectedHeadingValues[i].encoded_value,heading, SelectedHeadingValues[i].standard_value);
            pass=false;
        }
        
    }
#else
    printf("SelectedHeading regression test support not enabled\n");
#endif 
    return !pass;
}


bool SelectedHeading::TestEncode(bool verbose)
{

    bool pass=true;
#ifdef UAT_TEST
    SelectedHeading sh;
    for(int i=0;i<sizeof(SelectedHeadingValues)/sizeof(selected_heading_test);i++)
    {
        sh.setStandard(SelectedHeadingValues[i].standard_value,true);

        unsigned int selectedheading_enc=sh.getEncoded();

        if (selectedheading_enc != SelectedHeadingValues[i].encoded_value)
        {
            printf("Encode fail: %3.4f Selected Heading: %04x , expected:%04x \n", 
                    SelectedHeadingValues[i].standard_value,
                    selectedheading_enc,
                    SelectedHeadingValues[i].encoded_value);
            

            pass=false;
        }
    }
#endif 
    return !pass;

}


static TestEntry testlist[]=
{
    {SelectedHeading::TestDecode,"Selected Heading Decode Test"},
    {SelectedHeading::TestEncode,"Selected Heading Encode Test"},
    {NULL,NULL}
};


bool SelectedHeading::Test(bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef UAT_TEST
    pass=!RunTestList(testlist,verbose);
#endif
    return !pass;

}



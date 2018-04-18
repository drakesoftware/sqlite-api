// \file Longitude.cpp
/// \brief  UAT ADSB Longitude testing values


#include "Longitude.h"
#include "utils.h"
#ifdef UAT_TEST


///Test values for DO-282 Table 2-76
///Table values have been correct, but still do not agree
///with field encoding/decoding as described in secion 2.2.4.5.2.1 
///and Table 2-14

pos_test  DO282TestValues_Longitude[] =
{
    {     0x0,   0.0000000000000},
    {     0x1,   0.0000214576721},
    {     0x3,   0.0000643730164},
    {     0x6,   0.0001287460327},
    {     0xd,   0.0002789497375},
    {    0x1b,   0.0005793571472},
    {    0x36,   0.0011587142944},
    {    0x6d,   0.0023388862610},
    {    0xdb,   0.0046992301941},
    {    0x1b,   0.0005793571472},
    {   0x36c,   0.0187969207764},
    {   0x6d8,   0.0375938415527},
    {   0xdb0,   0.0751876831055},
    {  0x1b60,   0.1503753662109},
    {  0x36c0,   0.3007507324219},
    {  0x6d80,   0.6015014648438},
    {  0xdb00,   1.2030029296875},
    { 0x1b600,   2.4060058593750},
    { 0x36c00,   4.8120117187500},
    { 0x6d800,   9.6240234375000},
    { 0xdb100,  19.2535400390625},
    {0x1b6000,  38.4960937500000},
    {0x36c000,  76.9921875000000},
    {0x6d8000, 153.9843750000000},
    {0xdb0000, -52.0312500000000},
    {0xb60000,-104.0625000000000},
    {0x6c0000, 151.8750000000000},
    {0xd80000, -56.2500000000000},
    {0xb00000,-112.5000000000000},
    {0x600000, 135.0000000000000},
    {0xc00000, -90.0000000000000},
    {0x800000,-180.0000000000000},
    {0x400000,  90.0000000000000}
};





#endif


bool Longitude::Test(bool verbose)
{
    bool pass=true;
    Longitude lon;
#ifdef UAT_TEST
    for(int i=0;i<sizeof(DO282TestValues_Longitude)/sizeof(pos_test);i++)
    {
        float lon_float;
        unsigned int lon_int;

        lon.setEncoded(DO282TestValues_Longitude[i].intvalue);

        lon_float=lon.getStandard();
        if (!compare(lon_float,DO282TestValues_Longitude[i].fvalue))
        {
            printf("Discrepancy %f\n", DO282TestValues_Longitude[i].fvalue);
            pass=false;
        }

        lon.setStandard(DO282TestValues_Longitude[i].fvalue);
        lon_int=lon.getEncoded();

        if (lon_int != DO282TestValues_Longitude[i].intvalue)
        {
            printf("Encode fail: lon: %9x -> %f expected:%9x \n",lon_int, lon_float,DO282TestValues_Longitude[i].intvalue);
            pass=false;
        }

    }

#else
    printf("Longitude regression test support not enabled\n");
#endif
    return !pass;
}



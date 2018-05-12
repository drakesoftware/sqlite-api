// \file Latitude.cpp
/// \brief  UAT ADSB Latitude testing values


#include "Latitude.h"
#include "utils.h"
#ifdef UAT_TEST


///Test values for DO-282 Table 2-76
///Table values have been correct, but still do not agree
///with field encoding/decoding as described in secion 2.2.4.5.2.1 
///and Table 2-14
//Values to not match precisely when multiplying LSB * encoded value
//but they are within 1/2 LSB
pos_test DO282TestValues_Latitude[] =
{
    {0x0, 0.000000000},
    {0x1, 0.000026822},
    //{0x2, 0.000042915},
    {0x2, 0.000048280},
    {0x5, 0.000112653},
    {0xa, 0.000219941},
    {0x14, 0.000434518},
    {0x29, 0.000885129},
    {0x52, 0.001764894},
    {0xa5, 0.003545880},
    {0x14a, 0.007086396},
    {0x294, 0.014167428},
    {0x528, 0.028329492},
    {0xa50, 0.056653619},
    {0x14a0, 0.113301873},
    {0x2940, 0.226598382},
    {0x5280, 0.453191400},
    {0xa500, 0.906377435},
    {0x14a00, 1.812749505},
    {0x29400, 3.625495051},
    {0x52800, 7.250981927},
    {0xa5000, 14.501958489},
    {0x14a000, 29.003911614},
    {0x294000, 58.007817864},
    {0x400000, 90.000000000},
    {0x510000, -66.093755364},
    {0x550000, -60.468755364},
    {0x400000, 90.000000000}
};

#endif


bool Latitude::Test(bool verbose)
{
    bool pass=true;
#ifdef UAT_TEST
    Latitude lat;
    for(int i=0;i<sizeof(DO282TestValues_Latitude)/sizeof(pos_test);i++)
    {
        float lat_float;
        unsigned int lat_int;

        lat.setEncoded(DO282TestValues_Latitude[i].intvalue);
        lat_float=lat.getDecoded();
        if (!compare(lat_float,DO282TestValues_Latitude[i].fvalue,LSB/2))
        {
            PRECISION_FLOAT err=fabs(lat_float)-fabs(DO282TestValues_Latitude[i].fvalue);
            printf("Decode fail %15.12f<>%15.12f \n", lat_float,DO282TestValues_Latitude[i].fvalue);
            printf("Error: %15.12f   %d\n",
                    err,
                    int ((err/LSB)*100));
                    
                    
            pass=false;
        }
 

        lat.setDecoded(DO282TestValues_Latitude[i].fvalue);
        lat_int=lat.getEncoded();
        if (lat_int != DO282TestValues_Latitude[i].intvalue)
        {
            printf("Encode fail: lat: %9x -> %f expected:%9x \n",lat_int, lat_float,DO282TestValues_Latitude[i].intvalue);
            pass=false;
        }
    }

#else
    printf("Latitude regression test support not enabled\n");
#endif
    return !pass;
}



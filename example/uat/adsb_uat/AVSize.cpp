/// \file AVSize.cpp
/// \brief  AVSize encode/decode per DO-282 2.2.4.5.2.7.2

#include <stdio.h>
#include "AVSize.h"

#include "RegressionTests.h"

    AVSIZE_UNKNOWN   = 0,
        AVSIZE_15_WIDE   = 1,
        AVSIZE_25_NARROW = 2,
        AVSIZE_25_WIDE   = 3,
        AVSIZE_35_NARROW = 4,
        AVSIZE_35_WIDE   = 5,
        AVSIZE_45_NARROW = 6,
        AVSIZE_45_WIDE   = 7,
        AVSIZE_55_NARROW = 8,
        AVSIZE_55_WIDE   = 9,
        AVSIZE_65_NARROW = 0xa,
        AVSIZE_65_WIDE   = 0xb,
        AVSIZE_75_NARROW = 0xc,
        AVSIZE_75_WIDE   = 0xd,
        AVSIZE_85_NARROW = 0xe,
        AVSIZE_85_WIDE   = 0xf


struct GPSOffsetTestStruct
{
    unsigned char encoded_value;
    int standard_value;
    bool DecodeTestValid;
    AVSize::DIRECTION direction;     //Don't care for longitudinal
};

//Longitudinal test dataa
//No SETLAT bit 5 is 0
//Set LON axis bit in test 

#define SETLON(x) (x|1<<AVSize::AXIS_OFFSET)


GPSOffsetTestStruct GPSLonOffsetTestData[]=
{
    {SETLON(0x2), 2, true,AVSize::LEFT}, //right=don't care
    {SETLON(0x5), 8, true,AVSize::LEFT},
    {SETLON(0xa), 18, true,AVSize::LEFT},
    {SETLON(0xf), 28, true,AVSize::LEFT},
    {SETLON(0x1f),60, true,AVSize::LEFT},
    {SETLON(0x1f),61, false,AVSize::LEFT},
    {SETLON(0x1f),71, false,AVSize::LEFT}
};


GPSOffsetTestStruct GPSLatOffsetTestData[]=
{
    {0x1<<2, 2, true,AVSize::LEFT}, //Left
    {0x2<<2, 4, true,AVSize::LEFT},
    {0x3<<2, 6, true,AVSize::LEFT},
    {0x3<<2, 9, false,AVSize::LEFT},

    {0x4<<2, 0, true,AVSize::RIGHT}, //Right
    {0x5<<2, 2, true,AVSize::RIGHT},
    {0x6<<2, 4, true,AVSize::RIGHT},
    {0x7<<2, 6, true,AVSize::RIGHT},
    {0x7<<2, 8, false,AVSize::RIGHT}
};

#ifdef UAT_TEST
bool AVSize::LonEncodeTest(bool verbose)
{
    GPSOffset gpo;
    bool pass=true;
    for (int i=0;i<sizeof(GPSLonOffsetTestData)/sizeof(GPSOffsetTestStruct);i++)
    {
        unsigned char encoded_value;
        gpo.setLongitudinalDistance(GPSLonOffsetTestData[i].standard_value);

        if (!gpo.isValid() && !gpo.isLongitudinal())
        {
            printf("GPS Longitudinal encode, isValid:%d  isLongitudinal:%d\n", gpo.isValid(), gpo.isLongitudinal());
            pass=false;
            continue;
        }
        encoded_value=gpo.getEncodedValue();
        if (encoded_value != GPSLonOffsetTestData[i].encoded_value)
        {
            printf("GPS Longitudinal encode failure result: %x, expected:%x (%i)\n", encoded_value, GPSLonOffsetTestData[i].encoded_value,i);
            pass=false;
        }
    }

    gpo.setNoLongitudinalDistanceData();
    if (!gpo.isLongitudinal() && !gpo.isException() && !gpo.isNoDataAvailable())
    {
        printf("GPS Longitudinal encode: longitudinal: %d exception %d  nodata %d\n",
                gpo.isLongitudinal(),
                gpo.isException(),
                gpo.isNoDataAvailable());
        pass=false;
    }

    gpo.setNoLongitudinalDistanceData();
    if (!gpo.isException() && !gpo.isNoDataAvailable())
    {
        printf("GPS Longitudinal encode: exception %d  nodata %d\n",gpo.isException(),gpo.isNoDataAvailable());
        pass=false;
    }

    return !pass;
}

bool AVSize::LonDecodeTest(bool verbose)
{
    GPSOffset gpo;
    bool pass=true;
    int distance;

    //Test Decoding the encoded value
    for (int i=0;i<sizeof(GPSLonOffsetTestData)/sizeof(GPSOffsetTestStruct);i++)
    {
        unsigned char encoded_value;

        //Skip if Encode only test
        //TODO: change logic EncodeTestOnly
        if (!GPSLonOffsetTestData[i].DecodeTestValid)
        {
            continue;
        }
        gpo.setEncodedValue(GPSLonOffsetTestData[i].encoded_value);

        //Should be longitudinal data
        if (!gpo.isLongitudinal())
        {
            printf("GPS Longitudinal decode failure\n");
            pass=false;
        }

        //Should be valid
        if (!gpo.isValid())
        {
            printf("GPS Longitudinal decode failure: invalid\n") ;
            pass=false;
        }
        
        distance=gpo.getLongitudinalDistance();
        if (distance != GPSLonOffsetTestData[i].standard_value)
        {
            printf("GPS Longitudinal decode failure result: %d, expected:%d (%i)\n", distance, GPSLonOffsetTestData[i].standard_value,i);
            pass=false;

        }
    }

    //NoData value
    gpo.setEncodedValue(SETLON(FS_NODATA)); //0=nodata
    if (!gpo.isLongitudinal())
    {
        printf("GPS Longitudinal decode failure: axis\n");
        pass=false;
    }
    else if (!gpo.isException()) //Should be an exception
    {
        printf("GPS Longitudinal decode failure: Exception NO DATA\n");
        pass=false;
    }
    else if (!gpo.isNoDataAvailable()) //Should be NO DATA
    {
        printf("GPS Longitudinal decode failure: No Data\n");
        pass=false;
    }

    //value=Sensor applied
    gpo.setEncodedValue(SETLON(FS_SENSORAPPLIED)); //0=nodata
    if (!gpo.isLongitudinal())
    {
        printf("GPS Longitudinal decode failure\n");
        pass=false;
    }
    else if (!gpo.isException()) //Should be an exception
    {
        printf("GPS Longitudinal decode failure: Exception, SENSOR APPLIED\n");
        pass=false;
    }
    else if (!gpo.isSensorApplied()) //Should be NO DATA
    {
        printf("GPS Longitudinal decode failure: Sensor Applied");
        pass=false;
    }



    return !pass;
}

bool AVSize::LatEncodeTest(bool verbose)
{
    GPSOffset gpo;
    bool pass=true;
    for (int i=0;i<sizeof(GPSLatOffsetTestData)/sizeof(GPSOffsetTestStruct);i++)
    {

        unsigned char encoded_value;
        gpo.setLateralDistance(GPSLatOffsetTestData[i].standard_value,(DIRECTION) GPSLatOffsetTestData[i].direction);

        //Should be longitudinal data
        if (!gpo.isLateral())
        {
            printf("GPS Lateral encode failure: axis\n");
            pass=false;
        }

        //Should be valid
        if (!gpo.isValid())
        {
            printf("GPS Lateral encode failure: invalid\n") ;
            pass=false;
        }
 
        encoded_value=gpo.getEncodedValue();
        if (encoded_value != GPSLatOffsetTestData[i].encoded_value)
        {
            printf("GPS Lateral encode failure result: %x, expected:%x (%i)\n", encoded_value, GPSLatOffsetTestData[i].encoded_value,i);
            pass=false;
        }

        gpo.setNoLateralDistanceData();

        if (!gpo.isLateral() && !gpo.isException() && !gpo.isNoDataAvailable())
        {
            printf("GPS Lateralal encode: longitudinal: %d exception %d  nodata %d\n",
                    gpo.isLateral(),
                    gpo.isException(),
                    gpo.isNoDataAvailable());
            pass=false;
        }
    }
    return !pass;
}

static const char* leftstr="Left";
static const char* rightstr="Right";

const char *directionstr(AVSize::DIRECTION d)
{
    if (d == AVSize::LEFT)
    {
        return leftstr;
    }
    else
    {
        return rightstr;
    }
}

bool AVSize::LatDecodeTest(bool verbose)
{
    GPSOffset gpo;
    bool pass=true;
    int distance;
    DIRECTION dir;
    for (int i=0;i<sizeof(GPSLatOffsetTestData)/sizeof(GPSOffsetTestStruct);i++)
    {
        unsigned char encoded_value;

        if (!GPSLatOffsetTestData[i].DecodeTestValid)
        {
            continue;
        }
        gpo.setEncodedValue(GPSLatOffsetTestData[i].encoded_value);
        gpo.getLateralDistance(distance,dir);
        if (distance != GPSLatOffsetTestData[i].standard_value)
        {
            printf("GPS Lateral decode distance failure result: %d, expected:%d (%i)\n", distance, GPSLatOffsetTestData[i].standard_value,i);
            pass=false;

        }
        if (dir != (DIRECTION)GPSLatOffsetTestData[i].direction)
        {
            //TODO Revisit print and data values
            printf("GPS Lateral decode direction failure result: %d, expected:%d \n",GPSLatOffsetTestData[i].standard_value,i);
            pass=false;
        }
    }
    return !pass;
}

#endif

//Sizes in decimeters (meters * 10)
LengthClass SizeClasses[]=
{
    {0,0},
    {150,230},
    {250,285},
    {250,340},
    {350,330},
    {350,380},
    {450,395},
    {450,450},
    {550,450},
    {550,520},
    {650,595},
    {650,670},
    {750,725},
    {750,800},
    {850,800},
    {850,900}
};

static const int LASTCLASSINDEX=((sizeof(SizeClasses)/sizeof(LengthClass))-1);

void AVSize::getSize(int &length, int &width)
{
    width=0; //Error, invalid value
    length=0;
    //Find matching entry (Don't rely on index to match 'code' field

    if (unlikely(0==getSizeField())) //Field = 0 (unavailable)
    {
        length=width=0;
    }
    else
    {
        int v=getSizeField(); 
        if (v < NUM_ELEMENT(LengthClass))
        {
            length=SizeClasses[i].length;
            width=SizeClasses[i].width;
        }
    }
}

//*******************************************************************************
//Test data and functions
//*****

//Lengths and widths > maximum should
//be encoded as the maximum value. These will fail
//on the decode test and must not be tested.
struct AVSizeTestEntry
{
    unsigned char encoded;
    int  length; //Max /nominal value
    int  width;  //Max /nominal value
    bool DecodeValid; //True if valid for decode

};

#ifdef UAT_TEST

AVSizeTestEntry avstest[]=
{
    {0x0, 0, 0,true},
    {0x1,150,230,true},
    {0x2,250,285,true},
    {0x3,250,340,true},
    {0x4,350,330,true},
    {0x5,350,380,true},
    {0x6,450,395,true},
    {0x7,450,450,true},
    {0x8,550,450,true},
    {0x9,550,520,true},
    {0xa,650,595,true},
    {0xb,650,670,true},
    {0xc,750,725,true},
    {0xd,750,800,true},
    {0xe,850,800,true},
    {0xf,850,900,true},
    {0xf,900,900,false},
    {0xf,850,925,false}
    //TODO:Add cases that exceed,should map to 0xf
};
 


bool AVSize::AVSizeTestDecode(bool verbose)
{
    bool pass=true;
    AVSize avb;

    for(int i=0;i<sizeof(avstest)/sizeof(AVSizeTestEntry);i++)
    {
        int width,length;
        //Skip tests not valid for decode
        if (!avstest[i].DecodeValid)
        {
            continue;
        }

        avb.setSizeField(avstest[i].encoded); //Set size field
        avb.getSize(length,width);
        if (length != avstest[i].length)
        {
            printf("AVSize length mismatch %d <> %d (meters*10) \n",length, avstest[i].length);
            pass=false;
        }
        if (width != avstest[i].width)
        {
            printf("AVSize width mismatch %d <> %d (meters*10)\n",width, avstest[i].width);
            pass=false;
        }

    }
    return !pass;
}

bool AVSize::AVSizeTestEncode(bool verbose)
{
    bool pass=true;
    AVSize avb;

    for(int i=0;i<sizeof(avstest)/sizeof(AVSizeTestEntry);i++)
    {
        unsigned char encoded_val;
        avb.setSize(avstest[i].length, avstest[i].width);
        encoded_val=avb.getSizeField();
        if (encoded_val != avstest[i].encoded)
        {
            printf("AVSize encode mismatch %x <> %x\n",encoded_val,avstest[i].encoded);
            pass=false;
        }
    }
    return !pass;
}
TestEntry AVSTests[]=
{
    {AVSize::LatEncodeTest, "GPSOffset Lateral Encode"},
    {AVSize::LatDecodeTest, "GPSOffset Lateral Decode"},
    {AVSize::LonEncodeTest, "GPSOffset Longituindal Encode"},
    {AVSize::LonDecodeTest, "GPSOffset Longitudinal Decode"},
    {AVSize::AVSizeTestEncode, "AVSize Encode"},
    {AVSize::AVSizeTestDecode, "AVSize Decode"}
};


bool AVSize::Test(bool verbose)
{
    return RunTestList(AVSTests,verbose);
}


#endif


 

/// \file AVSize.h
/// \brief  AVSize (and GPS Offset) encode/decode per DO-282 2.2.4.5.2.7.2
///

#ifndef __avsize_h
#define __avsize_h

#include "utils.h"
#include "ExceptionState.h"
#include <string>

struct LengthClass
{
    int length;  //Decimeter
    int width; //In decimeters, width
};


///AVSize is used when A/C is "on-ground"
///Per DO-282 2.2.4.5.2.7.2
///Aircraft size and GPS location
/* GPS Antenna offset:

Byte 16       |   Byte 17
|6|5|4|3|2|1|0|7|6|5|4|
^AV Size^GPS Offset  ^

GPS Offset fields

6 bit field
Bit |Definition
5   Axis   0=lateral(pitch), 1=longitudinal(roll)

If Axis=0(lateral)
4   0=left, 1=right
3-2 00=No Data, 01=2meters, 10=4 meters, 11=6 meters 
1-0 Reservd (0's)


If axis=1(longitudinal)
Distance from nose of aircraft
4-0 00000=No data, 00001=Offset applied by sensor
    000010-11111  (value-1) *2 (meters)

*/


///Valid when aircraft A/G state is ONGROUND
///Aircraft size and Offset of the GPS sensor from the nose?? of the aircraft
///Lateral is the distance from the centerline expressed converted from meters.
///and a left/right flag.
///Longitudinal is distance from the nose of the craft converted from meters.
class AVSize:public ExceptionState,public Validity
{

    //Per DO-282B 2.2.4.5.2.7.2 
    //Offset within AVSize field
    static const int AVLENGTH_OFFSET=1;
    static const int AVLENGTH_SIZE=3;
    
    static const int AVWIDTH_OFFSET=0;
    static const int AVWIDTH_SIZE=1;

    //offset/size for entire field
    static const int AVSIZE_OFFSET=0;
    static const int AVSIZE_SIZE=4;
    static const int GPS_SIZE=6;
    static const int GPS_OFFSET=5;



    //GPS Offset constants
    //Give private functions access to public constant defintions
    //Offset of pitch indicating GPS axis
    //selector bit (Needed by test routines
    static const int AXIS_OFFSET = 5;
    static const int AXIS_SIZE   = 1;

    private:
    //Offset and size in bits 
    static const int GPS_LATERAL_DIRECTION_OFFSET = 4;
    static const int GPS_LATERAL_DIRECTION_SIZE   = 1;
    static const int GPS_LATERAL_DISTANCE_OFFSET = 2;
    static const int GPS_LATERAL_DISTANCE_SIZE = 2;
    static const int GPS_LATERAL_MAX_DISTANCE = 6;
    static const int GPS_LONGITUDINALDISTANCE_OFFSET=0;
    static const int GPS_LONGITUDINALDISTANCE_SIZE=5;
    static const int GPS_LONGITUDINALMAX_DISTANCE=60;
    

    unsigned char mGPSField;   //GPS offset Field
    unsigned int mAVSize_encoded; 
    
    public:
    enum AXIS {AXIS_LATERAL=0,AXIS_LONGITUDINAL=1};
    enum DIRECTION{LEFT=0, RIGHT=1};
    
    //SENSORAPPLIED only applies to the lateral axis
    enum FLAGS {NONE=0x0, NODATA=0x1, SENSORAPPLIED=0x2}; 
    enum EXCEPTIONSTATE {STATE_VALID,STATE_NODATA,STATE_SENSORAPPLIED};
    enum FIELD_STATES {FS_NODATA=0x0, FS_SENSORAPPLIED=0x1};


    //\note  Checking for axis (longitudinal/lateral must be done in caller
    bool isLongitudinalDistanceNoData(void)
    {
        return (bool) (FS_NODATA == GETBITFIELD(mGPSField,GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE));
    }

    ///\returns true if field setting matches NO DATA value
    //\note  Checking for axis (longitudinal/lateral must be done in caller
    bool isLateralDistanceNoData(void)
    {
        if(GETBITFIELD(mGPSField,GPS_LATERAL_DIRECTION_OFFSET,GPS_LATERAL_DIRECTION_SIZE+GPS_LATERAL_DISTANCE_SIZE));
        {
            return false;
        }
        return true;
    }

    ///Axis determines if this packet contains lateral or longitudinal offset
    ///data
    ///\param axis   LATERAL or LONGITUDINAL
    void setAxis(AXIS axis)
    {
        SETBITFIELD(mGPSField,axis, AXIS_OFFSET,AXIS_SIZE);
    }

    AXIS getAxis(void)
    {
        return (AXIS) GETBITFIELD(mGPSField,AXIS_OFFSET,AXIS_SIZE);
    }

    bool isLongitudinal(void)
    {
        return (getAxis()==AXIS_LONGITUDINAL);
    }

    bool isLateral(void)
    {
        return (getAxis()==AXIS_LATERAL);
    }

    bool isNoDataAvailable(void)
    {
        if (getAxis()==AXIS_LATERAL)
        {
            return isLateralDistanceNoData();
        }
        return isLongitudinalDistanceNoData();
    }

    bool isSensorApplied(void)
    {
        if (AXIS_LONGITUDINAL == getAxis())
        {
            unsigned char val =GETBITFIELD(mGPSField,GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE);
            if (FS_SENSORAPPLIED == val)
            {
                return true;
            }
        }
        return false;
    }

    void setLongitudinalSensorApplied(void)
    {
        SETBITFIELD(mGPSField,FS_SENSORAPPLIED,GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE);
    }

    void setGPSOffsetEncodedValue(unsigned char val)
    {
        mGPSField=val;

        if (isSensorApplied())
        {
            setExceptionState(STATE_SENSORAPPLIED);
        }
        else if (isNoDataAvailable())
        {
            setExceptionState(STATE_NODATA);
        }
        else
        {
            setExceptionState(STATE_VALID);
        }
    }


    void setNoLateralDistanceData(void)
    {
        setAxis(AXIS_LATERAL);
        SETBITFIELD(mGPSField,0, GPS_LATERAL_DISTANCE_OFFSET,GPS_LATERAL_DISTANCE_SIZE);
        setExceptionState(STATE_NODATA);
    }

    void setNoLongitudinalDistanceData(void)
    {
        setAxis(AXIS_LONGITUDINAL);
        SETBITFIELD(mGPSField,FS_NODATA, GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE);
        setExceptionState(STATE_NODATA);
    }

    ///Set Sensor applied offset value
    void setNoLongitudinalSensorApplied(void)
    {
        setAxis(AXIS_LONGITUDINAL);
        SETBITFIELD(mGPSField,FS_SENSORAPPLIED, GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE);
        setExceptionState(STATE_NODATA);
    }

   
    void setLateralDistance(int meters,DIRECTION d)
    {
        setAxis(AXIS_LATERAL);
        if (unlikely(meters > GPS_LATERAL_MAX_DISTANCE))
        {
            meters=GPS_LATERAL_MAX_DISTANCE;
        }
        if (unlikely(0 == meters))
        {
            d=RIGHT;
        }
        else
        {
            meters>>=1; //Masking done in SETBITFIELD
        }
        SETBITFIELD(mGPSField,d,GPS_LATERAL_DIRECTION_OFFSET,GPS_LATERAL_DIRECTION_SIZE);
        SETBITFIELD(mGPSField,meters,GPS_LATERAL_DISTANCE_OFFSET,GPS_LATERAL_DISTANCE_SIZE);
        setExceptionState(STATE_VALID);
    }

    ///This function only returns valid results if the exception state is STATE_VALID
    void getLateralDistance(int& meters, DIRECTION& d)
    {
        if (isValid())
        {
            d=(DIRECTION)GETBITFIELD(mGPSField,GPS_LATERAL_DIRECTION_OFFSET,GPS_LATERAL_DIRECTION_SIZE);
            meters=GETBITFIELD(mGPSField,GPS_LATERAL_DISTANCE_OFFSET,GPS_LATERAL_DISTANCE_SIZE)<<1;
        }
    }


    void setLongitudinalDistance(int meters,unsigned char flags=0)
    {
        unsigned char val;
        setAxis(AXIS_LONGITUDINAL);
        if (unlikely(flags & NODATA))
        {
            val=FS_NODATA;
            setExceptionState(STATE_NODATA);
        }
        else if (flags & SENSORAPPLIED)
        {
            val=FS_SENSORAPPLIED;
            setExceptionState(STATE_SENSORAPPLIED);
        }
        else
        {
            if (meters > GPS_LONGITUDINALMAX_DISTANCE)
            {
                meters = GPS_LONGITUDINALMAX_DISTANCE;
            }
            val=(meters/2)+1; 
            setExceptionState(STATE_VALID);
        }

        SETBITFIELD(mGPSField,val,GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE);

    }

    ///This function only returns valid results if the exception state is STATE_VALID
    int getLongitudinalDistance(void)
    {
        if(isValid())
        {
            unsigned char v=GETBITFIELD(mGPSField,GPS_LONGITUDINALDISTANCE_OFFSET,GPS_LONGITUDINALDISTANCE_SIZE);
            return ((v-1)*2);
        }
        return -1;
    }

 
    ///Set length field (raw/encoded value) 
    ///(Top 3 bits of size field
    void setLengthField(unsigned char val)
    { 
        mAVSize_encoded=SETBITFIELD(mAVSize_encoded, val, AVLENGTH_OFFSET,AVLENGTH_SIZE);
    }

    ///Set Width field (raw/encoded value) (only MSB used)
    void setWidthField(unsigned char val)
    {
        mAVSize_encoded=SETBITFIELD(mAVSize_encoded, val,  AVWIDTH_OFFSET,AVWIDTH_SIZE);
    }


    ///Get the raw length portion of size  field
    unsigned char getLengthField(void)
    {
        return GETBITFIELD(mAVSize_encoded,AVLENGTH_OFFSET, AVLENGTH_SIZE);
    }

    ///Get the raw AVSize field
    unsigned char getSizeField(void)
    {
        return GETBITFIELD(mAVSize_encoded,AVSIZE_OFFSET, AVSIZE_SIZE);
    }

    //TODO: Remove separate size and width fields
    //Gets width flag (0 or 1), which selects one of the two width values
    unsigned char getWidthField(void)
    {
        return GETBITFIELD(mAVSize_encoded,AVWIDTH_OFFSET, AVWIDTH_SIZE);
    }
  
  

    enum AVSIZE {
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
    };
            
        
    AVSize(void)
    {
        mAVSize_encoded=0;
        clear();
    }
   
    ///Lateral GPS offset  entry 
    ///\param avs  Enum AVSize
    ///\param meters GPS lateral distance from centerline of aircraft
    ///\param d      GPS lateral distance left/right
    ///\param long_meters  GPS distance of GPS aft from nose
    AVSize(AVSIZE avs, int meters,DIRECTION d,int long_meters, unsigned char flags=0)
    {
        setSize(avs);    
        setLateralDistance(meters, d);
        setLongitudinalDistance(long_meters,flags);
        setValid();
    }

    ///Longintudinal GPS offset entry 
    AVSize(AVSIZE avs, int meters,unsigned char flags=0)
    {
        setSize(avs);    
        setLongitudinalDistance(meters,flags);
        setValid();
    };


    ~AVSize(void)
    {
    }


    void setSizeUnknown(void)
    {
        setLengthField(0);
        setWidthField(0);
        setValid();
    }


    ///Set AVSize field, the entire Length&width field
    ///\param length length in meters
    ///\param width  width in decimeters (meters * 10)
    void setSize(AVSIZE avs)
    {
        mAVSize_encoded=(unsigned int) avs;
        setValid();
    }
    //Return size  in decimeters (meters * 10)
    void getSize(int &length, int &width);


    uint8_t getAVSizeEnum(void)
    {
        return mAVSize_encoded;
    }

    uint16_t getEncoded(void)
    {
        uint16_t val;
        SETBITFIELD(val,mAVSize_encoded, AVSIZE_OFFSET, AVSIZE_SIZE);
        SETBITFIELD(val,mGPSField, GPS_OFFSET, GPS_SIZE);
        return val;
    }

    void setEncoded(uint16_t val)
    {
        mAVSize_encoded=GETBITFIELD(val, AVSIZE_OFFSET, AVSIZE_SIZE);
        mGPSField=GETBITFIELD(val, GPS_OFFSET, GPS_SIZE);
        setValid();
    }

    std::string toString(void)
    {
        static const int STRSIZE=70;
        char str[STRSIZE];
        int length;
        int width;
        getSize(length,width);
        //Is GPS offset lateral
        if (isLateral())
        { 
            DIRECTION d;
            int dist;
            getLateralDistance(dist,d);
            snprintf(str,STRSIZE,"L:%4.1f W: %4.1f GPS Off: %d %s",
                 (double) length/10,
                 (double) width/10,
                 dist,
                 ((int)d)?"Right":"Left");
        }
        else
        {
             snprintf(str,STRSIZE,"L:%4.1f W: %4.1f GPS Off %d aft",
                 (double) length/10,
                 (double) width/10,
                 getLongitudinalDistance());
        }
        return std::string(str);
    }
#ifdef UAT_TEST
    static bool AVSizeTestDecode(bool verbose);
    static bool AVSizeTestEncode(bool verbose);

    static bool Test(bool verbose);
#endif
};


#endif


/// \file ADSB_Types.h
/// \brief  UAT  

#ifndef __adsb_types
#define __adsb_types

#include <math.h>
#include <time.h>

#include "EnumClass.h"
#include "ValueClass.h"
#define PRECISION_FLOAT double

#define CONST_INT   static const int 
#define CONST_UNSIGNED  static const unsigned int 

//Float consts can't easily be static
#define CONST_FLOAT  const float



//Value as specified in DO-282 Table 2-14 (Section 2.2.4.5.2.1)
const PRECISION_FLOAT LSB = 0.000021457672f;


const unsigned int LATITUDE_FIELD_MASK = 0x7fffff;  //Mask of bit 23 per SO-282 2.2.4.5.2.1 Note 1
const unsigned int LONGITUDE_LSB_MASK = 0x3fffff;   //Mask LSBs from quadrant

#ifdef UAT_TEST
struct pos_test
{
    unsigned long intvalue;
    float fvalue;
};

#endif

//Per table 2_10
enum EPayloadType
{
    UAT_PAYLOAD_TYPE_0 = 0,
    UAT_PAYLOAD_TYPE_1 = 1,
    UAT_PAYLOAD_TYPE_2 = 2,
    UAT_PAYLOAD_TYPE_3 = 3,
    UAT_PAYLOAD_TYPE_4 = 4,
    UAT_PAYLOAD_TYPE_5 = 5,
    UAT_PAYLOAD_TYPE_6 = 6,
    UAT_PAYLOAD_TYPE_7 = 7,
    UAT_PAYLOAD_TYPE_8 = 8,
    UAT_PAYLOAD_TYPE_9 = 9,
    UAT_PAYLOAD_TYPE_10 = 10,
    UAT_PAYLOAD_TYPE_11_RESERVED,
    UAT_PAYLOAD_TYPE_12_RESERVED,
    UAT_PAYLOAD_TYPE_13_RESERVED,
    UAT_PAYLOAD_TYPE_14_RESERVED,
    UAT_PAYLOAD_TYPE_15_RESERVED,
    UAT_PAYLOAD_TYPE_16_RESERVED,
    UAT_PAYLOAD_TYPE_17_RESERVED,
    UAT_PAYLOAD_TYPE_18_RESERVED,
    UAT_PAYLOAD_TYPE_19_RESERVED,
    UAT_PAYLOAD_TYPE_20_RESERVED,
    UAT_PAYLOAD_TYPE_21_RESERVED,
    UAT_PAYLOAD_TYPE_22_RESERVED,
    UAT_PAYLOAD_TYPE_23_RESERVED,
    UAT_PAYLOAD_TYPE_24_RESERVED,
    UAT_PAYLOAD_TYPE_25_RESERVED,
    UAT_PAYLOAD_TYPE_26_RESERVED,
    UAT_PAYLOAD_TYPE_27_RESERVED,
    UAT_PAYLOAD_TYPE_28_RESERVED,
    UAT_PAYLOAD_TYPE_29_RESERVED,
    UAT_PAYLOAD_TYPE_30_RESERVED_DEVELOPMENT, //Ignore per Table 2-10 Note 3
    UAT_PAYLOAD_TYPE_31_RESERVED_DEVELOPMENT  //Ignore per Table 2-10 Note 3
};

const int LAST_SUPPORTED_PAYLOAD_TYPE = UAT_PAYLOAD_TYPE_6;

typedef EnumClass<EPayloadType> PayloadType;





enum EEquipageClass 
{
    EQUIP_UNKNOWN=0,
    EQUIP_A0,
    EQUIP_A1L,
    EQUIP_A1S,
    EQUIP_A1H,
    EQUIP_A2,
    EQUIP_A3,
    EQUIP_B0,
    EQUIP_B1S,
    EQUIP_B1,
    EQUIP_B2,
    EQUIP_B3
};

typedef EnumClass<EEquipageClass> EquipageClass;


///Altitude type as specified in DO-282 Section 2.2.4.5.2.2
///getType returns 0 or one which can be placed in bit 8 
enum EAltitudeType{ALTITUDE_TYPE_PRESSURE=0, ALTITUDE_TYPE_GEOMETRIC=1};

typedef EnumClass<EAltitudeType> AltitudeType;
const char* const AltTypeNames[]={"Pressure","Geometric"};




///Navigation Integrity Category --DO-282 2.2.4.5.2.4
enum ENIC   //NavigationIntegrityCategory
{
    NIC_UNKNOWN=    0x0,
    NIC_20NM=       0x1,
    NIC_8NM=        0x2,
    NIC_4NM=        0x3,
    NIC_2NM=        0x4,
    NIC_1NM=        0x5,
    NIC__6NM=       0x6,
    NIC__3NM=       0x7,
    NIC__1NM=       0x8,
    NIC_75M=        0x9,
    NIC_25M=        0xa,
    NIC_7_5M=       0xb,
    NIC_RESERVED0=  0xc,
    NIC_RESERVED1=  0xd,
    NIC_RESERVED2=  0xe,
    NIC_RESERVED3=  0xf
};

typedef EnumClass<ENIC> NIC;

///Air/Ground state DO-282 2.2.4.5.2.5
enum EAirGroundState
{
    AG_SUBSONIC=    0x0,
    AG_SUPERSONIC=  0x1,
    AG_ONGROUND=    0x2,
    AG_RESERVED=    0x3
};

typedef EnumClass<EAirGroundState> AirGroundState;

enum EAngleType
{
    NOT_AVAILABLE= 0x0,
    TRUE_TRACK   = 0x1,
    MAG_HEADING  = 0x2,
    TRUE_HEADING = 0x3,
    ANGLE_TYPE_COUNT,

};

typedef EnumClass<EAngleType> AngleType;

const unsigned char AG_AIRBORNE_MASK   = 0x02;
const unsigned char AG_SUPERSONIC_MASK = 0x01;

const char* const AirGroundStagesNames[]={"Subsonic","Supersonic", "On Ground","A/G reserved"};

///Time in UTC  DO-282 2.2.4.5.2.8
enum EUTCCoupling
{
    UTC_UNCOUPLED=0,
    UTC_COUPLED=1
};

typedef EnumClass<EUTCCoupling> UTCCoupling;

enum EEmergencyStatus
{
    NOEMERGENCY = 0,
    GENERALEMERGENCY = 1,
    MEDICALEMERGENCY = 2,
    MINFUEL = 3,
    NOCOMM = 4,
    UNLAWFULINTERFERENCE = 5,
    DOWNEDAIRCRAFT = 6,
    RESERVED = 7
};
typedef EnumClass<EEmergencyStatus> EmergencyStatus;

//TODO  Consolidate constants to minimal number of files
//MOPS SIZE is in ModeStatus.h
static const int MOPS_SIZE_BITS=3;
typedef ValueClass<MOPS_SIZE_BITS> MOPSVersion;



///Uplink feedback info
///Reported values are fabricated, no channel statistic
///are maintained
///TODO: replace dummy Data
class UplinkFeedback:public Validity
{
    uint8_t mUPLFeedback;
    static const int UPLINK_CHANNELS=32;
    //static const int UPLINK_SAMPLES_PER_CHANNEL; 
//    unsigned int channel_feedback[UPLINK_CHANNELS];

#if 0
    UplinkFeedback(void)
    {

        for (int i=0;i<UPLINK_CHANNELS;i++)
        {
            channel_feedback[i]=0;
        }
    }
#endif
    public:    
    UplinkFeedback()=default;

    UplinkFeedback(uint8_t value)
    {
        mUPLFeedback=value;
        setValid();

    }

    uint8_t get(void)
    {
        return mUPLFeedback&0x7;
    }

    void set(uint8_t value)
    {
        mUPLFeedback=value&0x7;
        setValid();
    }

    //Mark as unset/invalid
    void clearValid(void)
    {
        clear();
    }

    ///Set the channel feedback for utc_second 
    void setChannelFeedback(time_t utc_second, int received_count)
    {
        unsigned char score;
        if (received_count >31)
        {
            score=0x7;
        }
        else if (received_count > 30) //31
        {
            score=0x6;
        }
        else if (received_count> 28)
        {
            score=0x5;
        }
        else if (received_count>25)
        {
            score=0x4;
        }
        else if (received_count>21)
        {
            score=0x3;
        }
        else if (received_count > 13)
        {
            score=0x2;
        }
        else if (received_count > 1)
        {
            score=0x1;
        }
        else
        {
            score=0x0;
        }
//        channel_feedback[utc_second % UPLINK_CHANNELS];
    }


};

enum ENACV
{
    NACV_UNKNOWN = 0, 
    NACV_LT10 = 1, //less than 10m/s
    NACV_LT3 = 2,
    NACV_LT1 = 3,
    NACV_LTPOINT3 = 4, //less than .3m/s
    NACV_RESERVED1 = 5,
    NACV_RESERVED2 = 6,
    NACV_RESERVED3 = 7,
};

typedef EnumClass<ENACV> NACV;


enum ENACP
{
    NACP_GT10 = 0, //greater than 10 NM
    NACP_LT10 = 1, //less than 10NM
    NACP_LT4 = 2,
    NACP_LT2 = 3,
    NACP_LT1 = 4,
    NACP_LTPOINT5 = 5, //less than .5 NM
    NACP_LTPOINT3 = 6, //less than .3 NM
    NACP_LTPOINT1 = 7, //less than .1 NM
    NACP_LTPOINT05 = 8, //less than .05 NM
    NACP_LT30M = 9, //less than 30m
    NACP_LT10M = 10, //less than 10m
    NACP_LT3M = 11, //less than 3m
    NACP_RESERVED1 = 12,
    NACP_RESERVED2 = 13,
    NACP_RESERVED3 = 14,
    NACP_RESERVED4 = 15    
};
   
typedef EnumClass<ENACP> NACP;

///2.2.4.5.4.11
enum ENICBaro
{
    NICB_UNCHECKED = 0,  //barometic altitue not crosschecked
    NICB_CROSSCHECKED = 1
};

typedef EnumClass<ENICBaro> NICBaro;

//TODO Consolidate field size info
static const int TXMSO_SIZE_BITS=6;
typedef ValueClass<TXMSO_SIZE_BITS> TXMSO;



//Call Sign Identification- DO282B 2.2.4.5.4.14
enum ECSIDFlag
{
  CSID_CHECKED = 0, //Call Sign/Flight Plan ID field will contain Flight Plan ID
  CSID_UNCHECKED = 1 //Call Sign/Flight Plan ID will contain the Call Sign
};
typedef EnumClass<ECSIDFlag> CSIDFlag;

///Internal setting, See DO-282B as described below
//CSID Logic Configuration Item - DO282B 2.2.4.5.4.15
enum CSIDLogicConfig
{
  CSIDENABLED = 0,  //if enabled, CSIDFlag is allowed to alternate between Callsign and Flight plan ID
  CSIDDISABLED = 1 //If disabled, CSIDFlag will always be encoded as Call sign and CSID field will be encoded as 1
};


enum EGeometricVerticalAccuracy
{
  GVA_UNKNOWNORGT150 = 0, // in meters
  GVA_LT150 = 1,
  GVA_LT45 = 2,
  GVA_RESERVED = 3
};

typedef EnumClass<EGeometricVerticalAccuracy> GeometricVerticalAccuracy;

enum ESingleAntenna
{
  SAF_NDIVERSITY = 0, //Transmitting subsystem that only use only one antenna shall set the Single Antenna subfield to 1
  SAF_DIVERSITY = 1 //Transmitting subsystem that only use two antenna shall set Single Antenna subfield to 0 at all times both antenna channels are functional
};
 
typedef EnumClass<ESingleAntenna> SingleAntenna;

enum ENICSupplement
{
  NICFLAG_UNAVAILABLE = 0,
  NICFLAG_AVAILABLE = 1
};

typedef EnumClass<ENICSupplement> NICSupplement;


enum ESelectedAltitudeType
{
  SAT_MCP = 0, //Data being used to encode Bit 2 of Payload Byte 30, through bit 4 of Byte 34 is derived from Mode Control Panel/Flight Control Unit
  SAT_FMS = 1 //Data being used to encode Bit 2 of Payload Byte 30, through bit 4 of Byte 34 is derived from Flight Management System
};

typedef EnumClass<ESelectedAltitudeType> SelectedAltitudeType;

//MCP/FCU sources 
///2.2.4.5.6.5
enum EMCP_FCUStatus
{
  STATUS_OFF=0,
  STATUS_ON = 1
};

typedef EnumClass<EMCP_FCUStatus> MCP_FCUStatus;
///2.2.4.5.6.6
enum EModeAutoPilotEngaged
{
  AP_OFF = 0,
  AP_ON = 1
};

typedef EnumClass<EModeAutoPilotEngaged> ModeAutoPilotEngaged;


///2.2.4.5.6.7
enum EModeVNAV
{
  VNAV_OFF = 0,
  VNAV_ON = 1
};
typedef EnumClass<EModeVNAV> ModeVNAV;

///2.2.4.5.6.8
enum EModeAltitudeHold
{
  ALTHOLD_OFF = 0,
  ALTHOLD_ON = 1
};
typedef EnumClass<EModeAltitudeHold> ModeAltitudeHold;

///2.2.4.5.6.9
enum EModeApproach
{
  APPROACH_OFF = 0,
  APPROACH_ON = 1
};
typedef EnumClass<EModeApproach> ModeApproach;

///2.2.4.5.6.10
enum EModeLNAV
{
  LNAV_OFF = 0,
  LNAV_ON = 1
};

typedef EnumClass<EModeLNAV> ModeLNAV;
enum ESystemDesignAssurance
{
  SDA_UNKNOWN = 0,
  SDA_MINOR = 1,
  SDA_MAJOR = 2,
  SDA_HAZARDOUS = 3
};
typedef EnumClass<ESystemDesignAssurance> SystemDesignAssurance;

enum ESIL
{
    SIL_UNKNOWN=0,
    SIL_TEN_3=1, //1x10^-3
    SIL_TEN_5=2, //1x10^-5
    SIL_TEN_7=3  //1*10^-7
};
typedef EnumClass<ESIL> SIL;

enum ESILSupplement
{
  SILFLAG_PER_HOUR = 0,
  SILFLAG_PER_SAMPLE = 1
};
typedef EnumClass<ESILSupplement> SILSupplement;


enum ETCASOperational
{
    TCAS_OFF=0,
    TCAS_UNKNOWN=0,
    TCAS_OPERATIONAL=1
};
typedef EnumClass<ETCASOperational> TCASOperational;

enum ETCASAdvise
{
    TCAS_ADVISE_OFF=0,
    TCAS_ADVISE_ACTIVE=1
};
typedef EnumClass<ETCASAdvise> TCASAdvise;

enum EIdent
{
    IDENT_OFF=0,
    IDENT_ON=1
};
typedef EnumClass<EIdent> Ident;

enum ECallsignID
{
    CSID_FLIGHTPLAN=0,
    CSID_CALLSIGN=1
};
typedef EnumClass<ECallsignID> CallsignID;

enum ERXATCServices
{
    RXATC_FALSE=0,
    RXATC_TRUE=1
};
typedef EnumClass<ERXATCServices> RXATCServices;

enum EGeoVertAccuracy
{
    GEO_VERT_ACCURACY_UNKNOWN=0,
    GEO_VERT_ACCURACY_LE_150METERS=1,
    GEO_VERT_ACCURACY_LE_45METERS=2,
    GEO_VERT_ACCURACY_RESERVED=3
};
typedef EnumClass<EGeoVertAccuracy> GeoVertAccuracy;

enum ESINGLE_ANTENNA
{
    ANTENNA_DIVERSE=0,
    ANTENNA_SINGLE=1
};

typedef EnumClass<ESINGLE_ANTENNA> SINGLE_ANTENNA;

struct GroundUplinkPayload
{


};



#endif




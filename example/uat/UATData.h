/// \file UATData.h
/// \brief  <Description>

#ifndef __UATData__h
#define __UATData__h


#include <time.h>
#include <math.h>
#include "Mutex.h"
#include "ADSB_Fields.h"
#include "entity.h"
#include "composite.h"

enum UAT_DATA_SOURCE
{
    SOURCE_SQLITE, //SQLITE Table initial values
    SOURCE_SIMUL,  //Simulator
    SOURCE_TEST,   //Standard test values
    SOURCE_END
};

struct UATData: public Entity
{
    Mutex mMutex;                                   ///0
    PayloadType mPayloadType;                       ///1 2.2.4.5.1
    //UAT ADSB Data fields 
    //Header (with payload type)
    Address mAddress;                               ///2 1-2 2.2.4.5.1.3.1-2 Includes address qualifier


    //============================================================
    //State Vector
    Latitude mLatitude;                             ///3 2.2.4.5.2.1
    Longitude mLongitude;                           ///4 2.2.4.5.2.1
    Altitude mAltitude;                             ///5
    AltitudeType mAltitudeType;                     ///6 2.2.4.5.2.2
    //as it affects the format of the data
    AirGroundState mAirGroundState;                 ///7 
    HorizontalVelocity mHorizontalVelocity;         ///8,Airborne/on ground, 
                                                    ///and supersonic parameters,
                                                    /// track angle/heading.
                                                    /// 2.2.4.5.2.5, 2.2.4.5.2.6.1-4
  

    NIC mNIC;                                       ///9 2.2.4.5.2.4
    AVSize mAVSize;                                 ///10 2.2.4.5.2.7.2 
    //Veritical Velocity if Airborne, AVSize if On Ground
    VerticalVelocity mVerticalVelocity;             ///11  2.2.4.5.2.7.1.1-3
    UTCCoupling mUTCCoupling;                       ///12 2.2.4.5.2.8
    UplinkFeedback mUplinkFeedback;                 ///13 2.2.4.5.2.9  Use 0 for now 

    //End state vector
    //============================================================
    //Mode Status
    //Emmiter category and callsign encoded in same field
    Callsign mCallsign;                             ///14  2.2.4.5.4.2
    EmergencyStatus mEmergencyStatus;               ///15  2.2.4.5.4.4
    MOPSVersion mMOPSVersion;                       ///16
    SIL mSIL;                                       ///17 2.2.4.5.4.6
    TXMSO mTXMSO;                                   ///18 MSO ????
    SystemDesignAssurance mSDA;                     ///19 2.2.4.5.4.8 
    NACP mNACP;                                     ///20 2.2.4.5.4.10
    NACV mNACV;                                     ///21 2.2.4.5.4.9 
    NICBaro mNICBaro;                               ///22 2.2.4.5.4.11

    //Join/related
    CapabilityCodes mCapabilityCodes;               ///23 2.2.4.5.4.12
    OperationalModes mOperationalModes;             ///24 2.2.4.5.4.13
    //----------
    
    CallsignID mCallsignID;                        ///25 2.2.4.5.4.14
    SILSupplement mSILSupplement;                  ///26 2.2.4.5.4.16
    GeoVertAccuracy mGeometricVerticalAccuracy;    ///27 2.2.4.5.4.17
    SingleAntenna mSingleAntenna;                  ///28 2.2.4.5.4.18 
    NICSupplement mNICSupplement;                  ///29 2.2.4.5.4.19
    //End Mode satus


    TCASOperational mTCASOperational;               ///30 2.2.4.5.4.12.3
    SelectedAltitudeType mSelectedAltitudeType;     ///31 2.2.4.5.6.1
    SelectedAltitude mSelectedAltitude;             ///32 2.2.4.5.6.2
    BaroSetting mBaroSetting;                       ///33 2.2.4.5.6.3
    SelectedHeading mSelectedHeading;               ///34 2.2.4.5.6.4
    MCP_FCUStatus mMCP_FCUStatus;                   ///35 2.2.4.5.6.5
    

    ModeAutoPilotEngaged mModeAutoPilotEngaged;     ///36 2.2.4.5.6.6
    ModeVNAV mModeVNAV;                             ///37 2.2.4.5.6.7
    ModeAltitudeHold mModeAltitudeHold;             ///38 2.2.4.5.6.8
    ModeApproach mModeApproach;                     ///39 2.2.4.5.6.9
    ModeLNAV mModeLNAV;                             ///40 2.2.4.5.6.7

    //unsigned int mRadioHeight;                    ///xxInput only 2.2.4.5.2.5.1
    //bool mPressureAltitudeDisable;                ///xxInput only 2.2.4.5.2.5.1
    //float mAirspeed;                              ///xxInput only 2.2.4.5.2.5.1
// Duplicate    Callsign mFlightPlan;                           ///49 2.2.4.5.4.2 

    //Used  for sequencing TX messages

    EquipageClass mEquipageClass;                   ///41 2.1.1 Table 2-1 Equipage class
   
    Altitude mSecondaryAltitude;                    ///42 2.2.4.5.5.1 

    void loadTestData(void);



    public:

    UATData():
    Entity("tel.db", "uat")
    {

    }
    UATData(const UATData& uatData):
    Entity(uatData)
    {

    }

    void pushData()
    {
        set("Address", mAddress.get());
        set("Latitude", mLatitude.getStandard());
        set("Altitude", mAltitude.getStandard());
        set("OperationalModes", mOperationalModes.get());
        set("AirGroundState", mAirGroundState.get());
        set("HorizontalVelocity", sp_columns(new Composite(
            {
                PlatUnit("Angle", 1.0/*mHorizontalVelocity.getVelocity().pv.angle*/),
                PlatUnit("Velocity", 20/*mHorizontalVelocity.getVelocity().pv.groundspeed*/)
            })));//Composite type 
        set("CapabilityCodes", mCapabilityCodes.get());
    }

    void reset(Columns cols)
    {
        mAddress.set(cols.getUInt("Address"));
        mLatitude.setStandard(cols.getDouble("Latitude"));
        mAltitude.setStandard(cols.getInt("Altitude"));
        mOperationalModes.set(cols.getInt("OperationalModes"));
        mAirGroundState.set(static_cast<EAirGroundState>(cols.getInt("AirGroundState")));
        double angle = cols.getColumns("HorizontalVelocity")->getDouble("Angle"); 
        double velocity = cols.getColumns("HorizontalVelocity")->getInt("Velocity"); 
        mCapabilityCodes.set(cols.getUInt("CapabilityCodes"));
    }

    schema getSchema() const
    {
        return
        {
            {"Address", PLAT_INT},              //unsigned int need to be handled
            {"Latitude", PLAT_DBL},             //Need to convert float to double
            {"Altitude", PLAT_INT},             //int
            {"OperationalModes", PLAT_INT},      //uint_8
            {"AirGroundState", PLAT_INT},       //TODO: Create separate type for Enums
            {"HorizontalVelocity", PLAT_CMPST},   //complex types need to be handled//Composite type - do later
            {"CapabilityCodes", PLAT_BOOL}       //unsigned char
        };
    }

    void loadData(UAT_DATA_SOURCE source)
    {

        switch (source)
        {
            case SOURCE_TEST:
                loadTestData();
                break;
        }
    }

    void lock(void)
    {
        mMutex.lock();
    }

    void unlock(void)
    {
        mMutex.unlock();
    }

//    UATData(void)
//    {
//
//    }

//    ~UATData(void)
//    {
//    }
};

#endif


#include "UATData.h"

    void UATData::onSetData()
    {
        set("Address", mAddress.get());
        set("Latitude", mLatitude.getStandard());
        set("Altitude", mAltitude.getStandard());
        set("OperationalModes", mOperationalModes.get());
        set("AirGroundState", mAirGroundState.get());
        set("HorizontalVelocity_Angle", 2.0/*mHorizontalVelocity.getVelocity().pv.angle*/);
        set("HorizontalVelocity_Velocity", 530/*mHorizontalVelocity.getVelocity().pv.groundspeed*/);
        set("CapabilityCodes", mCapabilityCodes.get());
    }

    void UATData::onGetData(Columns& cols)
    {
        mAddress.set(cols.getUInt("Address"));
        mLatitude.setStandard(cols.getDouble("Latitude"));
        mAltitude.setStandard(cols.getInt("Altitude"));
        mOperationalModes.set(cols.getInt("OperationalModes"));
        mAirGroundState.set(static_cast<EAirGroundState>(cols.getInt("AirGroundState")));
        double angle = cols.getDouble("HorizontalVelocity_Angle");
        int velocity = cols.getInt("HorizontalVelocity_Velocity");
        mCapabilityCodes.set(cols.getUInt("CapabilityCodes"));
    }

    schema UATData::getSchema() const
    {
        return
        {
            {"Address", PLAT_INT},              //unsigned int need to be handled
            {"Latitude", PLAT_DBL},             //Need to convert float to double
            {"Altitude", PLAT_INT},             //int
            {"OperationalModes", PLAT_INT},      //uint_8
            {"AirGroundState", PLAT_INT},       //TODO: Create separate type for Enums
            {"HorizontalVelocity_Angle", PLAT_DBL},   //complex types need to be handled//ObjectType type - do later
            {"HorizontalVelocity_Velocity", PLAT_INT},   //complex types need to be handled//ObjectType type - do later
            {"CapabilityCodes", PLAT_BOOL}       //unsigned char
        };
    }

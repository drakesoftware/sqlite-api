/// \file HorizontalVelocity.cpp
/// \brief  Encode/decode horizontal velocity in either format



///Note: Regression tests are in a separate file:HorizontalVelocityTests.cpp


#include "HorizontalVelocity.h"


//******************************************************************************
    //Generic encode 11 bit velocity field (1 sign bit, 10 magnitude bits)
    ///\param[in]  bSign        Sign bit (North or East
    ///\param[in]  velocity     Normalized velocity (1-1021) (*4 done by caller)
    ///\param[out]  value
    ///\returns True if value =0 or out of range
uint16_t HorizontalVelocity::encodeVelocity(bool bSign, unsigned int velocity, bool bSupersonic)
{
    uint16_t value  = velocity + 1;
    if (bSupersonic)
    {
    value >>= SUPERSONIC_SHIFT; //Adjust resolution

    }

    //Invert sign, bNorth and bEast encode to 0 (positive)
    if (!bSign)
    {
       value |= VELOCITY_SIGN_MASK;
    }

    return value;
}  

//***************************************************************
///\param bNorth        Sign bit, true = northbound velocity, false = south
///\param north_speed   North (or south velocity in knots
///\param bEast         kSign bit, true = Eastbound velocity, false = east
///\param east_speed    east velocity in knots
///\param ag_state      Air ground state
unsigned int HorizontalVelocity::encode()
{
    unsigned int encoded_value=VELOCITY_UNAVAILABLE;
    unsigned int v1, v2;


    //Modifying arguments rather use add intermediate variable
    if (!isOnGround())
    {
        unsigned int north_speed=mVelocity.xv.NorthVelocity;
        unsigned int east_speed=mVelocity.xv.EastVelocity;


        //All values can be encoded
        v1=encodeVelocity(mVelocity.xv.bNorth, north_speed,isSupersonic());
        v2=encodeVelocity(mVelocity.xv.bEast, east_speed,isSupersonic());
        
        encoded_value=populateFields(v1,v2);

    }
    else //Groundspeed
    {

        encodeAngle(mVelocity.pv.angle, mVelocity.pv.angletype, v1);
        encodeVelocity(false, mVelocity.pv.groundspeed, v2 );
        
        encoded_value=populateFields(v1,v2);
 

    }
    return encoded_value;
}

//******************************************************************************
/// Set Velocity from State vector (bit 4 of byte 13 through bit 1 of byte 16)
/// \param value    22 bit value with N/S-E/W or heading/groundspeed data
/// \param ag_state aircraft encoding 
bool HorizontalVelocity::set(unsigned int value,AirGroundState ag_state)
{
    if (ag_state.get() == AG_RESERVED) 
    {
        setExceptionState(STATE_UNAVAILABLE);
        return true;
    }
    mAirGroundState=ag_state;
    if (!isOnGround())
    {
        decodeVelocity(value >> VELOCITY_FIELD_SIZE,
                           mVelocity.xv.bNorth,
                           mVelocity.xv.NorthVelocity);
        decodeVelocity(value,
                       mVelocity.xv.bEast,
                       mVelocity.xv.EastVelocity);

        if (AG_SUPERSONIC & mAirGroundState.get())
        {
            mVelocity.xv.NorthVelocity <<= SUPERSONIC_SHIFT;
            mVelocity.xv.EastVelocity <<= SUPERSONIC_SHIFT;
        }
    }
    else //Groundspeed
    {
        bool dummy;  //Sign bit unused
        decodeAngle(value>>VELOCITY_FIELD_SIZE, mVelocity.pv.angle, mVelocity.pv.angletype);
        decodeVelocity(value, dummy,  mVelocity.pv.groundspeed);

    }
    setValid();
    setExceptionState(STATE_VALID);
    return false;
}

//******************************************************************************
bool  HorizontalVelocity::set(bool bNorth, 
                              unsigned int v1, 
                              bool bEast, 
                              unsigned int v2,
                              AirGroundState ag)
{
    if (!isOnGround())  //XY form
    {
        if (v1 > getMaxVelocity())
        {
            //TODO: Verify this encodes to all ones per DO-282 2.2.4.5.2.6.1
            v1=VELOCITY_MAX_VALUE +1; 
        }

        if (v2 > getMaxVelocity())
        {
            v2=VELOCITY_MAX_VALUE +1; 
        }
        
        mVelocity.xv.bNorth=bNorth;
        mVelocity.xv.bEast=bEast;
        mVelocity.xv.NorthVelocity=v1;
        mVelocity.xv.EastVelocity=v2;
        mAirGroundState=ag;
        setValid();
        return false;
    }
    setValid(false);
    setExceptionState(STATE_UNAVAILABLE);
    return true;
}



//******************************************************************************
bool  HorizontalVelocity::set(float angle, 
                             EAngleType type,  
                             unsigned int velocity, 
                             AirGroundState ag)

{
   
    mAirGroundState=ag;
    if (isOnGround())  //XY form
    {
        mVelocity.pv.angletype=type;
        mVelocity.pv.angle=angle;
        mVelocity.pv.groundspeed=velocity;
        setExceptionState(STATE_VALID);
        setValid();
        return false;
    }
    //State not changed TODO:Check
    return true;
}


void  HorizontalVelocity::decodeVelocity(unsigned int value, bool& bSign, unsigned int& velocity)
{
    bSign=false;

    if (value & VELOCITY_SIGN_MASK )
    {
        bSign=true;
    }
    velocity = (value  & VELOCITY_ABS_MASK) - 1 ;
}

//******************************************************************************
///Generic decode 11 bit  field (2 types bits, 9 2 types bits, 9  magnitude bits)
///\param[in]  value[in]    11 bit velocity value (bit 10 is sign bit),
void  HorizontalVelocity::decodeAngle(unsigned int value, float& angle, EAngleType& angtype )
{
    
    unsigned int anglevalue=value & TA_HEADING_MASK;

    angle=anglevalue * TA_DEGREES_BIT;

    angtype=(EAngleType) ((value >>TA_HEADING_SHIFT) & TA_HEADING_TYPE_MASK);
}


///Generic decode 11 bit  field (2 types bits, 9 2 types bits, 9  magnitude bits)
///\param[in]  value[in]    11 bit velocity value (bit 10 is sign bit),
void HorizontalVelocity::encodeAngle(float angle, EAngleType type, unsigned int& value)
{

    if (angle >= MAX_HEADING) //Bring out of range values into range
    {
        angle/=MAX_HEADING;
    }
    value=type << TA_HEADING_SHIFT;
    value |= (unsigned int) ((angle + .5)/TA_DEGREES_BIT);
}


bool HorizontalVelocity::Test(bool Verbose)
{

}



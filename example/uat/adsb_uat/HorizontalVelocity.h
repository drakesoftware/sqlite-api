/// \file HorizontalVelocity.h
/// \brief Encode/decode horizontal velocity in either format
#ifndef __velocity_h
#define __velocity_h

#include <stdio.h>
#include <limits.h>

#include "utils.h"
#include "ExceptionState.h"

#include "ADSB_Types.h"

#define TA_DEGREES_BIT 0.703125f

#define  VELOCITY_MAX_VALUE  1021.5f 
#define  VELOCITY_MAX_VALUE_SUPERSONIC  4084.0f



///Maximum heading/track angle
#define MAX_HEADING 360.0f
struct XYVelocity
{
    bool bNorth;
    unsigned int NorthVelocity;
    bool bEast;
    unsigned int EastVelocity;
};

struct PolarVelocity
{
    EAngleType angletype;
    float angle;               //0-511  .703125 degrees/unit
    unsigned int groundspeed;           //Knots 0 - 1023
};

union Velocity
{
    PolarVelocity pv;
    XYVelocity    xv;
};


///For DO-282B 2.2.4.5.2.(6-7)
/// Save horizontal velocity
/// Set and get functions save and retrieve value in desired form (encoded or standard)
/// Standard is engineering units (knots). 
///Does NOT convert between these forms, but stores and provides the velocity in the proper
//format based on the A/G state
class HorizontalVelocity:public ExceptionState,public Validity
{

    AirGroundState mAirGroundState;
    Velocity mVelocity;             ///Velocity vector, polar or XY, 


    enum EXCEPTIONSTATE {STATE_VALID,STATE_UNAVAILABLE};
    static const unsigned int VELOCITY_UNAVAILABLE = 0x0;      //Velocity unavailable

    //Number of bits to shift(multiply) for supersonic AG state
    static const int SUPERSONIC_SHIFT           = 2;  

    //No of bits in velocity field of State Vector
    static const int VELOCITY_VECTOR_SIZE       = 22; 

    //Shift to move North/South velocity
    static const int VELOCITY_FIELD_SIZE   = 11;

    //Size of velocity field w/o sign (North/South) bit
    static const int VELOCITY_ABS_FIELD_SIZE   = 10;
  
    static const int VELOCITY_SIGN_BIT_SHIFT  = 10;    
    static const int VELOCITY_MAX_ENCODED_VALUE= (2 << VELOCITY_ABS_FIELD_SIZE) - 1;
    static const int VELOCITY_ENCODED_VALUE_MAX=0x3ff;     //>1021/4084

    //Mask for  value portion (10 bits) of velocity field
    CONST_UNSIGNED VELOCITY_ABS_MASK = BITMASK_LSBS(VELOCITY_ABS_FIELD_SIZE); 

    //Mask for entire field (sign + value) of velocity field
    //11 bit velocity including sign bit.
    CONST_UNSIGNED VELOCITY_FIELD_MASK = BITMASK_LSBS(VELOCITY_FIELD_SIZE); 

    //Sign bit (North/South or East/West)
    CONST_UNSIGNED VELOCITY_SIGN_MASK  = 1 << VELOCITY_SIGN_BIT_SHIFT;
    
    static const int TA_HEADING_SHIFT  = 9;       //Heading 9 bits, 2 bits TA/H type
    static const unsigned int TA_HEADING_MASK = BITMASK_LSBS(9);  //2 bits
    static const unsigned int TA_HEADING_TYPE_MASK = BITMASK_LSBS(2);  //2 bits

    //Not allowed by compler, see #define above
    //static const float TA_DEGREES_BIT = 0.703125f;





    //private utility/conversion functions
    //Encode  a dimension of velocity. Decode/encode can never fail, all
    //values map to valid results
    void decodeVelocity(unsigned int value, bool& bSign, unsigned int& vel);
    uint16_t encodeVelocity(bool bSign, unsigned int velocity, bool bSupersonic=false);
    //Encode complete velocity
    void  decodeAngle(unsigned int value, float& angle, EAngleType& angtype);
    void  encodeAngle(float angle, EAngleType type, unsigned int& value);

    ///Encode velocity and return appropriate format based on AirGround state (mAirGroundState); 
    unsigned int encode(void);

    public:
    ///\return True if Onground state is true
    bool isOnGround(void)
    {
        return (mAirGroundState.get() ==  AG_ONGROUND);
    }

    bool isSupersonic(void)
    {
        return (mAirGroundState.get() ==  AG_SUPERSONIC);

    }


    float getMaxVelocity(void)
    {
        if (isSupersonic())
        {
            return VELOCITY_MAX_VALUE_SUPERSONIC;
        }
        else
        {
            return VELOCITY_MAX_VALUE;
        }
    }


    /// Store velocity.
    ///\param ag   AirGround state, must be AG_SUBSONIC or AG_SUPERSONIC
    /// TODO:  Check airground state for necessary range of values
    bool  set(bool bNorth, unsigned int v1, bool bEast, unsigned int v2,AirGroundState ag);
  
    ///Store velocity in groundspeed (polar) form
    ///\param angle   Heading/track
    ///\param AngleType  Type of angle (track, heading....)
    ///\param velocity  velocity (knots)
    ///\param ag    AirGround state, must be AG_ONGROUND  //TODO:  check for proper value
    bool set(float angle, EAngleType type,  unsigned int velocity, AirGroundState ag);


    ///Set with encoded value from UUT
    ///\param value  22 bit value from state vector
    ///\param ag_state  AirGround state from corresponding field
    ///\returns True if value is undecodeable 
    bool set(unsigned int value, AirGroundState ag_state);




    ///Constructor    
    HorizontalVelocity(void) = default;

    HorizontalVelocity(AirGroundState ag, float angle, EAngleType type, unsigned int velocity):ExceptionState(STATE_VALID)
    {
        set(angle, type, velocity,ag);

    }
    HorizontalVelocity(AirGroundState ag, bool bNorth, unsigned int v1, bool bEast, unsigned int v2):ExceptionState(STATE_VALID)
    {
        set(bNorth, v1, bEast, v2, ag);
    }
    ///Mark value unavaiable
    int setUnavailable(void)
    {
        setExceptionState(STATE_UNAVAILABLE);
        set(VELOCITY_UNAVAILABLE,AG_SUBSONIC);
    }

    //\returns Air Ground state
    AirGroundState getAG(void)
    {
        return mAirGroundState;
    }
   
    Velocity getVelocity(void)
    {
        return mVelocity;
    }

    ///Return decoded velocity values
    Velocity getStandard(void)
    {
        return getVelocity();
    } 

    //Return encoded velocity (22 bits)
    unsigned int getEncoded(void)
    {
        return encode();
    }


    //Utility function to create 22 bit velocity field from 2 11 bit values
    unsigned int populateFields(unsigned int v1, unsigned int v2)
    {
        unsigned int return_value= (HorizontalVelocity::VELOCITY_FIELD_MASK & v1)
                                    <<HorizontalVelocity::VELOCITY_FIELD_SIZE
                                    |(HorizontalVelocity::VELOCITY_FIELD_MASK & v2);
        return return_value;
    }


    ///Regression test UAT_TEST and  UAT_ALTITUDE_TEST must be enabled in Makefile
    static bool Test(bool verbose);
  

    //Subtests 
    static bool SubsonicTestEncode(bool verbose);
    static bool SubsonicTestDecode(bool verbose);
    static bool SupersonicTestEncode(bool verbose);
    static bool SupersonicTestDecode(bool verbose);
    static bool GroundspeedTestEncode(bool verbose);
    static bool GroundspeedTestDecode(bool verbose);


};





#endif


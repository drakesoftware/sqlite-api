/// \file ExceptionState.h
/// \brief  Records and reports excpetion data state
//   I.e.  No data, and other states where a getStandardValue (or similar) function
//   will not return a defined or useful value

#ifndef __exceptionstate__h
#define __exceptionstate__h


#include <time.h>
#include <math.h>


///This class,although not virtual, is intended to be used only as base class
///for other UAT ADS-B data classes. It 
///State is an integer and represents one of multiple mutually exclusive data
///Derived classes should define 0 as data valid, 1 as NO data (or equivalent--unavailable), and other states
///to indicate the state of the data source. States must be mutually exclusive
///E.g. GPSOffset has a Sensor offset state where the sensor automatically corrects
///it's data(position) with the offset from the nose (or reference point) of the aircraft
///
///Rationale:  With multiple data fields being parsed, to indicate non-typical return values
///(i.e. No data, or Sensor applied offset) a separate convention would be needed for each state
///This simple class can be customized for each data field but sets a common interface for use 
//by all data field classes.
class ExceptionState
{
   
    int mState;
    
    public:


    ExceptionState()=default;    

    ///Used when value should be initialized as invalid and
    ///set to valid (0) when valid data is decoded or set
    ///to another state on instatiation
    ExceptionState(int state)
    {
        mState=state;
    }

    ~ExceptionState(void)
    {
    }

    void setStateValid(void)
    {
        mState=0;
    }
    void setExceptionState(int state)
    {
        mState=state;
    }

    int getExcpetionState(void)
    {
        return mState;
    }

    ///Report if a standard data value is available. Non-zero does
    ///not, necessarily, indicate an error or even unavailable. 
    ///However, if false, it does indicate that data can be read using a
    ///getStandardValue function.
    ///\returns true if a standard data is not available
    bool isException(void)
    {
        return (mState != 0);
    }
};

#endif


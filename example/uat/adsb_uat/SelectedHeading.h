// \file SelectedHeading.h
/// \brief Encode/decode ADSB SelectedHeading DO-282 2.2.4.5.6.4
#ifndef __SelectedHeading_h
#define __SelectedHeading_h

#include <stdio.h>
#include <limits.h>
#include "ADSB_Types.h"
#include "Validity.h"

class SelectedHeading: public Validity
{
    int mSelectedHeading;  //Encoded    

    unsigned int encode(PRECISION_FLOAT selectedheading, bool heading_valid);
    PRECISION_FLOAT decode(unsigned int value);


    public:
    
    static const unsigned int SELECTED_HEADING_STATUS_OFFSET_BIT=0x9;
    static const unsigned int SELECTED_HEADING_SIGN_OFFSET_BIT=0x8;
    static const unsigned int SELECTED_HEADING_SIZE_BITS=8;
   
    static const float DEGREES_PER_BIT; 

    static const unsigned int SELECTED_HEADING_ENCODED_MAX=0xff;      //Maximum encoded SelectedHeading value, 
    static const int SELECTED_HEADING_MINIMUM_ENCODEABLE= 0;           //Minimum encodeable SelectedHeading
    static const int SELECTED_HEADING_MAXIMUM_ENCODEABLE= 180;      //Maximum encodeable SelectedHeading

  
    bool isHeadingValid(void)
    {
        if (mSelectedHeading & (1 << SELECTED_HEADING_STATUS_OFFSET_BIT))
        {
            return true;
        }
        return false;
    } 
    bool setStandard(PRECISION_FLOAT sh, bool heading_valid)
    {
        if (sh >SELECTED_HEADING_MAXIMUM_ENCODEABLE)
        {
            //TODO??
        }
        else
        {
           mSelectedHeading=encode(sh,heading_valid);
        }
        setValid();
    } 
   
    bool setEncoded(unsigned int encoded_val)
    {
        mSelectedHeading=encoded_val;
        setValid();
    }

    int getEncoded(void)
    {
        return mSelectedHeading;
    }

    PRECISION_FLOAT get(void)
    {
        return decode(mSelectedHeading);
    }

    SelectedHeading()=default;

    SelectedHeading(unsigned int encoded_val)
    {
        mSelectedHeading=encoded_val;
        setValid();
    }

    SelectedHeading(PRECISION_FLOAT heading, bool heading_valid)
    {
        setStandard(heading,heading_valid);
    }
    ///Regression test UAT_TEST must be enabled in Makefile
    static bool Test(bool verbose);
    static bool TestDecode(bool verbose);
    static bool TestEncode(bool verbose);


};


#endif


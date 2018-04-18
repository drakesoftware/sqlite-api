/// \file Callsign.cpp
/// \brief  Callsign and Emitter Category encode/decode per DO-282 2.2.4.5.4.1 and 2.2.4.5.4.2
///


#ifndef __Callsign__h
#define __Callsign__h
#include <stdio.h>
#include <utils.h>
#include "Validity.h"

class Callsign: public Validity
{

    uint16_t mBase40[3]; 
public:
    enum EmitterCategory
    {
        NONE = 0,
        LIGHT = 1,
        SMALL = 2,
        LARGE = 3,
        HIGH_VORTEX_LARGE = 4,
        HEAVY = 5,
        HIGHLY_MANEUVERABLE = 6,  
        ROTORCRAFT = 7,
        UNASSIGNED1 = 8,
        GLIDER = 9,
        LIGHTER_THAN_AIR = 10,
        PARACHUTIST = 11,
        ULTRA_LIGHT = 12,
        UNASSIGNED2 = 13,
        UAV = 14,
        SPACE_VEH = 15,
        UNASSIGNED3 = 16,
        SURFACE_VEH_EM = 17,
        SURFACE_VEH_SERV = 18,
        POINT_OBS = 19,
        CLUSTER_OBS = 20,
        LINE_OBS = 21,
        RESERVED1 = 22,
        RESERVED2 = 23,
        RESERVED3 = 24,
        RESERVED4 = 25,
        RESERVED5 = 26,
        RESERVED6 = 27,
        RESERVED7 = 28,
        RESERVED8 = 29,
        RESERVED9 = 30,
        RESERVED10 = 31,
        RESERVED11 = 32,
        RESERVED12 = 33,
        RESERVED13 = 34,
        RESERVED14 = 35,
        RESERVED15 = 36,
        RESERVED16 = 37,
        RESERVED17 = 38,
        RESERVED18 = 39
    };    
    
    static const int FIELD_DECODED_SIZE_BYTES=9;
    static const int CALLSIGN_LENGTH=8;

    Callsign(char const* callsign, EmitterCategory ec)
    {
        setStandard(callsign, ec);
    }



    Callsign()=default;
    Callsign operator= (const Callsign& cs)
    {
        int i;
        for (i=0;i<3;i++)
        {
            mBase40[i]=cs.mBase40[i];
        }
        setValid();
    }
/*
    Callsign (uint16_t arr[3])
    {
        int i;
        for (i=0;i<3;i++)
        {
            mBase40[i]=arr[i];
        }
        setValid();
    }
*/
    Callsign(Callsign &cs) = default;

    Callsign(uint64_t val)
    {
        setEncoded(val);
    }



    static const int CALLSIGN_SPACE = 36;
    static const int CALLSIGN_NOT_AVAILBLE = 37;
#ifdef UAT_TEST
    ///Tests function by calling TestDecode and TestEncode functions
    static bool Test(bool verbose);

    ///Tests the decode function
    static bool TestDecode(bool verbose);

    ///Tests the encode function
    static bool TestEncode(bool verbose);
#endif
    char ECConversionDC(char ec);
    unsigned int ECConversionCD(char ec);

    uint16_t getBase40Encoded(const char * arr);
    char* getBase40Decoded(unsigned int encodedval,char* rDecodedValue);

    char* setUnavailable(char* arr)
       {  
            //if the first element of array is not available, then the rest of will be 37(not available)
            if(arr[0] == 0)
            {
                for(int i =0; i<8; i++)
                {
                    arr[i] = CALLSIGN_NOT_AVAILBLE;
                }
            }
            return arr;

        } 

    char* pad(char* arr)
    {
            if(strlen(arr) < 8)
            for(int i =strlen(arr); i<8;i++)
            {
                arr[i]= CALLSIGN_SPACE;
            }
        
        return arr;
    }

    ///Sets standard value by calling setStandard function 3 times
    ///\param callsign     Callsign  array of 9 cha
    void setStandard(char const* callsign, EmitterCategory ec)
    {
        char arr[FIELD_DECODED_SIZE_BYTES];
        

        //This conversion is from decimal to character
        arr[0]=ECConversionDC(ec);
        int i=1;
        int callsignlength=strnlen(callsign,CALLSIGN_LENGTH);
        for (int j=0; j<CALLSIGN_LENGTH; j++)
        {
            if (j<callsignlength)
            {
                arr[i++]=callsign[j];
            }
            else
            {
                arr[i++]=' ';
            }
        }

        //base40.setStandard expects 9 character array
        //
        //
        for (int i = 0; i <3;i++)
        {
            mBase40[i]=getBase40Encoded(arr+(i*3));
        }
        setValid();
    }

    ///Sets the encoded value 3 times
    ///\param encoded_value      array of 3 elements to be passed through setEncoded from class base40
    void setEncoded(uint64_t encoded_value)
    {

        for(int i=0;i<3;i++)
        {
            mBase40[i]=(encoded_value>>(16*(2-i))) &0xffff;
        }
    }
    
    ///Calls base40 getEncoded 3 times then returns the mEncodedValue in arrEncodedValue array
    ///\param arrEncodedValue 
    uint64_t getEncoded(void)
    {
      uint64_t val = 0;
        for(int i=0; i<3 ; i++)
        {
            val <<= 16;
            val |= mBase40[i];
        }
        return val;
    }


    ///Calls base40 getStandard 3 times then returns the standard value and returns in array
    ///\param[out] arrStandardValue   arrStandardValue 
    ///\param[out] ec  Emitter category
    char* getStandard(char* arrStandardValue, unsigned char  &ec) 
    {
        char decodestr[FIELD_DECODED_SIZE_BYTES+1];
        for(int i=0; i<3; i++)
        {
            getBase40Decoded(mBase40[i], decodestr+(i*3));   
        }

        //This conversion is from character to decimal
        ec=ECConversionCD(decodestr[0]);

        strncpy(arrStandardValue,decodestr+1,FIELD_DECODED_SIZE_BYTES); //9 bytes, 8 bytes + terminator
        return arrStandardValue;

    }

    std::string toString(void)
    {
        static const int STRSIZE=30;
        char str[STRSIZE];
        char callsign[FIELD_DECODED_SIZE_BYTES];
        uint8_t ec;
        getStandard(CHARPTR(callsign),ec);
        snprintf(str,STRSIZE,"CS: %s, EC:%d",callsign,ec);
    
        return std::string(str);
    }
};


#endif

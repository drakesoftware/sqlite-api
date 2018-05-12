/// \file Callsign.cpp
/// \brief  Callsign and Emitter Category encode/decode per DO-282 2.2.4.5.4.1 and 2.2.4.5.4.2
///

#include "Callsign.h"
#include "RegressionTests.h"

struct charmap
{
  unsigned int base40digit;
  unsigned char character_value;
};
 static const struct charmap charmaptable[]=
{
    {0,     '0'},
    {1,     '1'},
    {2,     '2'},
    {3,     '3'},
    {4,     '4'}, 
    {5,     '5'},
    {6,     '6'},
    {7,     '7'},
    {8,     '8'},
    {9,     '9'},
    {10,    'A'},
    {11,    'B'},
    {12,    'C'},
    {13,    'D'},
    {14,    'E'},
    {15,    'F'},
    {16,    'G'},
    {17,    'H'},
    {18,    'I'}, 
    {19,    'J'},
    {20,    'K'},
    {21,    'L'},
    {22,    'M'},
    {23,    'N'},
    {24,    'O'},
    {25,    'P'},
    {26,    'Q'},
    {27,    'R'},
    {28,    'S'},
    {29,    'T'},
    {30,    'U'},
    {31,    'V'},
    {32,    'W'},
    {33,    'X'},
    {34,    'Y'},
    {35,    'Z'},
    {36,    ' '}, //space, if call sign is less than 8 characters, it will be padded with spaces
    {37,    '!'},  //if flight plan ID input is not available, all 8 characters of the field will be set to code 37
    {38,    'r'}, 
    {39,    'r'}
};

#ifdef UAT_TEST
struct base40_test
{
  unsigned int encoded_value[3];
  char const* standard_value;
  Callsign::EmitterCategory EC;
};

///TODO-Change encoded_value to hex
base40_test base40_testValues[]
{ //(encoded_value, standard value)
    {{22897,24573,29496}, "CEDEFGHI", Callsign::SURFACE_VEH_EM}, //H
    {{46814,19650,22897}, "ATABCHCE", Callsign::UAV}, //E
    {{40970,29496,24573}, "OPGHIDEF", Callsign::LIGHTER_THAN_AIR}, //A
    {{29509,45548,45389}, "HISISTES", Callsign::RESERVED8}, //T
    {{16642,25896,23050}, "GAG7GAGE", Callsign::SMALL}, //2
 //   {{0,0,0}, " ", Callsign::NONE},
 //   {{12345, 12312, 16721}, "HIMOMS", Callsign::SURFACE_VEH_SERV}
};

static TestEntry CStestlist[]=
{
    {Callsign::TestDecode,"Callsign Decode Test"},
    {Callsign::TestEncode,"Callsign Encode Test"},
    {NULL,NULL,}
};
#endif

//Convert from a char value to unsigned char
//Needed to get integer Emittercategory
char Callsign::ECConversionDC(char ec)
{
    unsigned char retval;
    for(int i= 0; i < 39; i++)
    {
          if(ec == charmaptable[i].base40digit)
        {
            retval = charmaptable[i].character_value;
            break;
        }
    }
    return retval;
}


//TODO: Wrong?
unsigned int Callsign::ECConversionCD(char ec)
{
    for(int i= 0; i < 39; i++)
    {
      if(ec == charmaptable[i].character_value)
      {
        ec = charmaptable[i].base40digit;
        break;
      }
    }
    return ec;
}



//****************************************************************************
//setStandard will take a array of 3 chars and do the base 40 encoding then return back an unsigned int
uint16_t Callsign::getBase40Encoded(const char * arr)
{
    unsigned int e[3];
    uint16_t encoded_val;
    for(int i=0; i<3; i++)
    {
      for(int j = 0; j < 39; j++)
      {
        if (arr[i] == charmaptable[j].character_value)
        {
          e[i] = charmaptable[j].base40digit;
          break;
        }
      }
    }

    //calculating the encoded value
    encoded_val = e[0]*1600+e[1]*40+e[2];
    return encoded_val;
}

//****************************************************************************
//Decode will take the unsigned int and an empty array and return back the array of 3 chars
char * Callsign::getBase40Decoded( unsigned int encodedval, char* rDecodedValue)
{
  unsigned int decoded_int[3];
  //decoding the base 40
    decoded_int[0]=(encodedval/1600) % 40;

    decoded_int[1]=(encodedval/40) % 40;

    decoded_int[2] = encodedval % 40;

for(int i = 0; i < 3; i++)
{
  for(int j = 0; j < 39; j++)
  {
    if(decoded_int[i]==charmaptable[j].base40digit)
      {
        rDecodedValue[i] = charmaptable[j].character_value;
        break;
      }
  }
}

  //3 character array
  return rDecodedValue;
}

//need to change from int encoded value to array 
#ifdef UAT_TEST

bool Callsign::TestEncode(bool verbose)
{

    bool pass=true;
    Callsign cs;


    for(int i=0;i<sizeof(base40_testValues)/sizeof(base40_test);i++)
    {

          cs.setStandard(base40_testValues[i].standard_value, base40_testValues[i].EC);

          unsigned int test_EncodedValues[3];

          cs.getEncoded(test_EncodedValues);

        //needs a loop for test_EncodedValues[j]
          for(int j=0;j<3; j++)
            {
              if (test_EncodedValues[j] != base40_testValues[i].encoded_value[j])
              {
                  printf("Test #: %d Emergency Status, Results are: %d, Should be %d \n",j+1, test_EncodedValues[j], base40_testValues[i].encoded_value[j]);

                  pass=false;
              }
            }



    return !pass;

  } 
}
bool Callsign::TestDecode(bool verbose)
{
      bool pass=true;
      unsigned int EC;
      Callsign cs;

    for(int i=0;i<sizeof(base40_testValues)/sizeof(base40_test);i++)
    {


        
        cs.setEncoded(base40_testValues[i].encoded_value);

        //stores the completed standard value
        char test_StandardValues[9];
        //stores the emitter category character

        cs.getStandard(test_StandardValues, EC);

      //setunavailable
        if(test_StandardValues[0] == '0')
        {
            cs.setUnavailable(test_StandardValues);
        }

      //checksize
        if(strlen(test_StandardValues)!=9)
        {
          cs.pad(test_StandardValues);
        }
        //returns the emitter category code for the tests


        //cs.getEmitterChar(test_StandardValues, emitterChar);

        for(int j=0;j<8;j++)
        {
          if (test_StandardValues[j+1] != base40_testValues[i].standard_value[j])
          {
              printf("Test #: %d \n Base 40 Decode fail, Results are: %c, Should be: %c \n",j+1, test_StandardValues[j], base40_testValues[i].standard_value[j]);

              pass=false;
          }  
        }
        if(EC != base40_testValues[i].EC)
        {
              printf("Test #: %d \n Base 40 Emitter Category fail, Results are: %d, Should be: %d \n",i+1, EC, base40_testValues[i].EC);
              pass=false;
          }

       
    }
 
    return !pass;
}
bool Callsign::Test(bool verbose)
{
    return RunTestList(CStestlist,verbose);

}
#endif

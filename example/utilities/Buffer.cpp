/// \file Buffer.cpp /// \brief  Class to insert and extract fields from a byte oriented buffer

#include <string.h>
#include <Buffer.h>

#include "RegressionTests.h"
BYTE  Buffer::readByte(int offset, int length)
{    
    int byteoffset = offset/BITS_PER_BYTE;
    int bitoffset = offset%BITS_PER_BYTE;
    BYTE returnvalue;
    if (length > BITS_PER_BYTE)
    {
        length = BITS_PER_BYTE;
    }
     
    //bits in high byte
    int high_bits=BITS_PER_BYTE-bitoffset;
    
    //Bits in low byte
    int low_bits=length-high_bits; 

    //Shift value right to get high byte val
    int high_shift=low_bits; 

    //Shift value left to get low byte
    int low_shift=BITS_PER_BYTE-low_bits;  

    //Mask to preserve original bits in high word
    BYTE maskh=BITMASK_LSBS(high_bits); //Mask after shift
    BYTE maskl=BITMASK_LSBS(low_bits);  // "


    if ((length + bitoffset)<BITS_PER_BYTE)  //Value in one byte
    {
        high_bits=length;
        high_shift=BITS_PER_BYTE-(bitoffset+length);
        returnvalue=(mBuffer[byteoffset]>>high_shift) & BITMASK_LSBS(length);
        
    }
    else //value spans two bytes
    {
        //Shift high bits over to make room
        returnvalue=((mBuffer[byteoffset]&maskh) << high_shift) ; 
        //For low bits (shift down
        returnvalue|=mBuffer[byteoffset+1] >> low_shift & maskl;
    }
    return returnvalue;
}
 
uint64_t Buffer::read(int offset, int length)
{
    uint64_t v = 0;
    int byteoffset = offset/BITS_PER_BYTE;
    int bitoffset = offset%BITS_PER_BYTE;

    int bytelength = length/BITS_PER_BYTE;
    int bitremainder=length%BITS_PER_BYTE;

    int i;
    int shift=length-8; 
    for (i=0;i<bytelength;i++)
    {
        v<<=8;
        v|=readByte(offset,
                    BITS_PER_BYTE);
        offset+=BITS_PER_BYTE;
    }
    if (bitremainder)
    {
        v<<=bitremainder;
        v|=readByte(offset,
                   bitremainder);
    }

    return v;
}

BYTE* Buffer::readArray(int offset, int bytelength,  BYTE* buf)
{

    int i;
    for (i=0;i<bytelength;i++)
    {
        buf[i]=readByte(offset,
                        BITS_PER_BYTE);
        offset+=BITS_PER_BYTE;
    }

    return buf;
}


void Buffer::writeByte(BYTE val, int offset, int length)
{    
    int byteoffset = offset/BITS_PER_BYTE;
    int bitoffset = offset%BITS_PER_BYTE;

    if (length > BITS_PER_BYTE)
    {
        length = BITS_PER_BYTE;
    }

    if (!bitoffset) //Byte boundary, fit within single byte
    {
        if (length == BITS_PER_BYTE)
        {
            mBuffer[byteoffset]=val;
        }
        else //< full byte
        {
            BYTE tempval;
            int shift=BITS_PER_BYTE-length;
            mBuffer[byteoffset]&=BITMASK_LSBS(BITS_PER_BYTE-length);

            tempval=(BITMASK_LSBS(length) & val)<<shift;

            mBuffer[byteoffset]|=tempval;
        
        }
    }
    else //value offset from MSB
    {
        //Value spans 2 bytes
        if ((length + bitoffset)>BITS_PER_BYTE) 
        {
            //bits in high byte
            int high_bits=BITS_PER_BYTE-(bitoffset);
            //Bits in low byte
            int low_bits=length-high_bits; 
          
            //Shift value right to get high byte val
            int high_shift=low_bits; 
            
            //Shift value left to get low byte
            int low_shift=BITS_PER_BYTE-low_bits;  

            //Mask to preserve original bits in high word
            BYTE maskh=BITMASK_BYTE_MSBS(BITS_PER_BYTE-high_bits); 

            //And low word
            BYTE maskl=BITMASK_LSBS(BITS_PER_BYTE-low_bits);
            mBuffer[byteoffset]&=maskh;
            mBuffer[byteoffset]|= val >> high_shift;

            mBuffer[byteoffset+1]&=maskl;
            mBuffer[byteoffset+1]|= (val & BITMASK_LSBS(low_bits))<<low_shift;

        }
        else //Value within 1 byte
        {
            int shift=BITS_PER_BYTE-(bitoffset+length); //Left shift to place value
            BYTE mask= BITMASK_BYTE_MSBS(bitoffset)|BITMASK_LSBS(shift);
            mBuffer[byteoffset] &=mask; 
            mBuffer[byteoffset] |= (~mask & (val << shift));
        }
    }

}


    void Buffer::write(uint64_t value, int offset, int length)
    { 
        uint64_t v = value & BITMASK_LSBS(length); 
        int byteoffset = offset/BITS_PER_BYTE;
        int bitoffset = offset%BITS_PER_BYTE;
   
        int bytelength = length/BITS_PER_BYTE;
        int bitremainder=length%BITS_PER_BYTE;
    
        int i;
        int shift;
        //for (i=bytelength;i>0;i--)
        for (i=0;i<bytelength;i++)
        {
            shift=(length-(BITS_PER_BYTE*(i+1)));
            writeByte((value >> shift) & BYTEMASK,
                      offset,
                      BITS_PER_BYTE);
            offset+=BITS_PER_BYTE;
        }
        if (bitremainder)
        {
            writeByte((value)  & BITMASK_LSBS(bitremainder),
                      offset,
                      bitremainder);
        }
    }

    ///Write array starting at
    ///\param offset    Offset into destination buffer in bits
    ///\param bytelength Number of bytes from 'values' to write                
    void Buffer::writeArray(const BYTE* values, int offset, int bytelength)
    { 
         
        int i;
        for (i=0;i<bytelength;i++)
        {
            writeByte(values[i],
                      (i*BITS_PER_BYTE)+offset,
                      BITS_PER_BYTE);
        }
    }

#define TESTBUFFERSIZE 8

struct BufferTestStruct
{
    const unsigned int value;
    const int length;
    const int offset;
    const int offset_read;
    const uint64_t ExpectedReadValue;
    const BYTE start[TESTBUFFERSIZE];
    const BYTE expected[TESTBUFFERSIZE];
};



#ifdef BUFFER_TEST
BufferTestStruct BufferTest[]=
{

    //0
    {0x3, 3, 12, 12, 0x3, 
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}},

    //1
    {0x1, 1, 4, 7, 0x1, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},

    //2
    {0x1, 2, 4, 4,0x1,
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},

    //3
    {0x0, 4, 8, 8, 0x0, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xff, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},

    //4
    {0x0, 8, 8, 4, 0xf0, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}},

    //5
    {0x0, 8, 8, 24, 0x32, 
    {0xff, 0xff, 0xff, 0x32, 0xff, 0xff, 0xff, 0xff},
    {0xff, 0x00, 0xff, 0x32, 0xff, 0xff, 0xff, 0xff}},

    //6
    {0x0, 24, 4, 16, 0x0fff, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff}},
    
    //7
    {0x0, 24, 8, 4,0xf00000, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff}},

    //8
    {0x7f7f7f7f, 24, 4, 12, 0x7f7fff, 
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xf7, 0xf7, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff}},
  
    //9
    {0x7f7f7f7f, 24, 8, 20, 0xf7ffff,
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
    {0xff, 0x7f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff}}
    

};

struct BufferArrayTestStruct 
{
    const int bytelength;     //Length of array to be written
    const int offset;       //ReadWrite bit offset
    const int offset_read;
    const BYTE start[TESTBUFFERSIZE];
    const BYTE writevalues[TESTBUFFERSIZE];
    const BYTE expected[TESTBUFFERSIZE];
    const BYTE expected_read[TESTBUFFERSIZE];
};

BufferArrayTestStruct BufferArrayTest[]=
{
    //0
    {2,4,12,
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //Start
    {0x04, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //writevalues
    {0x00, 0x43, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //expected
    {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} //expected_read
    },

    //1
    {3,1,12,
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //Start
    {0x02, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, //writevalues
    {0x01, 0x18, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00}, //expected
    {0x80, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} //expected_read
    },

    //2
    {2,4,4,
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, //Start
    {0x04, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //writevalues
    {0xf0, 0x43, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff}, //expected
    {0x04, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} //expected_read
    },

    //3
    {3,1,7,
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, //Start
    {0x02, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, //writevalues
    {0x81, 0x18, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff}, //expected
    {0x8c, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00} //expected_read
    },

    //4
    {6,1,8,
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, //Start
    {0x02, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, //writevalues
    {0x81, 0x18, 0x00, 0x80, 0x00, 0x00, 0x7f, 0xff}, //expected
    {0x18, 0x00, 0x80, 0x00, 0x00, 0x7f, 0x00, 0x00} //expected_read
    },


    //5
    {6,7,9,
    {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, //Start
    {0x02, 0x30, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00}, //writevalues
    {0xfe, 0x04, 0x60, 0x02, 0x00, 0x00, 0x01, 0xff}, //expected
    {0x8, 0xc0, 0x04, 0x00, 0x00, 0x03, 0x00, 0x00} //expected_read
    }


};



#endif


bool Buffer::ReadTest(bool verbose)
{
    bool pass=true;
    int i=0;

#ifdef BUFFER_TEST
    
    Buffer b;
    Buffer expected;
    BufferTestStruct* bt;
    
    for(i=0;i<sizeof(BufferTest)/sizeof(BufferTestStruct);i++)
    {
        uint64_t rval;
        bt=&BufferTest[i]; 
        b.set(bt->expected,TESTBUFFERSIZE);
        //b.print();
        if ((rval=b.read(bt->offset_read,bt->length)) != bt->ExpectedReadValue)
        {
            pass=false;
            printf("Buffer read failure: test %d\n  val:%2x <> expected:%2x:   \n",i, rval, bt->ExpectedReadValue);
        }
    }
#endif
    return !pass;
}

bool Buffer::WriteTest(bool verbose)
{
    bool pass=true;
    int i=0;


#ifdef BUFFER_TEST
    Buffer b;
    Buffer expected;
    //Run test with sign bits (North and West) set    
    for(i=0;i<sizeof(BufferTest)/sizeof(BufferTestStruct);i++)
    {
        b.set(BufferTest[i].start,TESTBUFFERSIZE);
        expected.set(BufferTest[i].expected,TESTBUFFERSIZE);
    
        b.write(BufferTest[i].value, BufferTest[i].offset, BufferTest[i].length);
        if (b.compare(expected))
        {
            pass=false;
            printf("Buffer write failure: %d\n  Result:   \n",i);
            b.print();
            printf("  Expected: ");
            expected.print();
        }
    } 
    return !pass;
#endif

}

bool Buffer::WriteArrayTest(bool verbose)
{
    bool pass=true;
    int i=0;


#ifdef BUFFER_TEST
    Buffer b;
    Buffer expected;
    //Run test with sign bits (North and West) set    
    for(i=0;i<sizeof(BufferArrayTest)/sizeof(BufferArrayTestStruct);i++)
    {
        b.set(BufferArrayTest[i].start,TESTBUFFERSIZE);
        expected.set(BufferArrayTest[i].expected,TESTBUFFERSIZE);
        b.writeArray(BufferArrayTest[i].writevalues, BufferArrayTest[i].offset, BufferArrayTest[i].bytelength);
        if (b.compare(expected))
        {
            pass=false;
            printf("Buffer write Array failure: %d\n  Result:   ",i);
            b.print();
            printf("  Expected: ");
            expected.print();
        } 
 
    }
#endif

    return !pass;
}

bool Buffer::ReadArrayTest(bool verbose)
{
    bool pass=true;
    int i=0;

#ifdef BUFFER_TEST
    Buffer b;
    Buffer expected;
    BufferArrayTestStruct* bat;
    //Run test with sign bits (North and West) set    
    for(i=0;i<sizeof(BufferArrayTest)/sizeof(BufferArrayTestStruct);i++)
    {
        bat=&BufferArrayTest[i];
        BYTE readbuf[bat->bytelength];
        b.set(bat->expected,TESTBUFFERSIZE);
        //Use expected array from write test for input.
        b.readArray(bat->offset_read, bat->bytelength, readbuf);

        //Use buffer to check values
        Buffer rd(readbuf,bat->bytelength);
        Buffer eb(bat->expected_read, bat->bytelength);
        if (rd.compare(eb,bat->bytelength))
        {
            Buffer eb(bat->expected_read, bat->bytelength);
            pass=false;

            printf("Buffer read  Array failure: %d\n  Result:   ",i);
            rd.print(bat->bytelength);
            printf("  Expected: ");
            eb.print(bat->bytelength);
            printf("\n\n");
        } 
        //printf("\n\n"); //<===
 
    }
#endif

    return !pass;
}

bool Buffer::PatternWalkTest(bool verbose)
{
    const int TESTBUFSIZE=16;
    unsigned char zeroes[TESTBUFSIZE];
    unsigned char ones[TESTBUFSIZE];
    Buffer buf;
    unsigned int readval;
    unsigned int writeval;
    bool pass=true;
    uint64_t testcount=0;

    for (int i=0;i<TESTBUFSIZE;i++)
    {
        zeroes[i]=0;
        ones[i]=0xff;
    }

    for (writeval=0x3; writeval<=0x99999999;writeval+=(writeval/128)+1)
    {
        for (int bitoffs=0;bitoffs<40;bitoffs++)
        {
            buf.set(zeroes,sizeof(zeroes)); 
            int minbits=bitsize(writeval);
            for (int bits=bitsize(writeval);bits<32;bits++)
            {
                buf.write(writeval,bitoffs,bits);

                readval=buf.read(bitoffs,bits);
               
                if (readval != writeval)
                { 
                    printf("0x%x<> 0x%x   %d %d\n",readval,writeval,bits,bitoffs);
                    pass=false;
                }

                //Also test to see if we write a zero back to the buffer, it is restored to its 
                //previous state.
                buf.write(0,bitoffs,bits);
                if (memcmp(buf.getBuffer(),zeroes,TESTBUFSIZE))
                {
                    printf("Buffer bounds failure %x %x bits:%d bitoffs:%d\n",readval,writeval,bits,bitoffs);
                    pass=false;
                }


                testcount++;
            }
        }
    }
    //printf("%ld tests\n",testcount);

    return !pass;
}




static TestEntry testlist[]=
{
    {Buffer::PatternWalkTest,"Buffer Pattern Walk Test"},
    {Buffer::ReadTest,"Buffer Read Test"},
    {Buffer::WriteTest,"Buffer Write Test"},
    {Buffer::WriteArrayTest,"Buffer Array Write Test"},
    {Buffer::ReadArrayTest,"Buffer Array Read Test"},
    {NULL,NULL}
};


bool Buffer::Test(bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef BUFFER_TEST

    while (testlist[i].func)
    {
        if ((*testlist[i].func)(verbose))
        {
            pass=false;
            if (verbose) printf("%s failed\n",testlist[i].name);
        }
        else
        {
            if (verbose) printf("%s passed\n",testlist[i].name);
        }
        i++;
    } 

#endif
    return !pass;
}





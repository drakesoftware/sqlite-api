/// \file Buffer.h
/// \brief  Read and write values into/from an arbitrary size buffer.

///Read/write values, of an arbitrary size (in bits) into/from a location
///that can cross byte boundaries. Values read will always be shifted so the last bit wiill
///be in the LSB of the returned values.
///
///The buffer must always be initialized with a byte aligned array and getBuffer returns a pointer to the
///entire raw buffer, which is simply an unsigned char (BYTE) array.
#ifndef __Buffer__h
#define __Buffer__h

#include "stdio.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "utils.h"


///Allow reads and writes to buffer of arbitrary size at arbirary
///(non byte aligned) offsets
class Buffer
{
    static const int BITS_PER_BYTE = 8; 
    static const unsigned int BYTEMASK = 0xff;
    unsigned char* mBuffer;
    int mSize;
    
    public:

    Buffer(void)
    {
        mBuffer=NULL;
        mSize=0;
    }

    void clear(void)
    {
        if (mSize)
        {
            memset(mBuffer,0,mSize);
        }
    }

    void deallocate(void)
    {
        mSize=0;
        delete [] mBuffer;
    }

    void allocate(int size)
    {
        if (mBuffer)
        {
            deallocate();
        }
        mBuffer=new unsigned char[size];
        mSize=size;
    }
    Buffer(int size)
    {
        mBuffer=NULL;
        allocate(size);
        clear();
    }

    Buffer(const BYTE *buf, int size)
    {
        mBuffer=NULL;
        set(buf,size);
    }

    ~Buffer(void)
    {
        deallocate();
    }

    ///\returns the allocated size of the buffer
    int getSize(void) const
    {
        return mSize;
    }

    //Get pointer to buffer
    BYTE* getBuffer(void) const
    {
        return mBuffer;
    }

    ///Populate buffer with a constant byte value
    void set(BYTE v)
    {
        for (int i=0;i<mSize;i++)
        {
            mBuffer[i]=v;
        }
    }

    ///Populate the contents of the buffer
    ///from an existing array
    void set(const BYTE *buf, int size)
    {
        if (mBuffer)
        {
            deallocate();
        }
        
        mBuffer=new unsigned char[size];
        mSize=size;
        for (int i=0;i<mSize;i++)
        {
            if (i >=size)
            {
                mBuffer[i]=0;
            }
            else
            {
                mBuffer[i]=buf[i];
            }
        }
    }; 


    ///Read a value from the buffer. 
    ///\param[in] offset        offset in bits to start reading
    ///\param[in]  bitlength  Number of bits to read
    BYTE  readByte(int offset, int length);

    ///Read value from buffer
    ///\param length     Number of bits (1-8)
    ///\param offset        offset in bits of array (1 to getSize()*8-1)
    ///\returns             value ,
    uint64_t read(int offset, int length);

    ///Read bytes from buffer at bit offset
    ///Note: bytelength number is # of BYTES (not bits) to be read, but offset
    ///    need not be on a byte boundary
    ///\param[in] offsetin]    Bit index/offset to 
    ///\param[in] lengthin]  Number of bytes  
    ///\param[out] bufout]      //Values read from buffer
    ///\returns pointer to buf
    BYTE* readArray(int offset, int length, BYTE* buf);

    ///Write value into buffer
    ///\param val   value to be written (only 'length' bits are used)
    ///\param offset    offset (bits) where value is to be written
    ///\param length Number of bits of value to write into array
    void writeByte(BYTE val, int offset, int length);
    

    ///Write value into buffer.
    ///\param value     Value to be written <= 8 bytes
    ///\param length      Number of bits of value to be written (2-32)
    ///                 (0=start at bit 7 (byte boundary)
    ///\param offset    Offset (in bits) of value to be writen 
    void write(uint64_t  value, int offset, int length);

    ///Write bytes from into buffer at bit offset
    ///Note: bytelength number of bytes will be written, but offset
    ///    need not be on a byte boundary
    ///\param[in] values        Array of values
    ///\param[in] offset        offset  (bits)
    ///\param[in] length        Number of bytes  
    void writeArray(const BYTE* values, int offset, int length);
   
    ///Print buffer to stdout.
    ///\param  linelength   length for each line
    ///\param length        number of bytes to print, default = all 
    void print(int linelength=16, int length=0)
    {
        int i;
        if (0 == length)
        {
            length=mSize;
        }
        for (i=0;i<length;i++)
        {
            if (i%linelength==0)
            {
                printf("%03x:",i);
            }
            printf("%02x",mBuffer[i]);
            if ((i+1)%linelength)
            {
                printf(" ");
            }
            else
            {
                printf("\n");
            }
        }
        if ((i)%linelength)
        {
            printf("\n");
        }

    }

    BYTE& operator[](int index)
    {
        return mBuffer[index];
    }

    //Compare buffers,byte by byte
    int compare(Buffer& b,int length=0)
    {
        if (0 == length)
        {
            length=mSize;
        }

        if(mSize != b.getSize())
        {
            return -1;
        }
        //return memcmp(mBuffer, b.getBuffer(), length);
        for (int i=0;i<length;i++)
        {
            //printf("%x .. %x\n",mBuffer[i],b[i]);
            if (mBuffer[i] !=  b[i])
            {
                return 1;
            }
        }
            return 0;
    }


    //Test functions
    static bool ReadTest(bool verbose);
    static bool ReadArrayTest(bool verbose);
    static bool WriteTest(bool verbose);
    static bool WriteArrayTest(bool verbose);
    static bool PatternWalkTest(bool verbose);

    ///Regression test 
    static bool Test(bool verbose);
 



};





#endif


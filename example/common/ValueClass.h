#ifndef __valueclass__h
#define __valueclass__h

#include "utils.h"
#include "Validity.h"
#include <string>

//Store a simple range of values with
//validity flag

template<unsigned bits>
class ValueClass:public Validity
{

    unsigned int maskval(unsigned int v)
    {
        return v & BITMASK_LSBS(bits);
    }

public:
	unsigned int mValue;
    
    void set(unsigned int v)
    {
        mValue=maskval(v);
        setValid();
    }


    ValueClass(unsigned int v)
    {
        mValue=maskval(v);
        setValid();
    }

    ValueClass()
    {
        clear();
    }

    unsigned int get(void)
    {
        return mValue;
    }


    std::string toString(void)
    {
        return toFormattedString(mValue,3);
    }

};

#endif

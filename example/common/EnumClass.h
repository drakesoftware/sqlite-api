#ifndef __enumclass__h
#define __enumclass__h

#include "utils.h"
#include "Validity.h"
#include <string>

template<class T>
class EnumClass:public Validity
{

public:
	T mValue;
    

    EnumClass<T>(T v)
    {
        mValue=v;
        setValid();
    }

    EnumClass<T>()
    {
        clear();
    }

    void set(T v)
    {
        mValue=v;
        setValid();
    }

    T get(void)
    {
        return mValue;
    }


    EnumClass<T> operator=(T v)
    {
        mValue=v;
        setValid();
    }


    std::string toString(void)
    {
        return to_string(mValue,3);
    }

};

#endif

#ifndef __validity__h
#define __validity__h


class Validity
{
    bool mValid;

public:

    Validity()
    {
        mValid=false;
    }

    void clear(void)
    {
        mValid=false;
    }

    void setValid(bool valid=true)
    {
        mValid=valid;
    }

    bool isValid(void)
    {
        return mValid;
    }


};

#endif

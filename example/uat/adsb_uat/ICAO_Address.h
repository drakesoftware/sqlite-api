// \file ICAO_Address.h
/// \brief  Address

#ifndef __icao_address__h
#define __icao_address__h

#include <stdint.h>
#include "icaocodes.h"
#include "Validity.h"

///Address qualifier field in header.
///DO-282 2.2.4.5.1.2
    

enum AddressQualifier
{
    ICAO=     0x0,    ///ICAO address
    SELF=     0x1,    ///Self assigned temporary address
    TISB_ADSR=0x2,    ///TIS-B or ADS-R target ICAO
    TISB_TRK= 0x3,    ///TIS-B target with track file ID
    SURFACE=  0x4,    ///Surface vehicle 
    BEACON=   0x5,    ///Fixed ADS-B beacon
    ADSR_NON_ICAO= 0x6,///Non-ICAO ADS-R address
    RESERVED1=0x7    ///Reserved value
};


class Address:public Validity
{
    unsigned int mAddress;
    AddressQualifier mAddressQualifier;

    public:
    
    AddressQualifier getAddressQualifier()
    {
        return mAddressQualifier;
    }
    
    void setAddressQualifier(AddressQualifier qual)
    {
        mAddressQualifier=qual;
    }

    ///Set target ICAO
    void setICAO(void)
    {
        mAddressQualifier=ICAO;
    }


    void setSelf(void)
    {
        mAddressQualifier=SELF;
    }
    
    void setTISB(void)
    {
        mAddressQualifier=TISB_ADSR;
    }

    void setTISB_TRK(void)
    {
        mAddressQualifier=TISB_TRK;
    }

    void setSurface(void)
    {
        mAddressQualifier=SURFACE;
    }

    void setBeacon(void)
    {
        mAddressQualifier=BEACON;
    }
    
    void setADSR_NON_ICAO(void)
    {
        mAddressQualifier=ADSR_NON_ICAO;
    }

    void init(void)
    {
        mAddressQualifier=RESERVED1; //Invalid value
    }

    ///Is address ICAO (permanent address)?
    bool isICAO(void)
    {
        return (ICAO == mAddressQualifier);
    }

    ///Is address self assigned temporary address?
    bool isTemp(void)
    {
        return (SELF == mAddressQualifier);
    }
    
    ///Is address a TISB (or ADS-R)?
    bool isTISB_ADSR(void)
    {
        return (TISB_ADSR == mAddressQualifier);
    }

    ///Is address TISB TRK file ID //Unimplemented
    bool isTISB_TRK(void)
    {
        return (TISB_TRK == mAddressQualifier);
    }

    ///Is address surface vehicle address
    bool isSurface(void)
    {
        return (SURFACE == mAddressQualifier);
    }

    ///Is address of an ADS-B beacon
    bool isBeacon(void) 
    {
        return (BEACON == mAddressQualifier);
    } 

    ///Is address surface veh
    bool isADSR_NON_ICAO(void) 
    {
        return (ADSR_NON_ICAO== mAddressQualifier);
    } 


    Address(void)
    {
        setSelf();
        mAddress=0x0;
        clear();
    }

    //Default address qualifier is ICA0
    Address(unsigned int address)
    {
        mAddress=address;
        setICAO();
        setValid();
    }

    Address(unsigned int address, AddressQualifier aq)
    {
        mAddress=address;
        setAddressQualifier(aq);
        setValid();
    }


    unsigned int get(void)
    {
        return mAddress;
    }

    ///Set address (if not all 0's or all 1's)
    ///otherwise leave unchanged
    //TODO: Through exception failure?
    void set(unsigned int address)
    {
        //Set address if valid 
        if (address > 0x0 &&
            address < 0xffffff)
        {
            mAddress=address;
            setValid();
        }
    }
    void set(unsigned int address, AddressQualifier aq)
    {
        //Set address if valid 
        if (address > 0x0 &&
            address < 0xffffff)
        {
            set(address);
            setValid();
        }
        setAddressQualifier(aq);
    }



    ICAO_Country getCountry(void)
    {
        for (int i=0;i<ICAO_COUNTRY_CODE_END; i++)
        {
            int prefix_shift=ICAO_ADDRESS_SIZE - icao_codes[i].prefix_size;

            unsigned int prefix= mAddress >> prefix_shift;
            if (prefix == icao_codes[i].prefix >> prefix_shift)
            {
                return icao_codes[i].country;
            }
        }
        return  ICAO_COUNTRY_CODE_ERROR;
    }
    
}; 



#endif


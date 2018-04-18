/// \file CapabilityCode.cpp
/// \brief  CapabilityCode  DO-282 2.2.4.5.4.12
///

   #include "CapabilityCode.h"

   bool CapabilityCodes::Test(bool verbose)
   {
   		CapabilityCode cc;
    	bool pass=true;
   		cc.setStandard(UATIN|ESIN|TCAS);
   		cc.getEncoded();

		return !pass;
   }

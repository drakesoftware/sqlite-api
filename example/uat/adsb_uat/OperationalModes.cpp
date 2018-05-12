/// \file OperationalModes.cpp
/// \brief  OperationalMode  DO-282 2.2.4.5.4.13
///
   #include "OperationalModes.h"

   bool OperationalModes::Test(bool verbose)
   {
   		OperationalModes om;

   		om.setStandard(TCAS|IDENT);
   		om.getEncoded();
   }

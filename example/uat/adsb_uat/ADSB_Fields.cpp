/// \file ADSB_Fields.cpp
/// \brief  Regression test routines for ADSB (UAT) field
//encode/decode functions


//#include "ADSB_Fields.h"
#include "Altitude.h"
#include "Latitude.h"
#include "Longitude.h"
#include "HorizontalVelocity.h"
#include "VerticalVelocity.h"
#include "utils.h"


#include "RegressionTests.h"

///Test values for DO-282 Table 2-76
///Table values have been correct, but still do not agree
///with field encoding/decoding as described in secion 2.2.4.5.2.1 
///and Table 2-14



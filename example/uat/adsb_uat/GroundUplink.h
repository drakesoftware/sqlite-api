///\file GroundUplink.h
///\brief Ground Uplink header and packet


#ifndef __grounduplink_h_
#define __grounduplink_h_


//UAT specific header 2.2.3.2.2.1   UPLINK
/*  Translated form Spec bit order and numbering to standard convetion
Header: bytes 0-8
-----------------
Fields:         Bit  
Latitude        0-22 
Longitude       23-46 
Pos valid       47          1=valid 
UTC Coupled     48          1=UTC coupled
Reserved        49 
App Data Valid  50
Slot ID         51-55       Uplink slot
TIS-B Site ID   56-59  
Reserved        60-63 

App Data        Bytes 8-431
*/







#endif

/// \file UATData.cpp
/// \brief  UAT ADSB data storage

#include "UATData.h"


//Does not compile, anothe approach needed
// UATData target1=
// {
//     0,                                      //0  Mutex
//     UAT_PAYLOAD_TYPE_3,                     //1  Payload Type
//     {0x896003,AddressQualifier::ICAO},      //2  Address: 9003011
//     (double)38.94,                          //3  Latitude
//     (double)95.24,                          //4  Longitude

    
//     (int)500,                               //5  Altitude 500 ft
//     (EAltitudeType) ALTITUDE_TYPE_PRESSURE, //6  AltitudeType

//     AG_SUBSONIC,                          //7  AirGround 
//     {AG_SUBSONIC,true,1000,true,500},     //8  Horizontal Velocity
//     NIC_20NM,                               //9  NIC
//     {AVSize::AVSIZE_15_WIDE,5,AVSize::LEFT},//10 AVSize
//     {false,false,256},                      //11 Vertical Velocity
//     UTC_UNCOUPLED,                            //12 UTC Coupling
//     (uint8_t) 3,                            //13 Uplink feedback

//     {"ABCDEFG",Callsign::LIGHT},            //14 Callsign 2.2.4.5.4.2 

//     (EEmergencyStatus)1,                    //15
//     3,                                      //16  MOPS verison
//     (ESIL)3,                                //17 SIL
//     200,                                    //18 TX MSO
//     (ESystemDesignAssurance)0,              //19 SDA
//     (ENACP)0,                               //20 ENAC
//     (ENACV)0,                               //21 ENACV
//     (ENICBaro)0,                            //22 ENICBaro
//     {2},                                    //23 Capabilities codes
//     {1},                                    //24 OperationalModes
//     (ECallsignID)1,                         //25 
//     (ESILSupplement)1,                      //26 
//     (EGeoVertAccuracy)1,                    //27 
//     (ESingleAntenna)1,                      //28
//     (ENICSupplement)1,                      //29
//     (ETCASOperational)1,                    //30
//     (ESelectedAltitudeType)SAT_MCP,         //31
//     (int) 5000,                             //32 SelectedAltitude 
//     10250,                                  //33 BaroSetting 
//     2,                                      //34 SelectedHeading
//     (EMCP_FCUStatus)STATUS_OFF,             //35
//     (EModeAutoPilotEngaged)AP_OFF,           //36
//     (EModeVNAV)VNAV_ON,                    //37
//     (EModeAltitudeHold)ALTHOLD_ON,         //38
//     (EModeApproach)APPROACH_ON,            //39
//     (EModeLNAV)LNAV_ON,                    //40

//     EQUIP_A0,                                //41
//     (unsigned int)0x3d                    //42 Secondary Altitude 500 ft
// };





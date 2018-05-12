/// \file RegressionTests.cpp
/// \brief  Execute regression tests

#include "ADSB_Fields.h"
#include "RegressionTests.h"
#include "utils.h"
#include "Buffer.h"
#include "HorizontalVelocity.h"
#include "VerticalVelocity.h"
#include "Longitude.h"
#include "Latitude.h"
#include "Altitude.h"
#include "Callsign.h"
#include "CapabilityCodes.h"
#include "OperationalModes.h"
#include "SelectedAltitude.h"
#include "SelectedHeading.h"

static TestEntry testlist[]=
{

    //{VerticalVelocity::Test, "Vertical velocity"},
    //{HorizontalVelocity::Test,"Horizontal velocity"},
    //{Latitude::Test, "Latitude"},
    //{Longitude::Test,"Longitude"},
    //{Altitude::Test, "Altitude"},
    //{Buffer::Test,"Buffer class Test"},
    //{Callsign::Test,"Callsign/Emitter category Test"},
    //{Enumtest, "EnumList Test"},
    //{CapabilityCode::Test, "Capability Codes Test"},
    //{OperationalModes::Test, "Operational Mode Test"},)
    //{SelectedAltitude::Test, "Selected Altitude Setting Test"},
    NULL,NULL
};



bool RunTestList(TestEntry testlist[],bool verbose)
{
    bool pass=true;
    int i=0;
#ifdef UAT_TEST

    while (testlist[i].func)
    {
        if ((*testlist[i].func)(verbose))
        {
            pass=false;
            printf("    FAILED:  %s failed\n",testlist[i].name);
        }
        else
        {
            printf("    PASSED:  %s \n",testlist[i].name);
        }
        i++;
    }
#endif
    return !pass;

}

bool RegressionTests::run(bool verbose)
{

    bool pass=true;
    int i=0;
    pass=!RunTestList(testlist,verbose);
    if (pass)
    {
        printf(">>>All regression tests passed.<<<\n");
    }
    return pass;

}



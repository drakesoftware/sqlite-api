#include <iostream>
#include "dbmanager.h"
#include "db.h"
#include "table.h"
#include "persistablebase.h"
#include "appdata.h"
#include "UATData.h"

int main()
{
    /*example*/

    UATData u;

    u.mAddress = {0x896003, AddressQualifier::ICAO};
    u.mLatitude = (double)38.94;
    u.mAltitude = (int)500;
    u.mOperationalModes = {1};
    u.mAirGroundState = AG_SUBSONIC;
    u.mCapabilityCodes = {2};
    std::unique_ptr<vector<std::unique_ptr<UATData>>> up_uat = PersistableBase::Select_UP(UATData());

    for (std::unique_ptr<UATData> &uatData : *up_uat)
    {
        cout << uatData->mAltitude.getStandard() << endl;
    }

    u.Save();
    u.Update();
    /*example*/

    //Insert
    AppData distance(1, "distance", "25Ft");
    distance.Save();

    AppData port(1, "port", "A");
    port.Save();
    //Select
    // vector<AppData> filtered = PersistableBase::Select(AppData(),
    //                                               apply_filter("key", string("distance"))
    //                                               .AND()
    //                                               .apply_filter("value", string("25Ft")));

    vector<AppData> filtered = PersistableBase::Select(AppData(),
                                                  "key = 'distance' AND value = '25Ft'");                                                  
    for (AppData &ad : filtered)
    {
        cout << ad.key() << ":" << ad.value() << endl;
    }
    //Update
    AppData &ad = filtered.at(0);
    ad.setKey("stars");
    ad.Update();
    //Delete
    filtered.at(1).Remove();

    /*example*/
    cin.ignore();
    return 0;
}
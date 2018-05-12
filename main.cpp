#include <iostream>
#include "dbmanager.h"
#include "db.h"
#include "table.h"
#include "entity.h"
#include "appdata.h"
#include "UATData.h"



int main()
{
/*example*/

    UATData u;
    u.mAddress = {0x896003,AddressQualifier::ICAO};
    u.mLatitude = (double)38.94;
    u.mAltitude = (int)500;
    u.mOperationalModes = {1};
    u.mAirGroundState = AG_SUBSONIC;
    u.mCapabilityCodes = {2};
    auto uat = Entity::Select(UATData());



    u.Save();
    u.Update();

//     AppData appSettings("app.db", "data1");
// //Insert
//     appSettings.populate(1, "distance", "25Ft");
//     appSettings.Save();

//     appSettings.populate(1, "port", "A");
//     appSettings.Save();
// //Select
//     auto all = Entity::Select(AppData("app.db", "data1"), 
//         apply_filter("key", string("distance"))
//         .AND()
//         .apply_filter("key", "25Ft"));
//     for(auto ad: all)
//     {
//         cout << ad.key() << ":" << ad.value() << endl;
//     }
// //Update
//     AppData& ad = all.at(0);
//     ad.setKey("stars");
//     ad.Update();
// //Delete
//     all.at(1).Remove();
    
/*example*/
    return 0;
}
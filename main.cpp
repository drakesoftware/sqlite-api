#include <iostream>
#include "dbmanager.h"
#include "db.h"
#include "field.h"
#include "table.h"
#include "entity.h"

class UATData: public Entity
{
 public:
    int Latitude;
    int Longitude;
    bool AirGroundState;
    const char* NIC;

    UATData():
    Latitude{1},
    Longitude{2},
    AirGroundState{true},
    NIC{"The NIC"}{
        auto firstRow = first();
        refSet(Latitude, firstRow["Latitude"]);
        refSet(Longitude, firstRow["Longitude"]);
        refSet(AirGroundState, firstRow["AirGroundState"]);
        refSet(NIC, firstRow["NIC"]);
    }

    void setData() override {
        set("Latitude", Latitude);
        set("Longitude", Longitude);
        set("AirGroundState", AirGroundState);
        set("NIC", NIC);        
    }
    
};
int main(){
    auto uatDb = DB("UAT");
    auto tab = uatDb.getTable("test");
    UATData u = tab.Create<UATData>();
    u.Save();   
     
    return 0;
}
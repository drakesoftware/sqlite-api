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
    UATData() = default;
    UATData(const char* dbName, const char* tableName):
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
class UATDataCreationPolicy{
public:
    template<class UATDataType>
    UATDataType Create(const char* dbName, const char* tableName){        
        return UATDataType(dbName, tableName);
    }
};

typedef DbManager<UATDataCreationPolicy> UATDBManager;

int main(){
    // UATData u = DbManager<UATData>::instance()
    //     .Create("UAT", "test");
    // u.Save();   
     UATData u = UATDBManager::instance().Create<UATData>("UAT","test");
     u.Save();
    return 0;
}
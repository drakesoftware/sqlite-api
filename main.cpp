#include <iostream>
#include "dbmanager.h"
#include "db.h"
#include "table.h"
#include "entity.h"
#include "appdata.h"

/**
 * Test data, derives from Enity class to support 
 * core DB features like save(), get()
*/
class UATData: public Entity
{
 public:
    int Latitude;
    int Longitude;
    bool AirGroundState;
    float Sig;
    string NIC;
    
    UATData() = default;
    UATData(const char* dbName, const char* tableName):
        Entity(dbName, tableName),
        Latitude{1},
        Longitude{2},
        AirGroundState{true},
        Sig(2.25),
        NIC{"The NIC"}{
            auto firstRow = first();
            reset(firstRow);
    }  
    /**
     * Overridden function, called internally when 
     * save is called on this entity object
    */
    void setData() override {
        set("Latitude", Latitude);
        set("Longitude", Longitude);
        set("AirGroundState", AirGroundState);
        set("NIC", NIC);        
        set("Sig", Sig);
    }    
    
    void reset(Columns col) {
        col.get(Latitude,"Latitude");
        col.get(Longitude,"Longitude");
        col.get(AirGroundState, "AirGroundState");
        col.get(NIC,"NIC");
        col.get(Sig,"Sig");
    }

    schema getschema() const override{
        return {
            {"Latitude", PLAT_INT},
            {"Longitude", PLAT_INT},
            {"AirGroundState", PLAT_BOOL},
            {"Sig", PLAT_DBL},
            {"NIC", PLAT_STR}
        };
    }
};


int main(){
    // /**
    //  * When creating an object, a singleton of DBManager 
    //  * that follows a specific creation policy can be used. 
    //  * Such functions can also be shifted to a factory.
    // */
   
    //  UATData u(
    //      /*Database name->*/ "/home/manish/git/sqlite-api/test.db", 
    //      /*Table name->*/ "UAT");
    //  u.Save();
    AppData("app.db", "data", 1, "FName", "Manish1" ).Save();
    AppData("app.db", "data", 1, "LName", "Verma1" ).Save();
    
    auto all = Entity::All(AppData("app.db", "data", 1, "", ""));
    for(auto ad: all){
        cout << ad.key() << ":" << ad.value() << endl;
    }
    return 0;
}
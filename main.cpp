#include <iostream>
#include "dbmanager.h"
#include "db.h"
#include "table.h"
#include "entity.h"

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
            firstRow.get("Latitude", Latitude);
            firstRow.get("Longitude", Longitude);
            firstRow.get("AirGroundState", AirGroundState);
            firstRow.get("NIC", NIC);
            firstRow.get("Sig", Sig);
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
    /**
     * When creating an object, a singleton of DBManager 
     * that follows a specific creation policy can be used. 
     * Such functions can also be shifted to a factory.
    */
   
     UATData u(
         /*Database name->*/ "/home/manish/git/sqlite-api/test.db", 
         /*Table name->*/ "UAT");
     u.Save();
    return 0;
}
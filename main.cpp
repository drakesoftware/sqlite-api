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
    const char* NIC;
    
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
};

/**
 * This is a data creation policy with a templated 
 * create function that creates the object for the 
 * type passed as an argument. A group of objects 
 * belonging to a specific domain (e.g. UAT) may 
 * follow a specific pattern when instantiated. A 
 * policy like this could be created for all such 
 * objects belonging to that group.  
*/
class UATDomainCreationPolicy{
public:
    template<class UATDataType>
    UATDataType Create(const char* dbName, const char* tableName){        
        return UATDataType(dbName, tableName);
    }
};

typedef DbManager<UATDomainCreationPolicy> UATDBManager;

int main(){
    /**
     * When creating an object, a singleton of DBManager 
     * that follows a specific creation policy can be used. 
     * Such functions can also be shifted to a factory.
    */
     UATData u = UATDBManager::instance()
        .Create<UATData>("/home/manish/git/sqlite-api/test.db", "UAT");
     u.Save();
    return 0;
}
#ifndef DBMANAGER_H
#define DBMANAGER_H
#include "entity.h"


class DefaultDataCreationPolicy{
public:
    template<class AnyDataType>
    AnyDataType Create(const char* dbName, const char* tableName){        
        return AnyDataType();
    }
};

/**
 * The class that creates recipe for an object according 
 * to the way it has to be created and then finally saved.
 * This class shall hold more features in future that will 
 * help configure object creation, serialization and 
 * deserialization.
*/
template<class EntityCreationPolicy = DefaultDataCreationPolicy>
class DbManager: public EntityCreationPolicy{
protected:
    DbManager() = default;
public: 
    static DbManager instance(){
        static DbManager s_instance;
        return s_instance;
    }
private:    
};

#endif //DBMANAGER_H
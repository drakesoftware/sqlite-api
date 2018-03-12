#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <db.h>
#include "entity.h"


class DefaultDataCreationPolicy{
public:
    template<class AnyDataType>
    AnyDataType Create(const char* dbName, const char* tableName){        
        return AnyDataType();
    }
};

template<class EntityCreationPolicy = DefaultDataCreationPolicy>
class DbManager: public EntityCreationPolicy{
protected:
    DbManager() = default;
public:
    // DB open(const char* dbName);    
    static DbManager instance(){
        static DbManager s_instance;
        return s_instance;
    }
private:    
};

#endif //DBMANAGER_H
#ifndef DBMANAGER_H
#define DBMANAGER_H


class DefaultDataCreationPolicy{
public:
    template<class AnyDataType>
    AnyDataType Create(const char* dbName, const char* tableName){        
        return AnyDataType();
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



typedef DbManager<UATDomainCreationPolicy> UATDBManager;

#endif //DBMANAGER_H
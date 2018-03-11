#ifndef ENTITY_H
#define ENTITY_H
#include "field.h"
#include "table.h"

class Entity{
public:
    Entity() = default;
    void Save(){
        m_fields.clear();
        setData();
        //do the db save
    }
    virtual void setData(){};

    template<
        class ValueType
    >
    void set(const char* key, ValueType value){
        Field<ValueType> field(key, value);
        m_fields.insert(std::pair<const char*, Field<ValueType> >(key, field));
    }
protected:
    std::map<const char*, IField> first(){
        std::map<const char*, IField> row;
        row.insert(std::pair<const char*, Field<int> >(
            "Longitude", 
            Field<int>("Longitude", 1)));
        row.insert(std::pair<const char*, Field<int> >(
            "Latitude", 
            Field<int>("Latitude", 5)));
        row.insert(std::pair<const char*, Field<bool> >(
            "AirGroundState", 
            Field<bool>("AirGroundState", true)));
        row.insert(std::pair<const char*, Field<const char*> >(
            "NIC", 
            Field<const char*>("NIC", "The NIC")));
        return row;
    }
    std::map<const char*, IField> last(){
        return last();
    }
    std::vector<std::map<const char*, IField> > all(){
        std::vector<std::map<const char*, IField> > vec;
        
        vec.push_back(first());
        return vec;
    }



    template<
        class ValueType
    >
    void refSet(ValueType& ref, IField ifield){
        Field<ValueType> field = static_cast<Field<ValueType>& >(ifield);
        ref = field.getValue();
    }
    
private:
    std::map<const char*, IField> m_fields;
};


#endif
#ifndef ENTITY_H
#define ENTITY_H
#include "table.h"
#include "columns.h"
#include <vector>


class Entity: public Columns{
public:
    Entity(const char* dbName, const char* tableName)
        :m_table(tableName, dbName){}

    void Save(){
        clear();
        setData();
        
        m_table.save(*this);
    }
    virtual void setData(){};
protected:
    Columns first(){
        
    }
    Columns last(){
        return first();
    }
    std::vector<Columns> all(){
        std::vector<Columns>  vec;
        
        vec.push_back(first());
        return vec;
    }
private:
    Table m_table;
};


#endif
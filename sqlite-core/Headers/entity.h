#ifndef ENTITY_H
#define ENTITY_H
#include "table.h"
#include "columns.h"
#include <vector>
#include <utility>
#include "databroker.h"



/**
 * The bae class for any data type that we want to save 
 * to database. The class can be decomposed further to 
 * derive policies related to saving data and then 
 * initializing user defined data objects from basic types.
*/
class Entity: public Columns{
public:
    Entity(const char* dbName, const char* tableName)
        :m_table(tableName, dbName){}

    void Save(){
        clear();
        setData();
        
        m_table.save(SqlValueFromColumn::create(*this));
    }
    virtual void setData(){};
protected:
    /**
     * A helper function that returns first value from db
    */
    Columns first(){
        sqlResult results;
        int rows = m_table.get(results, 1);
        if(rows > 0)
            return ColumnsFromSqlValue::create(getschema(), results[0]);
        else
            return Columns();
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
#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <utility>
#include "columns.h"
#include "table.h"


class Table;
/**
 * The bae class for any data type that we want to save 
 * to database. The class can be decomposed further to 
 * derive policies related to saving data and then 
 * initializing user defined data objects from basic types.
*/
class Entity: public Columns{
public:
    Entity(const char* dbName, const char* tableName);
    void Save();
    virtual void setData();    
protected:
    /**
     * A helper function that returns first value from db
    */
    Columns first();
    Columns last();
    std::vector<Columns> all();
private:
    Table m_table;
};


#endif
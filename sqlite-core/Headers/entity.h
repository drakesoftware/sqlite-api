#ifndef ENTITY_H
#define ENTITY_H
#include "table.h"
#include "columns.h"
#include <vector>
#include <utility>

class ColumnsFromSqlValue{
public:
    static Columns create(vector<SqlValue> values){
        Columns cols;
        for(size_t i = 0; i < values.size(); i++){
            const char* name = values[i].Name.data();
            switch(values[i].Tp){
                case SqlValueTypeEnum::TXT:
                    cols.set(name, values[i].TxtVal);
                    break;
                case SqlValueTypeEnum::FLT:
                    cols.set(name, values[i].FltVal);
                    break;
                case SqlValueTypeEnum::INT:
                    cols.set(name, values[i].IntVal);
                    break;
                case SqlValueTypeEnum::NUL:
                default:
                    break;
            }
        }
        return cols;
    }
};

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
        auto namesAndValues = getNamesAndValues();
        m_table.save(
            std::get<0>(namesAndValues), 
            std::get<1>(namesAndValues), 
            std::get<2>(namesAndValues));
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
            return ColumnsFromSqlValue::create(results[0]);
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
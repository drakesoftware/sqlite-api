#include "entity.h"
#include "databroker.h"

Entity::Entity(const char* dbName, const char* tableName)
    :m_table(tableName, dbName){}

void Entity::Save(){
    clear();
    setData();

    m_table.save(SqlValueFromColumn::create(*this));
}

Columns Entity::first(){
        sqlResult results;
        int rows = m_table.get(results, 1);
        if(rows > 0)
            return ColumnsFromSqlValue::create(getschema(), results[0]);
        else
            return Columns();
}

Columns Entity::last(){
    return first();
}

std::vector<Columns> Entity::all(){
    std::vector<Columns>  vec;
    sqlResult results;
    int rows = m_table.get(results);
    for(auto row: results){
        Columns cols = ColumnsFromSqlValue::create(getschema(), row);
        vec.push_back(cols);
    }    
    return vec;
}
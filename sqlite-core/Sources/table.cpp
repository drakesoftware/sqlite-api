#include "table.h"

Table::Table(const char* tableName, const char* dbName): 
        m_name{tableName}, m_db{dbName}{}

void Table::save(const Columns& columns){
        // m_db.createTable
        
}
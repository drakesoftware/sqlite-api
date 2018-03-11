#include "db.h"

DB::DB(const char* name): 
    m_name{name}, 
    m_dbTouched{false}{

}

Table DB::getTable(const char* tableName){
    auto find = this->m_tables.find(tableName);
    if(find != this->m_tables.end()){
        return m_tables[tableName];
    }
    else{
        Table table(tableName, std::shared_ptr<DB>(this));
        m_tables.insert(std::pair<const char*, Table>(tableName, table));
        return table;
    }
}

bool DB::dbTouched(){
    return this->m_dbTouched;
}

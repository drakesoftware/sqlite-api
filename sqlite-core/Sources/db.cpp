#include "db.h"

DB::DB(const char* name, const char* tableName): 
    m_name{name}, 
    m_tableName{tableName},
    m_dbTouched{false}{
}


bool DB::dbTouched(){
    return this->m_dbTouched;
}

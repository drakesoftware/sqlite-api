#include "table.h"
#include "sqlfactory.h"
#include <cmath>


Table::Table(const char* tableName, const char* dbName): 
        m_name{tableName}, m_db{dbName}, m_tableTouched{false}{}

bool Table::exists(){
        if(!m_tableTouched){
                string sql = SqlFactory::TABLE_EXISTS(m_name);
                int count = m_db.selectCountScalar(sql.data());
                return count > 0;
        }
}
void Table::create(vector<SqlField> fields){
        if(!exists()){
                auto sqlCreateTable = SqlFactory::CREATE_TABLE(m_name, fields);
                if(m_db.execScalar(sqlCreateTable.data()))
                        m_tableTouched = true;
        }
        else
                m_tableTouched = true;
}

void Table::save(vector<SqlValue> sqlValues){        
        if(!istableTouched()) create(SqlField::fromSqlValues(sqlValues));
        vector<string> names, values;
        for(auto val: sqlValues){
                names.push_back(val.Name);
                values.push_back(SqlFactory::treatSqlValue(val.toString(), val.Tp));
        }
        auto sqlInsertTable = SqlFactory::INSERT_TABLE(m_name, names, values);
        m_db.execScalar(sqlInsertTable.data());
}

int Table::get(sqlResult& results, int limit){
        string selectSql = SqlFactory::SELECT(m_name, limit);
        return m_db.select(selectSql.data(), results);
}
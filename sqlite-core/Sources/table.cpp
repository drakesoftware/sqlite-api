#include "table.h"
#include "sqlfactory.h"
#include <cmath>


Table::Table(const char* tableName, const char* dbName): 
        m_name{tableName}, m_db{dbName}, m_tableTouched{false}{}

bool Table::exists() const {
        if(!m_tableTouched){
                string sql = SqlFactory::TABLE_EXISTS(m_name);
                int count = m_db.selectCountScalar(sql.data());
                return count >= 0;
        }
}
void Table::create(const vector<SqlField>& fields){
        if(!exists()){
                vector<SqlField> newTable(fields);
                newTable.push_back(SqlField("_ID", SQL_INT, true));
                auto sqlCreateTable = SqlFactory::CREATE_TABLE(m_name, newTable);
                if(m_db.execScalar(sqlCreateTable.data()))
                        m_tableTouched = true;
        }
        else
                m_tableTouched = true;
}

void Table::save(const vector<SqlValue>& sqlValues) {        
        if(!istableTouched()) create(SqlField::fromSqlValues(sqlValues));
        vector<string> names, values;
        for(auto val: sqlValues){
                names.push_back(val.Name);
                values.push_back(SqlFactory::treatSqlValue(val.toString(), val.Tp));
        }
        auto sqlInsertTable = SqlFactory::INSERT_TABLE(m_name, names, values);
        m_db.execScalar(sqlInsertTable.data());
}

int Table::update(const int& _id, const vector<SqlValue>& sqlValues){
        if(exists()){
                vector<string> names, values;
                for(auto val: sqlValues){
                        names.push_back(val.Name);
                        values.push_back(SqlFactory::treatSqlValue(val.toString(), val.Tp));
                }
                auto sqlUpdateTable = SqlFactory::UPDATE_TABLE(m_name, _id, names, values);
                m_db.execScalar(sqlUpdateTable.data());
        }
}

int Table::get(sqlResult& results, int limit) const {
        string selectSql = SqlFactory::SELECT(m_name, limit);
        return m_db.select(selectSql.data(), results);
}

int Table::remove(const int& _id){
        if(exists()){
                string removeSql = SqlFactory::DELETE(m_name, _id);
                return m_db.execScalar(removeSql.data());
        }
        return -1;
}
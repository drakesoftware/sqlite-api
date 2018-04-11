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
void Table::create(vector<string> names, vector<char> types){
        if(!exists()){
                vector<SqlField> fields;
                for(int i = 0; i < names.size(); i++){
                        fields.push_back(
                                SqlFieldCreatorFromTypeStr::CreateField(names[i].c_str(), types.at(i)));
                }

                auto sqlCreateTable = SqlFactory::CREATE_TABLE(m_name, fields);
                if(m_db.execScalar(sqlCreateTable.data()))
                        m_tableTouched = true;
        }
        else
                m_tableTouched = true;
}
void Table::save(vector<string> names, vector<string> values, vector<char> types){
        if(!istableTouched()) create(names, types);

        auto sqlInsertTable = SqlFactory::INSERT_TABLE(m_name, names, values);
        m_db.execScalar(sqlInsertTable.data());
}

int Table::get(sqlResult& results, int limit){
        string selectSql = SqlFactory::SELECT(m_name, limit);
        return m_db.select(selectSql.data(), results);
}
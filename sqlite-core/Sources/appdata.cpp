#include "appdata.h"


AppData::AppData(const char* dbName, const char* tableName, const int userId, const string &key, const string &value):
    Entity(dbName, tableName),
    m_id{0},
    m_userId{userId},
    m_key{key},
    m_value{value}
{

}
// AppData::AppData(const Entity& entity):
//     Entity(entity)
// {

// }
void AppData::setData() {    
    set("id", m_id);
    set("key", m_key);
    set("value", m_value);
    set("userId", m_userId);
}

void AppData::reset(Columns cols){
    cols.get(m_id, "id");
    cols.get(m_key, "key");
    cols.get(m_value, "value");
    cols.get(m_userId, "userId");
}

schema AppData::getschema() const {
    return {
        {"id", PLAT_INT},
        {"key", PLAT_STR},
        {"value", PLAT_STR},
        {"userId", PLAT_INT}
    };
}

int AppData::id() const
{
    return m_id;
}

void AppData::setId(const int id)
{
    m_id = id;
}

int AppData::userId() const
{
    return m_userId;
}

void AppData::setUserId(const int userId)
{
    m_userId = userId;
}

string AppData::key() const
{
    return m_key;
}

void AppData::setKey(const string &key)
{
    m_key = key;
}

string AppData::value() const
{
    return m_value;
}

void AppData::setValue(const string &value)
{
    m_value = value;
}

vector<AppData> AppData::list(){
    vector<AppData> vec;
    auto cols = all();
    for(auto& col: cols){
        AppData a(*this);
        a.reset(col);
        vec.push_back(a);
    }
    return vec;
}
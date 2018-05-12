#include "appdata.h"

// AppData::AppData():
//     Entity{"app.db", "data1"}
// {

// }
AppData::AppData(const char* dbName, const char* tableName):
    Entity(dbName, tableName)
{

}
void AppData::populate(const int userId, const string &key, const string &value)
{
    m_userId = userId;
    m_key = key;
    m_value = value;
}

void AppData::pushData()
{
    set("key", m_key);
    set("value", m_value);
    set("userId", m_userId);
}

void AppData::reset(Columns cols)
{
    cols.get(m_key, "key");
    cols.get(m_value, "value");
    cols.get(m_userId, "userId");
}

schema AppData::getSchema() const
{
    return
    {
        {"key", PLAT_STR},
        {"value", PLAT_STR},
        {"userId", PLAT_INT}
    };
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

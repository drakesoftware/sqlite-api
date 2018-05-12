#include "columns.h"
#include "memory"


int Columns::getInt(const char* key, const int& def)
{
    auto find = m_intColumns.find(key);
    if(find == m_intColumns.end())
    {
        return def;
    }
    return m_intColumns[key];
}
unsigned int Columns::getUInt(const char* key, const unsigned int& def)
{
    return getInt(key, (int)def);
}

double Columns::getDouble(const char* key, const double& def)
{
    auto find = m_dblColumns.find(key);
    if(find == m_dblColumns.end())
    {
        return def;
    }
    return m_dblColumns[key];
}

float Columns::getFloat(const char* key, const float& def)
{
    return (float)getDouble(key, (double)def);
}

string Columns::getString(const char* key, const string& def)
{
    auto find = m_strColumns.find(key);
    if(find == m_strColumns.end())
    {
        return def;
    }
    return m_strColumns[key];
}

char* Columns::getChar(const char* key, const char* def)
{
    auto str = getString(key, string(def));
    return strdup(str.c_str());
}
int Columns::getBool(const char* key, const int& def)
{
    auto find = m_boolColumns.find(key);
    if(find == m_boolColumns.end())
    {
        return def;
    }
    return m_boolColumns[key];
}
sp_columns Columns::getColumns(const char* key)
{
    auto find = m_colsColumns.find(key);
    if(find == m_colsColumns.end())
    {
        return std::shared_ptr<Columns>();
    }
    return m_colsColumns[key];
}

bool Columns::get(int& refVar, const char* key)
{
    auto find = m_intColumns.find(key);
    if(find == m_intColumns.end())
    {
        return false;
    }
    refVar = m_intColumns[key];
    return true;
}
bool Columns::get(unsigned int& refVar, const char* key)
{
    return get((int&)refVar, key);
}

bool Columns::get(double& refVar, const char* key)
{
    auto find = m_dblColumns.find(key);
    if(find == m_dblColumns.end())
    {
        return false;
    }
    refVar = m_dblColumns[key];
    return true;
}
bool Columns::get(float& refVar, const char* key)
{
    return get((double&)refVar, key);
}
bool Columns::get(string& refVar, const char* key)
{
    auto find = m_strColumns.find(key);
    if(find == m_strColumns.end())
    {
        return false;
    }
    refVar = m_strColumns[key];
    return true;
}
bool Columns::get(const char* refVar, const char* key)
{
    string str;
    get(str, key);
    refVar = strdup(str.c_str());
}
bool Columns::get(bool& refVar, const char* key)
{
    auto find = m_boolColumns.find(key);
    if(find == m_boolColumns.end())
    {
        return false;
    }
    refVar = m_boolColumns[key];
    return true;
}

void Columns::set(const char* key, const int& value)
{
    auto find = m_intColumns.find(key);
    if(find != m_intColumns.end())
    {
        m_intColumns[key] = value;
    }
    else
    {
        m_intColumns.insert(std::make_pair(key, value));
    }
}

void Columns::set(const char* key, const unsigned int& value)
{                                   //TODO:...
    set(key, (const int&)value);    //if uint is a practical requirement  
}                                   //then we can handle it separately

void Columns::set(const char* key, const double& value)
{
    auto find = m_dblColumns.find(key);
    if(find != m_dblColumns.end())
    {
        m_dblColumns[key] = value;
    }
    else
    {
        m_dblColumns.insert(std::make_pair(key, value));
    }
}
void Columns::set(const char* key, const float& value)
{
    set(key, (const double&)value);
}

void Columns::set(const char* key, const string& value)
{
    auto find = m_strColumns.find(key);
    if(find != m_strColumns.end())
    {
        m_strColumns[key] = value;
    }
    else
    {
        m_strColumns.insert(std::make_pair(key, value));
    }
}
void Columns::set(const char* key, const char* value)
{
    set(key, string(strdup(value)));
}

void Columns::set(const char* key, const bool& value)
{
    auto find = m_boolColumns.find(key);
    if(find != m_boolColumns.end())
    {
        m_boolColumns[key] = value;
    }
    else
    {
        m_boolColumns.insert(std::make_pair(key, value));
    }
}

void Columns::set(const char* key, const sp_columns& value)
{
    auto find = m_colsColumns.find(key);
    if(find != m_colsColumns.end())
    {
        m_colsColumns[key] = value;
    }
    else
    {
        m_colsColumns.insert(std::make_pair(key, value));
    }
}

void Columns::remove(const char* name, const PlatformTypeEnum& tp)
{
    switch(tp)
    {
        case PLAT_BOOL:
        {
            auto find = m_boolColumns.find(name);
            if(find != m_boolColumns.end())
            {
                m_boolColumns.erase(find);
            }
            break;
        }
        case PLAT_DBL:
        {
            auto find = m_dblColumns.find(name);
            if(find != m_dblColumns.end())
            {
                m_dblColumns.erase(find);
            }
            break;
        }
        case PLAT_INT:
        {
            auto find = m_intColumns.find(name);
            if(find != m_intColumns.end())
            {
                m_intColumns.erase(find);
            }
            break;
        }
        case PLAT_STR:
        {
            auto find = m_strColumns.find(name);
            if(find != m_strColumns.end())
            {
                m_strColumns.erase(find);
            }
            break;
        }
        case PLAT_CMPST:
        {
            auto find = m_colsColumns.find(name);
            if(find != m_colsColumns.end())
            {
                m_colsColumns.erase(find);
            }
            break;
        }

    }
}
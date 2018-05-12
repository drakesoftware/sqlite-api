#ifndef COLUMNS_H
#define COLUMNS_H

#include <iostream>
#include <map>
#include <string>
#include "datadefinition.h"
#include <memory>
#include <algorithm>

template<class getset, class dataType>
class property: public getset
{
public:
    dataType getter()
    {
        return this->get();
    }
    void setter(dataType val)
    {
        return this->set(val);
    }
};


class Columns;
using std::string;
using sp_columns = std::shared_ptr<Columns>;

using mapInt = map<const char*, int, cmp_str>;
using mapStr = map<const char*, string, cmp_str>;
using mapFloat = map<const char*, double, cmp_str>;
using mapBool = map<const char*, bool, cmp_str>;
using mapCols = map<const char*, sp_columns, cmp_str>;

/**
 * This is a class that holds data for one row or one
 * object at a time. It can hold data for only int,
 * string, bool, double. But can be easily extended for
 * more data types limited only by the ones supported
 * by sqlite3. The specialized templated functions help
 * seemlessly add values to the internal associative
 * containers.
*/
class Columns
{

  public:
    virtual schema getSchema() const
    {
        schema scma;
        auto add = [&scma](const char* name, PlatformTypeEnum tp){scma.insert(make_pair(name, tp));};
        
        for(auto& p: m_boolColumns) add(p.first, PLAT_BOOL);
        for(auto& p: m_colsColumns) add(p.first, PLAT_CMPST);
        for(auto& p: m_dblColumns) add(p.first, PLAT_DBL);
        for(auto& p: m_intColumns) add(p.first, PLAT_INT);
        for(auto& p: m_strColumns) add(p.first, PLAT_STR);
        return scma;
    }
    
    int getInt(const char* key, const int& def = 0);
    char* getChar(const char* key, const char* def = 0);
    unsigned int getUInt(const char* key, const unsigned int& def = 0);
    double getDouble(const char* key, const double& def = 0.0);
    float getFloat(const char* key, const float& def = 0.0);
    string getString(const char* key, const string& def = "");
    int getBool(const char* key, const int& def = -1);
    sp_columns getColumns(const char* key);

    bool get(int& refVar   , const char* key);
    bool get(unsigned int& refVar, const char* key);
    bool get(float& refVar , const char* key);
    bool get(double& refVar , const char* key);
    bool get(string& refVar, const char* key);
    bool get(const char* refVar, const char* key);
    bool get(bool& refVar  , const char* key);

    void set(const char* key, const int& value);
    void set(const char* key, const unsigned int& value);
    void set(const char* key, const float& value);
    void set(const char* key, const double& value);
    void set(const char* key, const string& value);
    void set(const char* key, const char* value);
    void set(const char* key, const bool& value);
    void set(const char* key, const sp_columns& value);

    void clear()
    {
        m_intColumns.clear();
        m_strColumns.clear();
        m_boolColumns.clear();
        m_dblColumns.clear();
        m_colsColumns.clear();
    }

    void remove(const char* name, const PlatformTypeEnum& tp);
  private:
    mapInt m_intColumns;
    mapStr m_strColumns;
    mapBool m_boolColumns;
    mapFloat m_dblColumns;
    mapCols m_colsColumns;
};

#endif //COLUMNS_HColumns
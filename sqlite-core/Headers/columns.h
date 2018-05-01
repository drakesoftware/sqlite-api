#ifndef COLUMNS_H
#define COLUMNS_H

#include <iostream>
#include <map>
#include <string>
#include "datadefinition.h"

class Columns;
using std::string;

using mapInt = map<const char*, int, cmp_str>;
using mapStr = map<const char*, string, cmp_str>;
using mapFloat = map<const char*, float, cmp_str>;
using mapBool = map<const char*, bool, cmp_str>;
using mapObj = map<const char*, Columns>;

/**
 * This is a class that holds data for one row or one
 * object at a time. It can hold data for only int,
 * string, bool, float. But can be easily extended for
 * more data types limited only by the ones supported
 * by sqlite3. The specialized templated functions help
 * seemlessly add values to the internal associative
 * containers.
*/
class Columns
{

  public:
    virtual schema getschema() const
    {
        return {};
    }
    bool get(int& refVar   , const char* key);
    bool get(float& refVar , const char* key);
    bool get(string& refVar, const char* key);
    bool get(bool& refVar  , const char* key);
    bool get(Columns& refVar  , const char* key);

    void set(const char* key, const int& value);
    void set(const char* key, const float& value);
    void set(const char* key, const string& value);
    void set(const char* key, const bool& value);
    void set(const char* key, const Columns& value);
    void clear()
    {
        m_intColumns.clear();
        m_strColumns.clear();
        m_boolColumns.clear();
        m_dblColumns.clear();
    }

  private:
    mapInt m_intColumns;
    mapStr m_strColumns;
    mapBool m_boolColumns;
    mapFloat m_dblColumns;
    mapObj m_objColumns;
};

#endif //COLUMNS_H
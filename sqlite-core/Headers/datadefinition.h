#ifndef DATADEFINITION_H
#define DATADEFINITION_H
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
#include <map>

using namespace std;

enum PlatformTypeEnum
{
    PLAT_STR,
    PLAT_INT,
    PLAT_DBL,
    PLAT_BOOL
};


enum SqlTypeEnum
{
    SQL_STR,
    SQL_INT,
    SQL_DBL,
    NUL,
    LOGC
};


enum FilterOperatorEnum
{
    EQ,
    LT,
    LTQ,
    GT,
    GTQ,
    NQ
};

/**
 * This stucture holds value of one column coming
 * from the sqlite3 db. They can be any type. While
 * initializing the object, it is determined as to
 * what data type the column is initialized as.
 * This struct can be improved in many ways. To
 * begin with, it could be converted into a union.
*/
struct SqlValue
{
    const char* Name;
    int IntVal = 0;
    string TxtVal = "";
    float FltVal = 0;
    SqlTypeEnum Tp;
    SqlValue() = default;
    SqlValue(const char* name, const int& intVal):
        Name(name), IntVal(intVal), Tp(SQL_INT) {}
    SqlValue(const char* name, const string& charVal):
        Name(name), TxtVal(charVal), Tp(SQL_STR) {}
    SqlValue(const char* name, const float& dblVal):
        Name(name), FltVal(dblVal), Tp(SQL_DBL) {}
    SqlValue(const char* name):
        Name(name), Tp(NUL) {}

    string toString()
    {
        switch(Tp)
        {
            case SQL_DBL:
                return to_string(FltVal);
            case SQL_INT:
                return to_string(IntVal);
            case SQL_STR:
                return TxtVal;
            default:
                return "";
        }
    }
};

struct SqlField
{
    SqlField(
        const char* name,
        const SqlTypeEnum& type = SqlTypeEnum::SQL_STR,
        const bool& isKey = false,
        const int& sz = 0,
        const bool& notNull = false):
        Name(name), Type(type), Sz(sz), IsKey(isKey), NotNull(notNull) {}
    const char* Name;
    SqlTypeEnum Type;
    int Sz;
    bool IsKey;
    bool NotNull;

    static vector<SqlField> fromSqlValues(const vector<SqlValue>& sqlValues)
    {
        vector<SqlField> fields;
        for(auto val:sqlValues)
        {
            fields.push_back(SqlField(val.Name, val.Tp));
        }
        return fields;
    }
};

struct cmp_str
{
    bool operator()(const char* a, const char* b)
    {
        return std::strcmp(a, b) < 0;
    }
};

using mapSqlValue = map<const char*, SqlValue, cmp_str>;
using schema = map<const char*, PlatformTypeEnum, cmp_str>;

#endif //DATADEFINITION_H
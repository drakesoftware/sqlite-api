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
    PLAT_BOOL,
    PLAT_CMPST
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

struct PlatUnit
{

    const char* Name;
    int IntVal = 0;
    string TxtVal = "";
    double DblVal = 0;
    bool BoolVal = false;
    PlatformTypeEnum Tp;
    PlatUnit() = default;
    
    PlatUnit(const char* name, const int& intVal):
        Name(name), IntVal(intVal), Tp(PLAT_INT) {}
    PlatUnit(const char* name, const string& charVal):
        Name(name), TxtVal(charVal), Tp(PLAT_STR) {}
    PlatUnit(const char* name, const double& dblVal):
        Name(name), DblVal(dblVal), Tp(PLAT_DBL) {}
    PlatUnit(const char* name, const bool& boolVal):
        Name(name), BoolVal(boolVal),Tp(PLAT_BOOL) {}
};


/**
 * This stucture holds value of one column coming
 * from the sqlite3 db. They can be any type. While
 * initializing the object, it is determined as to
 * what data type the column is initialized as.
 * This struct can be improved in many ways. To
 * begin with, it could be converted into a union.
*/
struct SqlUnit
{
    const char* Name;
    int IntVal = 0;
    string TxtVal = "";
    double FltVal = 0;
    SqlTypeEnum Tp;
    SqlUnit() = default;
    SqlUnit(const char* name, const int& intVal):
        Name(name), IntVal(intVal), Tp(SQL_INT) {}
    SqlUnit(const char* name, const string& charVal):
        Name(name), TxtVal(charVal), Tp(SQL_STR) {}
    SqlUnit(const char* name, const double& dblVal):
        Name(name), FltVal(dblVal), Tp(SQL_DBL) {}
    SqlUnit(const char* name):
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

struct DBField
{
    DBField(
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

    static vector<DBField> fromSqlUnits(const vector<SqlUnit>& sqlUnits)
    {
        vector<DBField> fields;
        for(auto val:sqlUnits)
        {
            fields.push_back(DBField(val.Name, val.Tp));
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

using mapSqlUnit = map<const char*, SqlUnit, cmp_str>;
using schema = map<const char*, PlatformTypeEnum, cmp_str>;

#endif //DATADEFINITION_H
#ifndef DATADEFINITION_H
#define DATADEFINITION_H
#include <string>
#include <iostream>
#include <cstring>
#include <map>

using namespace std;

enum PlatformTypeEnum{
    PLAT_STR,
    PLAT_INT,
    PLAT_DBL,
    PLAT_BOOL
};


enum SqlTypeEnum{
    SQL_STR,
    SQL_INT,
    SQL_DBL,
    NUL
};
  
/**
 * This stucture holds value of one column coming 
 * from the sqlite3 db. They can be any type. While 
 * initializing the object, it is determined as to 
 * what data type the column is initialized as. 
 * This struct can be improved in many ways. To 
 * begin with, it could be converted into a union.
*/
struct SqlValue{
    SqlTypeEnum Tp;
    int IntVal = 0;
    string TxtVal = "";
    float FltVal = 0;
    const char* Name;
    SqlValue() = default;
    SqlValue(const char* name, const int& intVal):
        Name(name), IntVal(intVal), Tp(SQL_INT){}
    SqlValue(const char* name, const string& charVal):
        Name(name), TxtVal(charVal), Tp(SQL_STR){}
    SqlValue(const char* name, const float& dblVal):
        Name(name), FltVal(dblVal), Tp(SQL_DBL){}
    SqlValue(const char* name):
        Name(name), Tp(NUL){}

    string toString(){
        switch(Tp){
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

struct SqlField{
    SqlField(
        const char* name, 
        const SqlTypeEnum& type = SqlTypeEnum::SQL_STR, 
        const int& sz = 0, 
        const bool& isKey = false, 
        const bool& notNull = false):
        Name(name), Type(type), Sz(sz), IsKey(isKey), NotNull(notNull){}
    const char* Name;
    SqlTypeEnum Type;
    int Sz;
    bool IsKey;
    bool NotNull;

    static vector<SqlField> fromSqlValues(const vector<SqlValue>& sqlValues){
        vector<SqlField> fields;
        for(auto val:sqlValues){
            fields.push_back(SqlField(val.Name, val.Tp));
        }
        return fields;
    }
};

struct cmp_str{
    bool operator()(const char* a, const char* b){
        return std::strcmp(a, b) < 0;
    }
};

using mapSqlValue = map<const char*, SqlValue, cmp_str>;
using mapInt = map<const char*, int, cmp_str>;
using mapStr = map<const char*, string, cmp_str>;
using mapFloat = map<const char*, float, cmp_str>;
using mapBool = map<const char*, bool, cmp_str>;
using schema = map<const char*, PlatformTypeEnum, cmp_str>;

#endif //DATADEFINITION_H
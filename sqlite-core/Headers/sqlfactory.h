#ifndef SQLFACTORY_H
#define SQLFACTORY_H
#include <iostream>
#include <string>
#include <cstdio>
#include <utility>
#include <vector>
#include "datadefinition.h"

using namespace std;


string& operator<<(string& str1, const string& str2){
    return str1.append(str2);
}
string& operator<<(string& str1, const char* cstr){
    str1 += cstr;
    return str1;
}
string& operator<<(string& str1, const int& num){
    str1 += to_string(num);
    return str1;
}
string& operator<<(string& str1, const double& dbl){
    str1 += to_string(dbl);
    return str1;
}
string& operator<<(string& str1, const float& flt){
    str1 += to_string(flt);
    return str1;
}
/**
 * This is the class where all sql strings are made 
 * out of the data that it receives from client objects. 
 * It has functions for INSERT, SELECT and CREATE TABLE. 
 * More can be added in due course.
*/
class SqlFactory{
private:

    static inline void join(const vector<string>& v, char c, string& s) {
        s.clear();
        for (vector<string>::const_iterator p = v.begin();
             p != v.end(); ++p) {
           s += *p;
           if (p != v.end() - 1)
             s += c;
        }
    }
    static const char* SqlTypeEnum_To_Str(SqlTypeEnum type){
        switch(type){
            // case CHAR:
            //     return "CHAR";
            // case NUMERIC:
                // return "NUMERIC";
            case SQL_INT:
                return "INTEGER";
            case SQL_DBL:
                return "REAL";
            // case BLOB:
            //     return "BLOB";
            case SQL_STR:
            default:
                return "TEXT";
        }
    }
    
    static std::string fieldSpecs(SqlTypeEnum type, int sz = 0, bool isKey = false, bool notNull = false){
        string ss;
        char szSpec[10];
        char keySpec[12];
        char nullSpec[10];

        if(sz > 0)
            sprintf(szSpec, " (%i)", sz);
        else
            sprintf(szSpec, " ");

        if(isKey)
            sprintf(keySpec, " PRIMARY KEY");
        else
            sprintf(keySpec, " ");
        
        if(notNull)
            sprintf(nullSpec, " NOT NULL");
        else
            sprintf(nullSpec, " ");
            
        ss 
            << SqlTypeEnum_To_Str(type) \
            << szSpec \
            << keySpec \
            << nullSpec;
        return ss;
    }

    static std::string ddlCat(SqlField field){
        string ss;
        ss << field.Name << " " << \
            fieldSpecs(field.Type, field.Sz, field.IsKey, field.NotNull);
        return ss;
    }

    static void dmlCat(
        string& partSql,
        vector<string>::iterator itrName, 
        vector<string>::iterator itrValue, 
        const vector<string>& names, 
        const vector<string>& values){
            string os;
            os
                << *itrName \
                << " = " \
                << *itrValue;
            partSql += os;

            ++itrName; ++itrValue;
            if(itrName != names.end())
            {
                partSql += ", ";
                dmlCat(partSql, itrName, itrValue, names, values);
            }            
        }

public:

    static std::string treatSqlValue(string value, SqlTypeEnum type){
        switch(type){
            case SQL_DBL:
            case SQL_INT:{
                string os;
                os << value;
                return os;
            }
            case SQL_STR:
            default:{
                string os;
                os << "'" << value << "'";
                return os;
            }
        }
    }


    static string TABLE_EXISTS(const char* tablename){
        string os;
        os
            << "SELECT name FROM sqlite_master WHERE type='table' and name=" \
            << "'" \
            << tablename \
            << "'";
        return os;
    }

    static std::string CREATE_TABLE(const char* tableName, vector<SqlField> sqlFields){        
        string os;
        std::string fieldsList;

        for(auto sqlf: sqlFields){
            size_t len = fieldsList.length();
            fieldsList += ((fieldsList.length() > 0) ? "," : "") + 
                ddlCat(sqlf);
        }
        
        os
            << "CREATE TABLE " \
            << tableName \
            << "( " \
            << fieldsList \
            << ");";
        return os;
    }
    
    static std::string INSERT_TABLE(const char* tableName, std::string names, std::string values){
            string ss;        
            ss
                << "INSERT INTO " \
                << tableName \
                << " (" \
                << names \
                << ") " \
                << "VALUES(" \
                << values \
                << ")";

            return ss;
    }

    static std::string INSERT_TABLE(
        const char* tableName, 
        vector<string> names, 
        vector<string> values){
            string strNames, strValues;
            join(names, ',', strNames);
            join(values, ',', strValues);
            return INSERT_TABLE(tableName, strNames, strValues);    
    }
    
    static std::string UPDATE_TABLE(
        const char* tableName, 
        const int& _id,
        vector<string> names, 
        vector<string> values){
        string os;
        string partSql;
        dmlCat(partSql, names.begin(), values.begin(), names, values);
        os 
            << "UPDATE " \
            << tableName << " "\
            << "SET " \
            << partSql << " " \
            << "WHERE _ID = " \
            << _id;
        return os;
    }
    
    static std::string SELECT(const char* tableName, int count){
        string os;
        std::string limit = "";
        if(count > 0)
            limit += " LIMIT " + to_string(count);
        os
            << "SELECT * FROM " \
            << tableName \
            << limit;
        return os;
    }

    static std::string DELETE(const char* tableName, const int& _id){
        string os;

        os
            << "DELETE FROM " \
            << tableName << " "\
            << "WHERE _ID = " \
            << _id;
        return os;
    }
};

#endif //SQLFACTORY_H
#ifndef SQLFACTORY_H
#define SQLFACTORY_H
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <utility>
#include <vector>

using namespace std;

enum SqlTypeEnum{
    TEXT,
    CHAR,
    NUMERIC,
    INTEGER,
    REAL,
    BLOB
};

struct SqlField{
    SqlField(
        const char* name, 
        const SqlTypeEnum& type = SqlTypeEnum::TEXT, 
        const int& sz = 0, 
        const bool& isKey = false, 
        const bool& notNull = false):
        Name(name), Type(type), Sz(sz), IsKey(isKey), NotNull(notNull){}
    const char* Name;
    SqlTypeEnum Type;
    int Sz;
    bool IsKey;
    bool NotNull;
};

struct SqlFieldCreatorFromTypeStr{
    static SqlField CreateField(            
        const char* name, 
        const char type = 'T', 
        const int& sz = 0, 
        const bool& isKey = false, 
        const bool& notNull = false
    ){
        SqlTypeEnum tp = ::TEXT;
        switch(type){
            case 'I':
                tp = ::INTEGER;
                break;
            case 'D':
                tp = ::REAL;
                break;
        }
        return SqlField(name, tp, sz, isKey, notNull);
    }
};

template<class T>
struct SqlColumn{
    SqlColumn(const char* name, const T& value):
        Name(name), Value(value){}

    const char* Name;
    T Value;
};
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
            case CHAR:
                return "CHAR";
            case NUMERIC:
                return "NUMERIC";
            case INTEGER:
                return "INTEGER";
            case REAL:
                return "REAL";
            case BLOB:
                return "BLOB";
            case TEXT:
            default:
                return "TEXT";
        }
    }
    
    template<class T>
    static std::string treatSqlValue(T value){
        ostringstream os;
        os << "'" << value << "'";
        return os.str();
    }
    static std::string fieldSpecs(SqlTypeEnum type, int sz = 0, bool isKey = false, bool notNull = false){
        std::stringstream ss;
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
            
        ss << SqlTypeEnum_To_Str(type) << szSpec << keySpec << nullSpec;
        return ss.str();
    }

    static std::string ddlCat(std::string built, SqlField sqlField){
        std::stringstream ss;
        std::string find = "); ";
        size_t start = built.find(find);
        if(start != std::string::npos)
            built.replace(start, find.length(), ", ");

        ss << built << sqlField.Name << " " << \
            fieldSpecs(sqlField.Type, sqlField.Sz, sqlField.IsKey, sqlField.NotNull) << "); " << endl;
        return ss.str();
    }

    template<typename T>
    static std::string ddlCat(std::string built,T last){
        std::stringstream ss;
        ss << built << last.Name << " " << \
            fieldSpecs(last.Type, last.Sz, last.IsKey, last.NotNull) << "); " << endl;
        return ss.str();
    }

    template<typename T, typename... TArgs>
    static std::string ddlCat(std::string built, T first, TArgs... sqlField){
        std::stringstream ss;
        ss << built << first.Name << " " << \
            fieldSpecs(first.Type, first.Sz, first.IsKey, first.NotNull) << ", ";
        return ddlCat(ss.str(), sqlField...);        
    }
    template<typename T>
    static pair<string, string> dmlCat(pair<string, string> built,T last){
        ostringstream names, values;
        names << built.first << last.Name;
        values << built.second << treatSqlValue(last.Value);
        return make_pair(names.str(), values.str());
    }

    template<typename T, typename... TArgs>
    static pair<string, string> dmlCat(pair<string, string> built, T first, TArgs... sqlField){
        ostringstream names, values;
        names << built.first << first.Name << ", ";
        values << built.second << treatSqlValue(first.Value) << ", ";
        return dmlCat(make_pair(names.str(), values.str()), sqlField...);        
    }
public:

    static string TABLE_EXISTS(const char* tablename){
        ostringstream os;
        os
            << "SELECT COUNT(type) FROM sqlite_master WHERE type='table' and name=" \
            << "'" \
            << tablename \
            << "'";
        return os.str();
    }

    template<class T>
    static pair<string, string> dmlCat(pair<string, string> built, SqlColumn<T> sqlCol){
        ostringstream names, values;
        names << built.first << sqlCol.Name << ", ";
        values << built.second << treatSqlValue(sqlCol.Value) << ", ";
        return make_pair(names, values);        
    }

    static std::string CREATE_TABLE(const char* tableName, vector<SqlField> sqlFields){        
        std::ostringstream os;
        std::string fieldsList;

        for(auto sqlf: sqlFields){
            fieldsList = ddlCat(fieldsList, sqlf);
        }

        os
            << "CREATE TABLE " \
            << tableName \
            << "( " \
            << fieldsList;
        return os.str();
    }

    template<typename... TArgs>
    static std::string CREATE_TABLE(const char* tableName, TArgs... sqlFields){        
        char buff[255];
        sprintf(buff, "CREATE TABLE %s(", tableName);

        std::string build = ddlCat(buff, sqlFields...);

        return build;
    }
    static std::string INSERT_TABLE(const char* tableName, std::string names, std::string values){
            stringstream ss;        
            ss
                << "INSERT INTO " \
                << tableName \
                << " (" \
                << names \
                << ") " \
                << "VALUES(" \
                << values \
                << ")";

            return ss.str();
    }
    static std::string INSERT_TABLE(const char* tableName, vector<string> names, vector<string> values){
        ostringstream os;    
        string strNames, strValues;
        join(names, ',', strNames);
        join(values, ',', strValues);
        return INSERT_TABLE(tableName, strNames, strValues);    
    }

    template<typename... TArgs>
    static std::string INSERT_TABLE(const char* tableName, TArgs... sqlColumns){              
        stringstream ss;
        pair<string, string> build = dmlCat(make_pair("",""), sqlColumns...);
        return INSERT_TABLE(tableName, build.first, build.second);
    }
    template<class T>
    static SqlField CreateFieldFromValues(const char* fieldName, T value){
        return SqlField(fieldName);
    }    
    static std::string SELECT(const char* tableName, int count){
        ostringstream os;
        std::string limit = "";
        if(count > 0)
            limit += " LIMIT " + std::to_string(count);
        os
            << "SELECT * FROM " \
            << tableName \
            << limit;
        return os.str();
    }
};

#endif //SQLFACTORY_H
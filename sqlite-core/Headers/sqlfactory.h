#ifndef SQLFACTORY_H
#define SQLFACTORY_H
#include <iostream>
#include <sstream>
#include <string>
#include <cstdio>
#include <utility>

using namespace std;

enum SqlTypeEnum{
    TEXT,
    CHAR,
    NUMERIC,
    INTEGER,
    REAL,
    BLOB
};
const char* SqlTypeEnum_To_Str(SqlTypeEnum type){
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

struct Field{
    Field(
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

template<class T>
struct Column{
    Column(const char* name, const T& value):
        Name(name), Value(value){}

    const char* Name;
    T Value;
};

class SqlFactory{
public:
    template<typename... TArgs>
    std::string CreateTable(const char* tableName, TArgs... fields){        
        char buff[255];
        sprintf(buff, "CREATE TABLE %s(", tableName);

        std::string build = ddlCat(buff, fields...);

        return build;
    }
    template<typename... TArgs>
    std::string InsertTable(const char* tableName, TArgs... columns){              
        stringstream ss;
        stringstream names, values;
        pair<string, string> build = dmlCat(make_pair("",""), columns...);
        ss
            << "INSERT INTO " \
            << tableName \
            << " (" \
            << build.first \
            << ") " \
            << "VALUES(" \
            << build.second \
            << ")"
            << endl;

        return ss.str();
    }
private:
    template<typename T>
    std::string ddlCat(std::string built,T last){
        std::stringstream ss;
        ss << built << last.Name << " " << \
            fieldSpecs(last.Type, last.Sz, last.IsKey, last.NotNull) << "); " << endl;
        return ss.str();
    }

    template<typename T, typename... TArgs>
    std::string ddlCat(std::string built, T first, TArgs... fields){
        std::stringstream ss;
        ss << built << first.Name << " " << \
            fieldSpecs(first.Type, first.Sz, first.IsKey, first.NotNull) << ", ";
        return ddlCat(ss.str(), fields...);        
    }

    template<typename T>
    pair<string, string> dmlCat(pair<string, string> built,T last){
        ostringstream names, values;
        names << built.first << last.Name;
        values << built.second << treatSqlValue(last.Value);
        return make_pair(names.str(), values.str());
    }

    template<typename T, typename... TArgs>
    pair<string, string> dmlCat(pair<string, string> built, T first, TArgs... fields){
        ostringstream names, values;
        names << built.first << first.Name << ", ";
        values << built.second << treatSqlValue(first.Value) << ", ";
        return dmlCat(make_pair(names.str(), values.str()), fields...);        
    }

    std::string fieldSpecs(SqlTypeEnum type, int sz = 0, bool isKey = false, bool notNull = false){
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
    template<class T>
    std::string treatSqlValue(T value){
        ostringstream os;
        os << "'" << value << "'";
        return os.str();
    }
    
};

#endif //SQLFACTORY_H
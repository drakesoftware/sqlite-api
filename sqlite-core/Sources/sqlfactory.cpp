#include "sqlfactory.h"
#include <vector>


std::string SqlFactory::treatSqlValue(string value){
    ostringstream os;
    os << "'" << value << "'";
    return os.str();
}
std::string SqlFactory::treatSqlValue(int value){
    ostringstream os;
    os << value;
    return os.str();
}
std::string SqlFactory::treatSqlValue(float value){
    ostringstream os;
    os << value;
    return os.str();
}
std::string SqlFactory::treatSqlValue(bool value){
    ostringstream os;
    const char* bl = value ? "1" : "0";
    os << bl;
    return os.str();
}


SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, string value){
    return SqlField(fieldName, ::TEXT);
}

SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, int value){
    return SqlField(fieldName, ::INTEGER);
}

SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, float value){
    return SqlField(fieldName, ::REAL);
}

SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, bool value){
    return SqlField(fieldName, ::INTEGER, 1);
}

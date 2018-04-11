#include "sqlfactory.h"
#include <vector>


template<>
std::string SqlFactory::treatSqlValue(int value){
    ostringstream os;
    os << value;
    return os.str();
}
template<>
std::string SqlFactory::treatSqlValue(float value){
    ostringstream os;
    os << value;
    return os.str();
}
template<>
std::string SqlFactory::treatSqlValue(bool value){
    ostringstream os;
    const char* bl = value ? "1" : "0";
    os << bl;
    return os.str();
}

template<>
SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, int value){
    return SqlField(fieldName, ::INTEGER);
}

template<>
SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, float value){
    return SqlField(fieldName, ::REAL);
}

template<>
SqlField SqlFactory::CreateFieldFromValues(const char* fieldName, bool value){
    return SqlField(fieldName, ::INTEGER, 1);
}

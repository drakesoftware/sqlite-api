#include "sqlfactory.h"

template<>
std::string SqlFactory::treatSqlValue(int value){
    ostringstream os;
    os << value;
    return os.str();
}
template<>
std::string SqlFactory::treatSqlValue(double value){
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
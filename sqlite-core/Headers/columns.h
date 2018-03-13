#ifndef COLUMNS_H
#define COLUMNS_H

#include <iostream>
#include <map>
#include <string>

using namespace std;
class Columns{
public:
    template<class ValueType>
    void set(const char* key, const ValueType value){

    }
    
    template<class ValueType>
    bool get(const char* key, ValueType& refVar){
        
    }
    

    std::map<const char*, int> m_intColumns;
    std::map<const char*, string> m_strColumns;
    std::map<const char*, bool> m_boolColumns;
    std::map<const char*, double> m_dblColumns;
};

#endif //COLUMNS_H
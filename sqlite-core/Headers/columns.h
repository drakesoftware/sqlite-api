#ifndef COLUMNS_H
#define COLUMNS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <tuple>


/**
 * This is a class that holds data for one row or one 
 * object at a time. It can hold data for only int, 
 * string, bool, float. But can be easily extended for 
 * more data types limited only by the ones supported 
 * by sqlite3. The specialized templated functions help 
 * seemlessly add values to the internal associative 
 * containers.
*/
using namespace std;
class Columns{
public:
    template<class ValueType>
    void set(const char* key, const ValueType value){

    }
    
    template<class ValueType>
    bool get(const char* key, ValueType& refVar){
        
    }
    
    /**
     * A helper function to pass data to classes that 
     * handle database operations. It holds names of 
     * the columns, there values in sql-context sensitive 
     * format and a char depicting data-type.
    */
    tuple<vector<string>, vector<string>, vector<char>> getNamesAndValues();

    void clear(){
        m_intColumns.clear();
        m_strColumns.clear();
        m_boolColumns.clear();
        m_dblColumns.clear();
    }

    std::map<const char*, int> m_intColumns;
    std::map<const char*, string> m_strColumns;
    std::map<const char*, bool> m_boolColumns;
    std::map<const char*, float> m_dblColumns;

};

#endif //COLUMNS_H
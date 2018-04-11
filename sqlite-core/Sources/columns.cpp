#include "columns.h"

template<>
bool Columns::get(const char* key, int& refVar){
    auto find = m_intColumns.find(key);
    if(find == m_intColumns.end()){
        return false;
    }
    refVar = m_intColumns[key];
    return true;
}
template<>
bool Columns::get(const char* key, float& refVar){
    auto find = m_dblColumns.find(key);
    if(find == m_dblColumns.end()){
        return false;
    }
    refVar = m_dblColumns[key];
    return true;
}
template<>
bool Columns::get(const char* key, string& refVar){
    auto find = m_strColumns.find(key);
    if(find == m_strColumns.end()){
        return false;
    }
    refVar = m_strColumns[key];
    return true;
}
template<>
bool Columns::get(const char* key, bool& refVar){
    auto find = m_strColumns.find(key);
    if(find == m_strColumns.end()){
        return false;
    }
    refVar = m_boolColumns[key];
    return true;
}

template<>
void Columns::set(const char* key, const int value){
    auto find = m_intColumns.find(key);
    if(find != m_intColumns.end())
        m_intColumns[key] = value;
    else
        m_intColumns.insert(std::make_pair(key, value));
}
template<>
void Columns::set(const char* key, const float value){
    auto find = m_dblColumns.find(key);
    if(find != m_dblColumns.end())
        m_dblColumns[key] = value;
    else
        m_dblColumns.insert(std::make_pair(key, value));
}
template<>
void Columns::set(const char* key, const char* value){
    auto find = m_strColumns.find(key);
    if(find != m_strColumns.end())
        m_strColumns[key] = value;
    else
        m_strColumns.insert(std::make_pair(key, value));
}
template<>
void Columns::set(const char* key, const bool value){
    auto find = m_boolColumns.find(key);
    if(find != m_boolColumns.end())
        m_boolColumns[key] = value;
    else
        m_boolColumns.insert(std::make_pair(key, value));
}

tuple<vector<string>, vector<string>, vector<char>> Columns::getNamesAndValues(){
    vector<string> names;
    vector<string> values;
    vector<char> types;

    for(auto pair: m_intColumns){
        names.push_back(pair.first);
        values.push_back(to_string(pair.second));
        types.push_back('I');
    }
    for(auto pair: m_strColumns){
        names.push_back(pair.first);
        values.push_back("'" + pair.second + "'");
        types.push_back('S');
    }
    for(auto pair: m_boolColumns){
        names.push_back(pair.first);
        string val = pair.second? "1": "0";
        values.push_back(val);
        types.push_back('I');
    }
    for(auto pair: m_dblColumns){
        names.push_back(pair.first);
        values.push_back(to_string(pair.second));
        types.push_back('D');
    }
    return make_tuple(names, values, types);
}
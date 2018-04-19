#include "columns.h"

bool Columns::get(int& refVar, const char* key) {
    auto find = m_intColumns.find(key);
    if(find == m_intColumns.end()){
        return false;
    }
    refVar = m_intColumns[key];
    return true;
}
bool Columns::get(float& refVar, const char* key) {
    auto find = m_dblColumns.find(key);
    if(find == m_dblColumns.end()){
        return false;
    }
    refVar = m_dblColumns[key];
    return true;
}
bool Columns::get(string& refVar, const char* key) {
    auto find = m_strColumns.find(key);
    if(find == m_strColumns.end()){
        return false;
    }
    refVar = m_strColumns[key];
    return true;
}
bool Columns::get(bool& refVar, const char* key) {
    auto find = m_boolColumns.find(key);
    if(find == m_boolColumns.end()){
        return false;
    }
    refVar = m_boolColumns[key];
    return true;
}

void Columns::set(const char* key, const int& value){
    auto find = m_intColumns.find(key);
    if(find != m_intColumns.end())
        m_intColumns[key] = value;
    else
        m_intColumns.insert(std::make_pair(key, value));
}
void Columns::set(const char* key, const float& value){
    auto find = m_dblColumns.find(key);
    if(find != m_dblColumns.end())
        m_dblColumns[key] = value;
    else
        m_dblColumns.insert(std::make_pair(key, value));
}
void Columns::set(const char* key, const string& value){
    auto find = m_strColumns.find(key);
    if(find != m_strColumns.end())
        m_strColumns[key] = value;
    else
        m_strColumns.insert(std::make_pair(key, value));
}
void Columns::set(const char* key, const bool& value){
    auto find = m_boolColumns.find(key);
    if(find != m_boolColumns.end())
        m_boolColumns[key] = value;
    else
        m_boolColumns.insert(std::make_pair(key, value));
}



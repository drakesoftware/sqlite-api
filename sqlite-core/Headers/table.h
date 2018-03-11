#ifndef TABEL_H
#define TABEL_H
#include <map>
#include <vector>
#include "field.h"
#include <memory>


class DB;
class Table{
public:
    Table() = default;
    Table(const char* tableName, std::shared_ptr<DB> db): 
    m_name{tableName}, m_db{db}{

    }

    template<
        class EntityType
    >
    EntityType Create(){
        return EntityType();
    }
    
    
private:
    const char* m_name;
    std::map<const char*, IField> m_fields;
    std::shared_ptr<DB> m_db;
};



#endif //TABEL_H
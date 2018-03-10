#ifndef DB_H
#define DB_H
#include <string>
#include <map>
#include "table.h"

class DB{
public:
    DB() = default;
    DB(const char* name);

    Table getTable(const char* tableName);

    bool dbTouched();
private:
    std::string m_name;
    bool m_dbTouched;
    std::map<const char*, Table> m_tables;
};

#endif //DB_H
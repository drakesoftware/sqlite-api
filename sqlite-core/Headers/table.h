#ifndef TABEL_H
#define TABEL_H
#include "db.h"
#include "columns.h"

class Table{
public:
    Table(const char* tableName, const char* dbName);    
    void save(const Columns& columns);
private:
    const char* m_name;
    DB m_db;
};



#endif //TABEL_H
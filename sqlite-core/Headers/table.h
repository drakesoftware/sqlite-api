#ifndef TABEL_H
#define TABEL_H
#include "db.h"
#include <utility>

/**
 * This class is a broker between the front end 
 * classes and the DB class that handles core db 
 * operations. This class gets data from fron-end 
 * classes, formats them using SqlFactory class 
 * and passes them to the DB class.
*/
class Table{
public:
    Table(const char* tableName, const char* dbName);   
    inline bool istableTouched() const {return m_tableTouched; }
    void create(const vector<SqlField>& fields);
    void save(const vector<SqlValue>& values);
    int update(const int& _id, const vector<SqlValue>& sqlValues);
    int get(sqlResult& results, int limit = 0) const;
    int remove(const int& _id);
private:
    bool exists() const;
    const char* m_name;
    DB m_db;
    bool m_tableTouched;
};



#endif //TABEL_H
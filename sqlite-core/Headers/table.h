#ifndef TABEL_H
#define TABEL_H
#include "db.h"
#include "columns.h"
#include <utility>

class SqlFieldCreatorFromTypeStr;
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
    inline bool istableTouched(){return m_tableTouched; }
    bool exists();
    void create(vector<string> names, vector<char> types);
    void save(vector<string> names, vector<string> values, vector<char> types);
    int get(sqlResult& results, int limit = 0);
private:
    const char* m_name;
    DB m_db;
    bool m_tableTouched;
};



#endif //TABEL_H
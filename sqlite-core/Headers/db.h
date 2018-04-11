#ifndef DB_H
#define DB_H
#include <sqlite3.h>
#include <memory>
#include <vector>
#include <string>

using namespace std;

struct sqlite3_deleter {
    void operator()(sqlite3* db) {
    sqlite3_close_v2(db);
    }
};
struct sqlite3_stmt_deleter{
    void operator()(sqlite3_stmt* stmt){
        sqlite3_finalize(stmt);
    }
};

enum SqlValueTypeEnum{
    INT,
    TXT,
    FLT,
    NUL
};
/**
 * This stucture holds value of one column coming 
 * from the sqlite3 db. They can be any type. While 
 * initializing the object, it is determined as to 
 * what data type the column is initialized as. 
 * This struct can be improved in many ways. To 
 * begin with, it could be converted into a union.
*/
struct SqlValue{
    SqlValueTypeEnum Tp;
    int IntVal = 0;
    const unsigned char* TxtVal = 0;
    float FltVal = 0;
    string Name;
    SqlValue(string name, int intVal):Name(name), IntVal(intVal), Tp(::INT){}
    SqlValue(string name, const unsigned char* charVal):Name(name), TxtVal(charVal), Tp(::TXT){}
    SqlValue(string name, float dblVal):Name(name), FltVal(dblVal), Tp(::FLT){}
    SqlValue(string name):Name(name), Tp(::NUL){}
};
using sqldb = std::unique_ptr<sqlite3, sqlite3_deleter>;
using sqlstmt = std::unique_ptr<sqlite3_stmt, sqlite3_stmt_deleter>;
using sqlResult = std::vector<std::vector<SqlValue> >;
/**
 * All database related operations are performed in this class only. 
*/
class DB{
public:
    DB(const char* name);
    static int callback(void *data, int argc, char **argv, char **azColName);
    /**
     * The create function returns a sqlite3 db held in a 
     * unique_ptr, ensuring safety from memory leakage.
    */
    sqldb create(); 
    /**
     * This function can be used for executing queries that 
     * are not run for any direct data. It can be used for 
     * Insert, Update, Delete. It returns if the operation 
     * executed successfully.
    */
    int execScalar(const char* sql);  
    /**
     * The function can be used to find how many rows of 
     * data exist for a query or if a record exists in 
     * the table at all. It can be helpful for queries 
     * where we need to check if table exists.
    */
    int selectCountScalar(const char* sql);  
    /**
     * The function helps getting one single value from 
     * any one column or the first column depending on the query.
    */
    int selectScalar(const char* sql, SqlValue& sqlValue);  
    /**
     * The function most helpful with select statements. 
     * It returns a vector of a vector of a SqlValue 
     * (representing one column). The return value returns 
     * the number of rows present in the table against the 
     * query.
    */
    int select(const char* sql, sqlResult& result);
private:
    const char* m_name;
};

#endif //DB_H
#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <map>
#include <string>
#include <db.h>

using std::string;


class DbManager{
protected:
    DbManager();
public:
    DB open(const char* dbName);    
    static DbManager instance();
private:    
    std::map<const char*, DB> m_dbs;
};

#endif //DBMANAGER_H
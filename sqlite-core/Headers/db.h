#ifndef DB_H
#define DB_H

class DB{
public:
    DB() = default;
    DB(const char* name, const char* tableName);   

    bool dbTouched();
private:
    const char* m_name;
    const char* m_tableName;
    bool m_dbTouched;
};

#endif //DB_H
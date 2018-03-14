#ifndef DB_H
#define DB_H
#define Using(what, body) { what; body; }
class DB{
public:
    DB(const char* name);  
    bool create(); 
    bool dbTouched();
private:
    const char* m_name;
    bool m_dbTouched;
};

#endif //DB_H
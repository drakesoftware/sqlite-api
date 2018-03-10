#ifndef TABEL_H
#define TABEL_H


class Table{
public:
    Table() = default;
    Table(const char* tableName);
private:
    const char* m_name;
    
};

#endif //TABEL_H
#ifndef TABEL_H
#define TABEL_H

class IEntity{};

template<
    class Domain
>
class CreateUATObjects{
    IEntity* Create();
};

template<
    template<
        class
    > 
    class CreationPoilicy
>
class Table: CreationPoilicy<IEntity>{
public:
    Table() = default;
    Table(const char* tableName);
private:
    const char* m_name;
};

#endif //TABEL_H
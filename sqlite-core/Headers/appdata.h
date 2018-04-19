#ifndef APPDATA_H
#define APPDATA_H

#include "entity.h"
using namespace std;

class   AppData: public Entity
{

private:
    int m_id = 0;
    int m_userId = 0;
    string m_key;
    string m_value;

public:
    AppData(const char* dbName, const char* tableName);
    void populate(const int userId, const string &key, const string &value);
    

    
    void setData() override;
    void reset(Columns col) override;

    int id() const;
    void setId(const int id);

    int userId()const;
    void setUserId(const int userId);

    string key()const;
    void setKey(const string &key);

    string value()const;
    void setValue(const string &value);

    vector<AppData> list();

    schema getschema() const override;
};

#endif // APPDATA_H

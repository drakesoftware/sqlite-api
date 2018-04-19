#ifndef APPDATADAO_H
#define APPDATADAO_H

#include <vector>
#include <memory>
#include <string>

using namespace std;


class AppData;
class QSqlDatabase;

class  AppDataDao
{
public:
    AppDataDao(const char* db, const char* table);
    void init() const;

    void addAppData(AppData &appData) const;
    void removeAppData(int id) const;
    void editAppData(int id, string key, string value) const;

    unique_ptr<vector<unique_ptr<AppData> > > appData() const;
    ~AppDataDao();
};

#endif // APPDATADAO_H

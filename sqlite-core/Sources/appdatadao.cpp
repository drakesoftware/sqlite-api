#include "appdatadao.h"
#include "appdata.h"

using namespace std;

AppDataDao::AppDataDao(const char* db, const char* table)
{
}

void AppDataDao::init() const
{
}

void AppDataDao::addAppData(AppData &appData) const
{
    appData.Save();
}

void AppDataDao::removeAppData(int id) const
{
}

void AppDataDao::editAppData(int id, QString column, QVariant value) const
{
}

unique_ptr<vector<unique_ptr<AppData>>> AppDataDao::appData() const
{
    
}


AppDataDao::~AppDataDao()
{

}

#include <iostream>
#include "dbmanager.h"
#include "db.h"
#include "field.h"


int main(){
    auto uatDb = DbManager::instance().open("UAT");
    Field<int> f = Field<int>("Age", 8);
    
    return 0;
}
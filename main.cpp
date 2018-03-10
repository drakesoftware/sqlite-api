#include <iostream>
#include "dbmanager.h"
#include "db.h"


int main(){
    auto uatDb = DbManager::instance().open("UAT");
    
    return 0;
}
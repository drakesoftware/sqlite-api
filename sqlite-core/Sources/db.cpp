#include "db.h"
#include <sqlite3.h>
#include <stdio.h>

DB::DB(const char* name): 
    m_name{name},
    m_dbTouched{false}{
}

bool DB::create(){
    sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   rc = sqlite3_open(m_name, &db);

   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }
   sqlite3_close(db);
}

bool DB::dbTouched(){
    return this->m_dbTouched;
}

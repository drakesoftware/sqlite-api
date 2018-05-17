#ifndef TABEL_H
#define TABEL_H
#include "db.h"
#include <utility>
#include <vector>
#include <memory>

class Filter;
/**
 * This class is a broker between the front end
 * classes and the DB class that handles core db
 * operations. This class gets data from fron-end
 * classes, formats them using SqlScriptHelper class
 * and passes them to the DB class.
*/
class Table
{
private:
  Table(const char *tableName, const DB &db);
  bool exists() const;
  const char *m_name;
  DB m_db;

public:
  Table(const char *tableName, const char *dbName);
  void create(const vector<DBField> &fields);
  int save(const vector<SqlUnit> &values);
  int update(const int &_id, const vector<SqlUnit> &sqlUnits);
  int get(sqlResult &results, int limit = 0) const;
  int get(sqlResult &results, const Filter &filter, int limit = 0) const;
  int get(sqlResult &results, const string &sqlFilter, int limit = 0) const;
  int remove(const int &_id);
  std::unique_ptr<Table> CreateLinked(const char *child) const;
};

#endif //TABEL_H
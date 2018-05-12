#include "table.h"
#include "sqlfactory.h"
#include "filter.h"
#include <cmath>

Table::Table(const char *tableName, const DB &db) : m_name{tableName}, m_db{db}
{
}

Table::Table(const char *tableName, const char *dbName) : m_name{tableName}, m_db{dbName} {}

bool Table::exists() const
{
    if (!m_db.istableTouched(m_name))
    {
        string sql = SqlFactory::TABLE_EXISTS(m_name);
        int count = m_db.selectCountScalar(sql.data());
        return count >= 0;
    }
    else
    {
        return false;
    }
}
void Table::create(const vector<DBField> &fields)
{
    if (!exists())
    {
        vector<DBField> newTable(fields);
        newTable.push_back(DBField("_ID", SQL_INT, true));
        auto sqlCreateTable = SqlFactory::CREATE_TABLE(m_name, newTable);
        if (m_db.execScalar(sqlCreateTable.data()))
        {
            m_db.tableTouched(m_name);
        }
    }
    else
    {
        m_db.tableTouched(m_name);
    }
}

int Table::save(const vector<SqlUnit> &sqlUnits)
{
    if (!m_db.istableTouched(m_name))
    {
        create(DBField::fromSqlUnits(sqlUnits));
    }
    vector<string> names, values;
    for (auto val : sqlUnits)
    {
        names.push_back(val.Name);
        values.push_back(SqlFactory::treatSqlUnit(val.toString(), val.Tp));
    }
    auto sqlInsertTable = SqlFactory::INSERT_TABLE(m_name, names, values);
    return m_db.execScalar(sqlInsertTable.data());
}

int Table::update(const int &_id, const vector<SqlUnit> &sqlUnits)
{
    if (exists())
    {
        vector<string> names, values;
        for (auto val : sqlUnits)
        {
            names.push_back(val.Name);
            values.push_back(SqlFactory::treatSqlUnit(val.toString(), val.Tp));
        }
        auto sqlUpdateTable = SqlFactory::UPDATE_TABLE(m_name, _id, names, values);
        m_db.execScalar(sqlUpdateTable.data());
    }
    else
    {
        return -1;
    }
}

int Table::get(sqlResult &results, int limit) const
{
    string selectSql = SqlFactory::SELECT(m_name, limit);
    return m_db.select(selectSql.data(), results);
}

int Table::get(sqlResult &results, const Filter &filter, int limit) const
{
    string selectSql = SqlFactory::SELECT(m_name, filter, limit);
    return m_db.select(selectSql.data(), results);
}

int Table::remove(const int &_id)
{
    if (exists())
    {
        string removeSql = SqlFactory::DELETE(m_name, _id);
        return m_db.execScalar(removeSql.data());
    }
    return -1;
}

std::unique_ptr<Table> Table::CreateLinked(const char *child) const
{
    char tableName[255];
    sprintf(tableName, "%s_%s", m_name, child);
    return std::unique_ptr<Table>(new Table(strdup(tableName), m_db));
}
#include "db.h"
#include <sqlite3.h>
#include <stdio.h>
#include <memory>
#include <string.h>
#include <algorithm>

tableJournal DB::g_tableJournal;
DB::DB(const char *name) : m_name{name}
{
}

void DB::tableTouched(const char *tableName)
{
    string strTableName = string(tableName);
    auto find = g_tableJournal.find(m_name);
    if (find == g_tableJournal.end())
    {
        g_tableJournal.insert(make_pair(m_name, std::vector<string>()));
    }
    auto tablesList = g_tableJournal[m_name];
    if (std::find(tablesList.begin(), tablesList.end(), strTableName) == tablesList.end())
    {
        tablesList.push_back(strTableName);
    }
}

bool DB::istableTouched(const char *tableName) const
{
    string strTableName = string(tableName);
    auto find = g_tableJournal.find(m_name);
    if (find != g_tableJournal.end())
    {
        auto tablesList = g_tableJournal[m_name];
        if (std::find(tablesList.begin(), tablesList.end(), strTableName) != tablesList.end())
        {
            return true;
        }
    }
    return false;
}

int DB::callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: ", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");
    return 0;
}
sqldb DB::create() const
{
    sqlite3 *db;
    int rc = sqlite3_open(m_name, &db);
    sqldb up_db{db};
    if (rc)
    {
        up_db.reset();
    }
    return up_db;
}
int DB::getLastRowID(const sqldb &db) const
{
    sqlite3_stmt *stmt;
    auto ret = sqlite3_prepare(db.get(), "SELECT last_insert_rowid()", -1, &stmt, NULL);

    sqlstmt up_stmt{stmt};

    if (sqlite3_step(up_stmt.get()) == SQLITE_ROW)
    {
        return sqlite3_column_int(up_stmt.get(), 0);
    }
    else
    {
        return 0;
    }
}

int DB::execScalar(const char *sql) const
{
    char *err_msg = 0;
    auto db = create();

    int rc = sqlite3_exec(db.get(), sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        fprintf(stdout, "query executed successfully\n");
    }
    auto rowID = getLastRowID(db);
    return rowID;
}

int DB::selectCountScalar(const char *sql) const
{
    auto db = create();
    sqlite3_stmt *stmt;
    sqlite3_prepare(db.get(), sql, -1, &stmt, NULL);
    sqlstmt up_stmt{stmt};
    int rows = -1;
    while (sqlite3_step(up_stmt.get()) == SQLITE_ROW)
    {
        rows++;
    }
    return rows;
}

int DB::selectScalar(const char *sql, SqlUnit &sqlUnit) const
{
    auto db = create();
    sqlite3_stmt *stmt;
    sqlite3_prepare(db.get(), sql, -1, &stmt, NULL);
    sqlstmt up_stmt{stmt};
    int rows = 0;
    if (sqlite3_step(up_stmt.get()) == SQLITE_ROW)
    {
        const char *colName = sqlite3_column_name(up_stmt.get(), 0);
        switch (sqlite3_column_type(up_stmt.get(), 0))
        {
        case SQLITE_INTEGER:
        {
            sqlUnit = SqlUnit(colName, sqlite3_column_int(up_stmt.get(), 0));
            break;
        }
        case SQLITE_FLOAT:
        {
            sqlUnit = SqlUnit(colName, (double)sqlite3_column_double(up_stmt.get(), 0));
            break;
        }
        case SQLITE_TEXT:
        {
            const unsigned char *ucharVal = sqlite3_column_text(up_stmt.get(), 0);
            string strVal = reinterpret_cast<const char *>(ucharVal);
            sqlUnit = SqlUnit(colName, strVal);
            break;
        }
        //    case SQLITE_BLOB:    cout << "BLOB " << endl;
        //         cout << "Size of blob: " << sqlite3_column_bytes(up_stmt.get(), col) << endl;
        //         struct tm *blobRetreived;
        //         blobRetreived = (struct tm *) sqlite3_column_blob(up_stmt.get(), col);
        //         cout << "Year retrieved from blob: " << blobRetreived->tm_year+1900 << endl;
        //         break;
        case SQLITE_NULL:
        {
            sqlUnit = SqlUnit(colName);
            break;
        }
        default:
            break;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int DB::select(const char *sql, sqlResult &result) const
{
    auto db = create();

    result.clear();
    sqlite3_stmt *stmt;
    sqlite3_prepare(db.get(), sql, -1, &stmt, NULL);
    sqlstmt up_stmt{stmt};
    int rows = 0;
    while (sqlite3_step(up_stmt.get()) == SQLITE_ROW)
    {
        rows++;
        vector<SqlUnit> row;
        for (int col = 0; col < sqlite3_column_count(up_stmt.get()); col++)
        {
            const char *colName = strdup(sqlite3_column_name(up_stmt.get(), col));
            switch (sqlite3_column_type(up_stmt.get(), col))
            {
            case SQLITE_INTEGER:
            {
                row.push_back(SqlUnit(colName, sqlite3_column_int(up_stmt.get(), col)));
                break;
            }
            case SQLITE_FLOAT:
            {
                row.push_back(SqlUnit(colName, (double)sqlite3_column_double(up_stmt.get(), col)));
                break;
            }
            case SQLITE_TEXT:
            {
                const unsigned char *ucharVal = sqlite3_column_text(up_stmt.get(), col);
                string strVal(reinterpret_cast<const char *>(ucharVal));
                row.push_back(SqlUnit(colName, strVal));
                break;
            }
            //    case SQLITE_BLOB:    cout << "BLOB " << endl;
            //         cout << "Size of blob: " << sqlite3_column_bytes(up_stmt.get(), col) << endl;
            //         struct tm *blobRetreived;
            //         blobRetreived = (struct tm *) sqlite3_column_blob(up_stmt.get(), col);
            //         cout << "Year retrieved from blob: " << blobRetreived->tm_year+1900 << endl;
            //         break;
            case SQLITE_NULL:
            {
                row.push_back(SqlUnit(colName));
                break;
            }
            default:
                break;
            }
        }
        result.push_back(row);
    }
    return rows;
}

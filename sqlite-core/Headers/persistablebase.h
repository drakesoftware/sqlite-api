#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <utility>
#include <memory>
#include "columns.h"
#include "table.h"
#include "filter.h"

typedef std::unique_ptr<Columns> up_columns;
typedef std::unique_ptr<vector<up_columns>> up_vecols;

class Table;
/**
 * The bae class for any data type that we want to save
 * to database. The class can be decomposed further to
 * derive policies related to saving data and then
 * initializing user defined data objects from basic types.
*/
class PersistableBase : public Columns
{
private:
    void SaveObjectType(Columns &cols, std::vector<SqlUnit> &parentSqlUnits);
    void UpdateObjectType(Columns &cols, std::vector<SqlUnit> &parentSqlUnits);

public:
    PersistableBase(const char *dbName, const char *tableName);

    void Save();
    void Update();
    void Remove();

    template <class t>
    static vector<t> Select(t persistable)
    {
        vector<t> vec;
        unique_ptr<vector<unique_ptr<Columns>>> cols = persistable.all();
        for (const up_columns &col : *cols)
        {
            t item(persistable);
            item.onGetData(*col);
            PersistableBase &e = item;
            col->get(e.m__id, "_ID");
            e.onSetData();
            vec.push_back(item);
        }
        return vec;
    }

    template <class t>
    static std::unique_ptr<vector<std::unique_ptr<t>>> Select_UP(t persistable)
    {
        std::unique_ptr<vector<std::unique_ptr<t>>> up_vec(new vector<std::unique_ptr<t>>);
        up_vecols cols = persistable.all();
        for (const up_columns &col : *cols)
        {
            std::unique_ptr<t> item(new t(persistable));
            item->onGetData(*col);
            PersistableBase &e = *item;
            col->get(e.m__id, "_ID");
            e.onSetData();
            up_vec->push_back(std::move(item));
        }
        return up_vec;
    }

    template <class t>
    static vector<t> Select(t persistable, const Filter &filter)
    {
        vector<t> vec;
        up_vecols cols = persistable.select(filter);
        for (up_columns &col : *cols)
        {
            t item(persistable);
            item.onGetData(*col);
            PersistableBase &e = item;
            col->get(e.m__id, "_ID");
            e.onSetData();
            vec.push_back(item);
        }
        return vec;
    }

    template <class t>
    static vector<t> Select(t persistable, const string & sqlFilter)
    {
        vector<t> vec;
        up_vecols cols = persistable.select(sqlFilter);
        for (up_columns &col : *cols)
        {
            t item(persistable);
            item.onGetData(*col);
            PersistableBase &e = item;
            col->get(e.m__id, "_ID");
            e.onSetData();
            vec.push_back(item);
        }
        return vec;
    }

    template <class t>
    static std::unique_ptr<vector<std::unique_ptr<t>>> Select_UP(t persistable, const Filter &filter)
    {
        std::unique_ptr<vector<std::unique_ptr<t>>> up_vec(new vector<std::unique_ptr<t>>);
        up_vecols cols = persistable.select(filter);
        for (up_columns &col : *cols)
        {
            std::unique_ptr<t> item(new t(persistable));
            item->onGetData(*col);
            PersistableBase &e = *item;
            col->get(e.m__id, "_ID");
            e.onSetData();
            up_vec->push_back(std::move(item));
        }
        return up_vec;
    }

    template <class t>
    static t First(t persistable)
    {
        up_columns col = persistable.first();
        t item(persistable);
        item.onGetData(*col);
        item.onSetData();
        return item;
    }

    template <class t>
    static std::unique_ptr<t> First_UP(t persistable)
    {
        up_columns col = persistable.first();
        std::unique_ptr<t> item(new t(persistable));
        item->onGetData(*col);
        item->onSetData();
        return item;
    }

protected:
    PersistableBase(Table t) : m_table(t) {};
    virtual void onSetData() = 0;
    virtual void onGetData(Columns& col) = 0;

    void GetObjectType(const schema &schm, up_columns &cols) const;

    /**
     * Helper function that returns firstl value from db
    */
    up_columns first();
    /**
     * Helper function that returns last value from db
    */
    up_columns last();
    /**
     * Helper function that returns all values from db
    */
    up_vecols all();
    /**
     * Helper function that returns all values from db qualifying a filter
    */
    up_vecols select(const Filter &filter) const;
    /**
     * Helper function that returns all values from db qualifying a filter
    */
    up_vecols select(const string &sqlFilter) const;

private:
    int m__id;
    Table m_table;
};

#endif
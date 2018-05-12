#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <utility>
#include <memory>
#include "columns.h"
#include "table.h"
#include "filter.h"

using up_columns = std::unique_ptr<Columns>;
using up_vecols = std::unique_ptr<vector<up_columns> >;

class Table;
/**
 * The bae class for any data type that we want to save
 * to database. The class can be decomposed further to
 * derive policies related to saving data and then
 * initializing user defined data objects from basic types.
*/
class Entity: public Columns
{
  private:
    void SaveComposite(Columns &cols, std::vector<SqlUnit>& parentSqlUnits);
    void UpdateComposite(Columns &cols, std::vector<SqlUnit> &parentSqlUnits);
  public:
    Entity(const char* dbName, const char* tableName);

    void Save();
    void Update();
    void Remove();


    template<class t>
    static vector<t> Select(t entity)
    {
        vector<t> vec;
        auto cols = entity.all();
        for(auto& col: *cols)
        {
            t item(entity);
            item.reset(*col);
            Entity& e = item;
            col->get(e.m__id, "_ID");
            e.pushData();
            vec.push_back(item);
        }
        return vec;
    }

    template<class t>
    static vector<t> Select(t entity, const Filter& filter) 
    {
        vector<t> vec;
        auto cols = entity.select(filter);
        for(auto& col: *cols)
        {
            t item(entity);
            item.reset(*col);
            Entity& e = item;
            col->get(e.m__id, "_ID");
            e.pushData();
            vec.push_back(item);
        }
        return vec;
    }

    template<class t>
    static t First(t entity)
    {
        vector<t> vec;
        auto col = entity.first();
        auto item = t(entity);
        item.reset(*col);
        item.pushData();
        return item;
    }
    
  protected:
    Entity(Table t):m_table(t) {};
    virtual void pushData() = 0;
    virtual void reset(Columns col) = 0;

    void GetComposite(const schema& schm, up_columns& cols) const;

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
    up_vecols select(const Filter& filter) const;
  private:
    int m__id;
    Table m_table;
};


#endif
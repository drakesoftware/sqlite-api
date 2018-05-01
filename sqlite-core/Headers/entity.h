#ifndef ENTITY_H
#define ENTITY_H
#include <vector>
#include <utility>
#include "columns.h"
#include "table.h"
#include "filter.h"


class Table;
/**
 * The bae class for any data type that we want to save
 * to database. The class can be decomposed further to
 * derive policies related to saving data and then
 * initializing user defined data objects from basic types.
*/
class Entity: public Columns
{
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
        for(auto& col: cols)
        {
            auto item = t(entity);
            item.reset(col);
            Entity& e = item;
            col.get(e.m__id, "_ID");
            e.setData();
            vec.push_back(item);
        }
        return vec;
    }

    template<class t>
    static vector<t> Select(t entity, const Filter& filter) 
    {
        vector<t> vec;
        auto cols = entity.select(filter);
        for(auto& col: cols)
        {
            auto item = t(entity);
            item.reset(col);
            Entity& e = item;
            col.get(e.m__id, "_ID");
            e.setData();
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
        item.reset(col);
        item.setData();
        return item;
    }
    
  protected:
    Entity(Table t):m_table(t) {};
    virtual void setData() = 0;
    virtual void reset(Columns col) = 0;
    /**
     * Helper function that returns firstl value from db
    */
    Columns first();
    /**
     * Helper function that returns last value from db
    */
    Columns last();
    /**
     * Helper function that returns all values from db
    */
    std::vector<Columns> all();
    /**
     * Helper function that returns all values from db qualifying a filter
    */
    std::vector<Columns> select(const Filter& filter) const;
  private:
    int m__id;
    Table m_table;
};


#endif
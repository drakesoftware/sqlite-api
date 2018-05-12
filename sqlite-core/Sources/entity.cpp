#include "entity.h"
#include "databroker.h"

Entity::Entity(const char* dbName, const char* tableName)
    :m_table(tableName, dbName), m__id{0}
{
    
}

void Entity::Save()
{
    clear();
    setData();

    m_table.save(SqlValueFromColumn::create(*this));
}

void Entity::Update()
{
    clear();
    setData();

    m_table.update(m__id, SqlValueFromColumn::create(*this));
}

void Entity::Remove()
{
    if(m__id > 0)
    {
        m_table.remove(m__id);
    }
}

int Entity::id() const 
{
    return m__id;
}

Columns Entity::first()
{
    sqlResult results;
    int rows = m_table.get(results, 1);
    if(rows > 0)
    {
        return ColumnsFromSqlValue::create(getschema(), results[0]);
    }
    else
    {
        return Columns();
    }
}

std::vector<Columns> Entity::all()
{
    std::vector<Columns>  vec;
    sqlResult results;
    int rows = m_table.get(results);
    for(auto row: results)
    {
        Columns cols = ColumnsFromSqlValue::create(getschema(), row);
        vec.push_back(cols);
    }
    return vec;
}

std::vector<Columns> Entity::select(const Filter& filter) const
{
    std::vector<Columns>  vec;
    sqlResult results;
    int rows = m_table.get(results, filter);
    for(auto row: results)
    {
        Columns cols = ColumnsFromSqlValue::create(getschema(), row);
        vec.push_back(cols);
    }
    return vec;
}

Columns Entity::byId(const int& id) const
{   
    auto vecCols = select(apply_filter("_ID", id));
    if(vecCols.size() > 0)
    {
        return vecCols[0];
    }
    else
    {
        Columns();
    }
}
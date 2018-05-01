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
    schema sch = getSchema();
    for(auto& field: sch)
    {
        if(field.second == PLAT_OBJ)
        {
            //TODO: you have the entity, just save it!!
            // Entity& entity = static_cast<Entity>(getObject(field.first));            
            // entity.Save();
        }
    }
    m_table.save(SqlValueFromColumn::create(*this));
}

void Entity::Remove()
{
    if(m__id > 0)
    {
        m_table.remove(m__id);
    }
}

void Entity::Update()
{
    clear();
    setData();

    m_table.update(m__id, SqlValueFromColumn::create(*this));
}

Columns Entity::first()
{
    sqlResult results;
    int rows = m_table.get(results, 1);
    if(rows > 0)
    {
        return ColumnsFromSqlValue::create(getSchema(), results[0]);
    }
    else
    {
        return Columns();
    }
}

Columns Entity::last()
{
    return first();
}

std::vector<Columns> Entity::all()
{
    std::vector<Columns>  vec;
    sqlResult results;
    int rows = m_table.get(results);
    for(auto row: results)
    {        
        Columns cols = ColumnsFromSqlValue::create(getSchema(), row);
        schema sch = getSchema();
        for(auto& field: sch)
        {
            if(field.second == PLAT_OBJ)
            {
                // cols.getObject()
            }
        }
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
        Columns cols = ColumnsFromSqlValue::create(getSchema(), row);
        vec.push_back(cols);
    }
    return vec;
}
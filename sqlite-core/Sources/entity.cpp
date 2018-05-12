#include "entity.h"
#include "databroker.h"

Entity::Entity(const char *dbName, const char *tableName)
    : m_table(tableName, dbName), m__id{0}
{
}

void Entity::SaveComposite(Columns &cols, std::vector<SqlUnit> &parentSqlUnits)
{
    for (auto &col : cols.getSchema())
    {
        if (col.second == PLAT_CMPST)
        {
            const sp_columns& sp_cmpst = cols.getColumns(col.first);
            if (sp_cmpst != nullptr)
            {
                auto up_childTable = m_table.CreateLinked(col.first);
                auto sqlUnits = SqlUnitFromColumn::create(*sp_cmpst);
                auto id = up_childTable->save(sqlUnits);
                sp_cmpst->set("_ID", id);
                parentSqlUnits.push_back(SqlUnit(col.first, id));
            }
        }
    }
}
void Entity::UpdateComposite(Columns &cols, std::vector<SqlUnit> &parentSqlUnits)
{
    for (auto &col : cols.getSchema())
    {
        if (col.second == PLAT_CMPST)
        {
            const sp_columns& sp_cmpst = cols.getColumns(col.first);
            if (sp_cmpst != nullptr)
            {
                auto up_childTable = m_table.CreateLinked(col.first);
                auto sqlUnits = SqlUnitFromColumn::create(*sp_cmpst);
                int cmpstId = sp_cmpst->getInt("_ID");
                auto id = up_childTable->update(cmpstId, sqlUnits);
                parentSqlUnits.push_back(SqlUnit(col.first, id));
            }
        }
    }
}

void Entity::Save()
{
    clear();
    pushData();

    auto sqlUnits = SqlUnitFromColumn::create(*this);
    SaveComposite(*this, sqlUnits);

    m_table.save(sqlUnits);
}

void Entity::Remove()
{
    if (m__id > 0)
    {
        m_table.remove(m__id);
    }
}

void Entity::Update()
{
    clear();
    pushData();

    auto sqlUnits = SqlUnitFromColumn::create(*this);
    UpdateComposite(*this, sqlUnits);

    m_table.update(m__id, sqlUnits);
}
void Entity::GetComposite(const schema& schm, up_columns& cols) const
{
    for (auto &col : schm)
    {
        if (col.second == PLAT_CMPST)
        {
            auto up_childTable = m_table.CreateLinked(col.first);
            sqlResult results;
            int rows = up_childTable->get(results, 1);
            if (rows > 0)
            {
                cols->set(col.first, ColumnsFromSqlUnit::create(results[0]));
                // cols->remove(col.first, col.second);
            }
        }
    }
}

up_columns Entity::first()
{
    sqlResult results;
    int rows = m_table.get(results, 1);
    if (rows > 0)
    {
        auto cols = ColumnsFromSqlUnit::create(getSchema(), results[0]);
        GetComposite(getSchema(), cols);
        return cols;
    }
    else
    {
        return up_columns(nullptr);
    }
}

up_columns Entity::last()
{
    return first();
}

up_vecols Entity::all()
{
    up_vecols vec(new vector<up_columns>);
    sqlResult results;
    int rows = m_table.get(results);
    for (auto row : results)
    {
        up_columns cols = ColumnsFromSqlUnit::create(getSchema(), row);
        GetComposite(getSchema(), cols);
        vec->push_back(std::move(cols));
    }
    return vec;
}

up_vecols Entity::select(const Filter &filter) const
{
    up_vecols vec(new vector<up_columns>);
    sqlResult results;
    int rows = m_table.get(results, filter);
    for (auto row : results)
    {
        up_columns cols = ColumnsFromSqlUnit::create(getSchema(), row);
        GetComposite(getSchema(), cols);
        vec->push_back(std::move(cols));
    }
    return vec;
}
#include "persistablebase.h"
#include "databroker.h"

PersistableBase::PersistableBase(const char *dbName, const char *tableName)
    : m_table(tableName, dbName), m__id{0}
{
}

void PersistableBase::SaveObjectType(Columns &cols, std::vector<SqlUnit> &parentSqlUnits)
{
    for (const pair<const char *, PlatformTypeEnum> &col : cols.getSchema())
    {
        if (col.second == PLAT_CMPST)
        {
            const sp_columns &sp_cmpst = cols.getColumns(col.first);
            if (sp_cmpst != nullptr)
            {
                const unique_ptr<Table> &up_childTable = m_table.CreateLinked(col.first);
                const vector<SqlUnit> &sqlUnits = SqlUnitFromColumn::create(*sp_cmpst);
                int id = up_childTable->save(sqlUnits);
                sp_cmpst->set("_ID", id);
                parentSqlUnits.push_back(SqlUnit(col.first, id));
            }
        }
    }
}
void PersistableBase::UpdateObjectType(Columns &cols, std::vector<SqlUnit> &parentSqlUnits)
{
    for (const pair<const char *, PlatformTypeEnum> &col : cols.getSchema())
    {
        if (col.second == PLAT_CMPST)
        {
            const sp_columns &sp_cmpst = cols.getColumns(col.first);
            if (sp_cmpst != nullptr)
            {
                const unique_ptr<Table> &up_childTable = m_table.CreateLinked(col.first);
                const vector<SqlUnit> &sqlUnits = SqlUnitFromColumn::create(*sp_cmpst);
                int cmpstId = sp_cmpst->getInt("_ID");
                int id = up_childTable->update(cmpstId, sqlUnits);
                parentSqlUnits.push_back(SqlUnit(col.first, id));
            }
        }
    }
}

void PersistableBase::Save()
{
    clear();
    this->onSetData();

    vector<SqlUnit> &&sqlUnits = SqlUnitFromColumn::create(*this);
    SaveObjectType(*this, sqlUnits);

    m_table.save(sqlUnits);
}

void PersistableBase::Remove()
{
    if (m__id > 0)
    {
        m_table.remove(m__id);
    }
}

void PersistableBase::Update()
{
    clear();
    onSetData();

    vector<SqlUnit> &&sqlUnits = SqlUnitFromColumn::create(*this);
    UpdateObjectType(*this, sqlUnits);

    m_table.update(m__id, sqlUnits);
}
void PersistableBase::GetObjectType(const schema &schm, up_columns &cols) const
{
    for (const pair<const char *, PlatformTypeEnum> &col : schm)
    {
        if (col.second == PLAT_CMPST)
        {
            unique_ptr<Table> up_childTable = m_table.CreateLinked(col.first);
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

up_columns PersistableBase::first()
{
    sqlResult results;
    int rows = m_table.get(results, 1);
    if (rows > 0)
    {
        up_columns &&cols = ColumnsFromSqlUnit::create(getSchema(), results[0]);
        GetObjectType(getSchema(), cols);
        return move(cols);
    }
    else
    {
        return up_columns(nullptr);
    }
}

up_columns PersistableBase::last()
{
    return first();
}

up_vecols PersistableBase::all()
{
    up_vecols vec(new vector<up_columns>);
    sqlResult results;
    int rows = m_table.get(results);
    for (const vector<SqlUnit> &row : results)
    {
        up_columns cols = ColumnsFromSqlUnit::create(getSchema(), row);
        GetObjectType(getSchema(), cols);
        vec->push_back(std::move(cols));
    }
    return vec;
}

up_vecols PersistableBase::select(const Filter &filter) const
{
    up_vecols vec(new vector<up_columns>);
    sqlResult results;
    int rows = m_table.get(results, filter);
    for (const vector<SqlUnit> &row : results)
    {
        up_columns cols = ColumnsFromSqlUnit::create(getSchema(), row);
        GetObjectType(getSchema(), cols);
        vec->push_back(std::move(cols));
    }
    return vec;
}
up_vecols PersistableBase::select(const string &sqlFilter) const
{
    up_vecols vec(new vector<up_columns>);
    sqlResult results;
    int rows = m_table.get(results, sqlFilter);
    for (const vector<SqlUnit> &row : results)
    {
        up_columns cols = ColumnsFromSqlUnit::create(getSchema(), row);
        GetObjectType(getSchema(), cols);
        vec->push_back(std::move(cols));
    }
    return vec;
}
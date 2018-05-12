#ifndef DATABROKER_H
#define DATABROKER_H
#include "table.h"
#include "columns.h"
#include <vector>
#include <utility>


class ColumnsFromSqlUnit
{
  public:
    static mapSqlUnit mapFromVec(const vector<SqlUnit>& values)
    {
        mapSqlUnit valueMap;
        for(auto val: values)
        {
            valueMap.insert(std::make_pair(val.Name, val));
        }
        return valueMap;
    }

    static up_columns create(const schema& schema, const vector<SqlUnit>& values)
    {
        up_columns cols(new Columns);
        mapSqlUnit valueMap = mapFromVec(values);

        cols->set("_ID", valueMap["_ID"].IntVal);

        for(auto& pair: schema)
        {
            const char* name = pair.first;
            switch(pair.second)
            {
                case PLAT_STR:
                    cols->set(name, valueMap[name].TxtVal);
                    break;
                case PLAT_DBL:
                    cols->set(name, valueMap[name].FltVal);
                    break;
                case PLAT_INT:
                    cols->set(name, valueMap[name].IntVal);
                    break;
                case PLAT_BOOL:
                    cols->set(name, (bool)valueMap[name].IntVal);
                    break;
                case PLAT_CMPST:

                    break;
                default:
                    break;
            }
        }

        return cols;
    }
    static up_columns create(const vector<SqlUnit>& values)
    {
        up_columns cols(new Columns);


        for(auto& sqlUnit: values)
        {
            const char* name = sqlUnit.Name;
            if(strcmp(name, "_ID")== 0)
            {
                cols->set("_ID", sqlUnit.IntVal);
                continue;
            }
            switch(sqlUnit.Tp)
            {
                case SQL_STR:
                    cols->set(name, sqlUnit.TxtVal);
                    break;
                case SQL_DBL:
                    cols->set(name, sqlUnit.FltVal);
                    break;
                case SQL_INT:
                    cols->set(name, sqlUnit.IntVal);
                    break;
                default:
                    break;
            }
        }

        return cols;
    }
};

class SqlUnitFromColumn
{
  public:
    static vector<SqlUnit> create(Columns& cols)
    {
        vector<SqlUnit> sqlUnits;
        const schema &schema = cols.getSchema();

        for(auto pair: schema)
        {
            const char* name =  pair.first;
            switch(pair.second)
            {
                case PLAT_STR:
                {
                    string val = "";
                    cols.get(val, name);
                    sqlUnits.push_back(SqlUnit(name, val));
                    break;
                }
                case PLAT_DBL:
                {
                    double val = 0.0;
                    cols.get(val, name);
                    sqlUnits.push_back(SqlUnit(name, val));
                    break;
                }
                case PLAT_BOOL:
                {
                    bool val = false;
                    cols.get(val, name);
                    sqlUnits.push_back(SqlUnit(name, val));
                    break;
                }
                case PLAT_INT:
                {
                    int val = 0;
                    cols.get(val, name);
                    sqlUnits.push_back(SqlUnit(name, val));
                    break;
                }
                default:
                break;
            }
        }
        return sqlUnits;
    }
};
#endif //DATABROKER_H
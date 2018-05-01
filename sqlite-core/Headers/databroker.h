#ifndef DATABROKER_H
#define DATABROKER_H
#include "table.h"
#include "columns.h"
#include <vector>
#include <utility>


class ColumnsFromSqlValue
{
  public:
    static mapSqlValue mapFromVec(const vector<SqlValue>& values)
    {
        mapSqlValue valueMap;
        for(auto val: values)
        {
            valueMap.insert(std::make_pair(val.Name, val));
        }
        return valueMap;
    }

    static Columns create(const schema& schema, const vector<SqlValue>& values)
    {
        Columns cols;
        mapSqlValue valueMap = mapFromVec(values);

        cols.set("_ID", valueMap["_ID"].IntVal);

        for(auto pair: schema)
        {
            const char* name = pair.first;
            switch(pair.second)
            {
                case PLAT_STR:
                    cols.set(name, valueMap[name].TxtVal);
                    break;
                case PLAT_DBL:
                    cols.set(name, valueMap[name].FltVal);
                    break;
                case PLAT_INT:
                    cols.set(name, valueMap[name].IntVal);
                    break;
                case PLAT_BOOL:
                    cols.set(name, (bool)valueMap[name].IntVal);
                    break;
                case PLAT_OBJ:
                {
                    Columns cols;
                    cols.set("_ID", valueMap[name].IntVal);
                    cols.set(name, cols);
                    break;
                }
                default:
                    break;
            }
        }

        return cols;
    }
};
class SqlValueFromColumn
{
  public:
    static vector<SqlValue> create(Columns& cols)
    {
        vector<SqlValue> sqlValues;
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
                    sqlValues.push_back(SqlValue(name, val));
                    break;
                }
                case PLAT_DBL:
                {
                    float val = 0.0;
                    cols.get(val, name);
                    sqlValues.push_back(SqlValue(name, val));
                    break;
                }
                case PLAT_BOOL:
                {
                    bool val = false;
                    cols.get(val, name);
                    sqlValues.push_back(SqlValue(name, val));
                    break;
                }
                case PLAT_OBJ:
                {
                    Columns obj;
                    int _ID;
                    cols.get(obj, name);
                    cols.get(_ID, "_ID");
                    sqlValues.push_back(SqlValue(name, _ID));
                }
                case PLAT_INT:
                default:
                {
                    int val = 0;
                    cols.get(val, name);
                    sqlValues.push_back(SqlValue(name, val));
                    break;
                }
            }
        }
        return sqlValues;
    }
};
#endif //DATABROKER_H
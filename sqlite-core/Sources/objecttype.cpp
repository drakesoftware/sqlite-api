#include "objecttype.h"

ObjectType::ObjectType(const vector<PlatUnit> &columns)
{
    for (const PlatUnit &pUnit : columns)
    {
        mSchema.insert(make_pair(pUnit.Name, pUnit.Tp));
        switch (pUnit.Tp)
        {
        case PLAT_STR:
            set(pUnit.Name, pUnit.TxtVal);
            break;
        case PLAT_DBL:
            set(pUnit.Name, pUnit.DblVal);
            break;
        case PLAT_INT:
            set(pUnit.Name, pUnit.IntVal);
            break;
        case PLAT_BOOL:
            set(pUnit.Name, (int)pUnit.BoolVal);
            break;
        default:
            break;
        }
    }
}

schema ObjectType::getSchema() const
{
    return mSchema;
}
#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "columns.h"


class Composite:public Columns
{
private:
    schema mSchema;
public:
    Composite(const vector<PlatUnit>& columns);
    schema getSchema() const override;
    int Save(const char* table);
};


#endif //COMPOSITE_H
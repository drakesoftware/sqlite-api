#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "columns.h"

class ObjectType : public Columns
{
  private:
    schema mSchema;

  public:
    ObjectType(const vector<PlatUnit> &columns);
    schema getSchema() const override;
    int Save(const char *table);
};

#endif //COMPOSITE_H
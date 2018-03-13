#ifndef ENTITY_H
#define ENTITY_H
#include "table.h"
#include "columns.h"


class Entity: public Columns{
public:
    Entity() = default;
    void Save(){
        //Clear data here
        setData();
        //do the db save
    }
    virtual void setData(){};
protected:
    Columns first(){
        
    }
    Columns last(){
        return first();
    }
    std::vector<Columns> all(){
        std::vector<Columns>  vec;
        
        vec.push_back(first());
        return vec;
    }
private:
};


#endif
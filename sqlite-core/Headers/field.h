#ifndef FIELD_H
#define FIELD_H


template<
    class ValueType
>
class SqlToCppTypeConversion{
public:
    const char* Convert(ValueType type){
        return "String";
    };
};


template<
    class ValueType,
    template<class>
    class ValueTypeCoversionPolicy = SqlToCppTypeConversion
>
class Field: ValueTypeCoversionPolicy<ValueType>{
public:
    Field() = default;
    Field(const char* name, ValueType value){

    }
    
};



#endif
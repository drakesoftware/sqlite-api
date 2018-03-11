#ifndef FIELD_H
#define FIELD_H


template<
    class ValueType
>
class SqlToCppTypeConversion{
    const char* Convert(ValueType type){
        return "String";
    };
};

class IField{
public:
    void get(){};
};

template<
    class ValueType,
    template<class>
    class ValueTypeCoversionPolicy = SqlToCppTypeConversion
>
class Field: 
    public ValueTypeCoversionPolicy<ValueType>,
    public IField {
public:
    Field() = default;
    Field(const char* name):
        m_name{name}{

    }
    Field(const char* name, ValueType value):
        m_name{name},
        m_value{value}{
        
    }
    ValueType getValue(){
        return m_value;
    }
private:
    const char* m_name;
    ValueType m_value;
};



#endif
#ifndef FIELD_H
#define FIELD_H


template<
    class ValueType
>
class DefaultTypeConversion{
    const char* Convert(ValueType type){
        return "String";
    };
};

class IField{
public:
    IField() = default;
    IField(const char* name):
        m_name{name}{}

    template<class ConvertToType>
    ConvertToType get(){};
    const char* m_name;
    virtual ~IField() = default;
protected:
};

template<
    class ValueType,
    template<class>
    class ValueTypeCoversionPolicy = DefaultTypeConversion
>
class Field: 
    public ValueTypeCoversionPolicy<ValueType>,
    public IField {
public:
    Field() = default;
    Field(const char* name):
        IField(name){

    }
    Field(const char* name, ValueType value):
        IField(name),
        m_value{value}{
        
    }

    ValueType getValue(){
        return m_value;
    }
private:
    ValueType m_value;
};


class Setter{
public:
    template<class ValueType>
    static void refSet(ValueType& var, IField& ifield){
        Field<ValueType>& field = (Field<ValueType>&)ifield;
        var = field.getValue();
    }
};
#endif
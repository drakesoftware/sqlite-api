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
    void get(){};
    const char* m_name;
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



#endif
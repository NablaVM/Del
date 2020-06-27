#ifndef DEL_TYPES_VARIABLES_HPP
#define DEL_TYPES_VARIABLES_HPP

#include <string>

namespace DEL
{
    enum class ArithmeticOp
    {
        LSH, RSH, BW_OR, BW_AND, BW_XOR, AND, OR, NEGATE,      MOD,
        LTE, GTE, GT, LT, EQ, NE, BW_NOT, DIV, ADD, SUB, MUL,  POW 
    };

    enum class DataType
    {
        NIL,         // Value representing nothing
        NONE,        // No value
        INT,         // Integer
        DOUBLE,      // Double
        STRING,      // String
        ID_STRING,   // Data type is string representing identifier
        USER_DEFINED // User defined data type
    };

    class EncodedDataType
    {
    public:
        EncodedDataType(DataType type, std::string raw) : 
            dataType(type), raw(raw){}
            
        DataType dataType;
        std::string raw;
    };

    struct VariableNameTypePair
    {
        std::string name;
        DataType dataType;
    };


    class Parameter
    {
    public:
        Parameter(DataType type, std::string var, std::string obj_type, bool is_ref) : 
            dataType(type), var(var), obj_type(obj_type), is_ref(is_ref) {}
            
        DataType dataType;
        std::string var;
        std::string obj_type;
        bool is_ref;
    };


    static std::string DataType_to_string(DataType type)
    {
        switch(type)
        {
            case DataType::NONE:          return "NONE";
            case DataType::NIL:           return "NIL";
            case DataType::INT:           return "INT";
            case DataType::DOUBLE:        return "DOUBLE";
            case DataType::ID_STRING:     return "ID_STRING";
            case DataType::STRING:        return "STRING";
            case DataType::USER_DEFINED:  return "USER_DEFINED";
            default:                      return "ERROR";
        }
        return "ERROR";
    };
}

#endif
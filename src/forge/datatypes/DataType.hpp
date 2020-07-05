#ifndef FORGE_DATA_TYPE_HPP
#define FORGE_DATA_TYPE_HPP

#include <string>
#include <iostream>

namespace FORGE
{
    //! \brief Data types 
    enum class DataType
    {
        STANDARD_INTEGER     = 10,
        VAR_STANDARD_INTEGER = 11,
        REF_STANDARD_INTEGER = 12,

        STANDARD_DOUBLE      = 20,
        VAR_STANDARD_DOUBLE  = 21,
        REF_STANDARD_DOUBLE  = 22,

        STANDARD_CHAR        = 30,
        VAR_STANDARD_CHAR    = 31,
        REF_STANDARD_CHAR    = 32,

        STANDARD_STRING      = 40,
        VAR_STANDARD_STRING  = 41,
        REF_STANDARD_STRING  = 42,

        NIL                  = 50,
        UNDEFINED            = 51,
        UNKNOWN              = 52,
        REF_UNKNOWN          = 53
    };

    static std::string DataType_to_string(DataType type)
    {
        switch(type)
        {
            case DataType::STANDARD_INTEGER:        return "STANDARD_INTEGER";
            case DataType::STANDARD_DOUBLE:         return "STANDARD_DOUBLE";
            case DataType::STANDARD_CHAR:           return "STANDARD_CHAR";
            case DataType::STANDARD_STRING:         return "STANDARD_STRING";
            case DataType::REF_STANDARD_INTEGER:    return "REF_STANDARD_INTEGER";
            case DataType::REF_STANDARD_DOUBLE:     return "REF_STANDARD_DOUBLE";
            case DataType::REF_STANDARD_CHAR:       return "REF_STANDARD_CHAR";
            case DataType::REF_STANDARD_STRING:     return "REF_STANDARD_STRING";
            case DataType::VAR_STANDARD_INTEGER:    return "VAR_STANDARD_INTEGER";
            case DataType::VAR_STANDARD_DOUBLE:     return "VAR_STANDARD_DOUBLE";
            case DataType::VAR_STANDARD_CHAR:       return "VAR_STANDARD_CHAR";
            case DataType::VAR_STANDARD_STRING:     return "VAR_STANDARD_STRING";
            case DataType::NIL:                     return "NIL";
            case DataType::UNDEFINED:               return "UNDEFINED";
            case DataType::UNKNOWN:                 return "UNKNOWN";
            case DataType::REF_UNKNOWN:             return "REF_UNKNOWN";
        }
        return "UNDEFINED";
    }

    //! \brief Check if the values match within the same range for the ones that need to be compared
    static bool DataType_base_equal(DataType lhs, DataType rhs)
    {
        if((int)lhs < 20)                   { return((int)rhs < 20); }
        if((int)lhs >= 20 && (int)lhs < 30) { return((int)rhs >= 20 && (int)lhs < 30); }
        if((int)lhs >= 30 && (int)lhs < 40) { return((int)rhs >= 30 && (int)lhs < 40); }
        if((int)lhs >= 40 && (int)lhs < 50) { return((int)rhs >= 40 && (int)lhs < 50); }
        return false;
    }

    static bool DataType_is_variable(DataType type)
    {
        return (type == DataType::VAR_STANDARD_CHAR     ||
                type == DataType::VAR_STANDARD_DOUBLE   ||
                type == DataType::VAR_STANDARD_INTEGER  ||
                type == DataType::VAR_STANDARD_STRING   ||
                type == DataType::UNKNOWN);
    }

    static bool DataType_is_value(DataType type)
    {
        return (type == DataType::STANDARD_CHAR     ||
                type == DataType::STANDARD_DOUBLE   ||
                type == DataType::STANDARD_INTEGER  ||
                type == DataType::STANDARD_STRING );
    }

    static bool DataType_is_double(DataType type)
    {
        return((int)type >= 20 && (int)type < 30);
    }

}
#endif
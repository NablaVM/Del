#ifndef FORGE_DATA_TYPE_HPP
#define FORGE_DATA_TYPE_HPP

#include <string>

namespace FORGE
{
    //! \brief Data types 
    enum class DataType
    {
        STANDARD_INTEGER,
        STANDARD_DOUBLE,
        STANDARD_CHAR,
        STANDARD_STRING,
        REF_STANDARD_INTEGER,
        REF_STANDARD_DOUBLE,
        REF_STANDARD_CHAR,
        REF_STANDARD_STRING,
        NIL,
        UNDEFINED,
        UNKNOWN,
        REF_UNKNOWN
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
            case DataType::NIL:                     return "NIL";
            case DataType::UNDEFINED:               return "UNDEFINED";
            case DataType::UNKNOWN:                 return "UNKNOWN";
            case DataType::REF_UNKNOWN:             return "REF_UNKNOWN";
        }
        return "UNDEFINED";
    }

}
#endif
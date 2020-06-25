#ifndef DEL_TYPES_VARIABLES_HPP
#define DEL_TYPES_VARIABLES_HPP

#include <string>

namespace DEL
{
    enum class DataType
    {
        NONE,
        INT,
        DOUBLE,
        ID_STRING,
        USER_DEFINED
    };

    static std::string DataType_to_string(DataType type)
    {
        switch(type)
        {
            case DataType::NONE:          return "NONE";
            case DataType::INT:           return "INT";
            case DataType::DOUBLE:        return "DOUBLE";
            case DataType::ID_STRING:     return "ID_STRING";
            case DataType::USER_DEFINED:  return "USER_DEFINED";
            default:                      return "ERROR";
        }
        return "ERROR";
    };
}

#endif
#ifndef DEL_TYPES_VARIABLES_HPP
#define DEL_TYPES_VARIABLES_HPP

#include <string>
#include "forge/datatypes/DataType.hpp"

namespace DEL
{
    enum class ArithmeticOp
    {
        LSH, RSH, BW_OR, BW_AND, BW_XOR, AND, OR, NEGATE,      MOD,
        LTE, GTE, GT, LT, EQ, NE, BW_NOT, DIV, ADD, SUB, MUL,  POW 
    };

    //! \brief A data type with a string for representation
    class EncodedDataType
    {
    public:
        EncodedDataType(FORGE::DataType type, std::string raw) : 
            dataType(type), raw(raw){}
            
        FORGE::DataType dataType;
        std::string raw;
    };

}

#endif
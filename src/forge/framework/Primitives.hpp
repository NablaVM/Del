#ifndef FORGE_PRIMITIVE_TYPES_HPP
#define FORGE_PRIMITIVE_TYPES_HPP

#include "DataTypes.hpp"
#include "SystemSettings.hpp"
#include <stdint.h>

namespace FORGE
{
    //! \brief Primitive data types
    enum class PrimitiveTypes
    {
        CHAR,
        DOUBLE,
        INTEGER,
        LIST,
        REFERENCE,
    };

    //! \brief A base for "Primitive" data types
    class Primitive : public ForgeDataType
    {
    public:
        Datum(PrimitiveTypes type, uint8_t minimum_bytes) : 
            ForgeDataType(DataClassificiation::PRIMITIVE),
            type(type), minimum_bytes(minimum_bytes)
        {
            switch(type)
            {
                case PrimitiveTypes::CHAR:      type_name = "char";      break;
                case PrimitiveTypes::DOUBLE:    type_name = "double";    break;
                case PrimitiveTypes::INTEGER:   type_name = "integer";   break;
                case PrimitiveTypes::LIST:      type_name = "list";      break;
                case PrimitiveTypes::REFERENCE: type_name = "reference"; break;
                default:                        type_name = "undefined"; break;
            }
        }

    protected:
        PrimitiveTypes type;    // Type
        uint8_t minimum_bytes;  // Minimum number of bytes to hold item
    };

    //! \brief The Char type
    class Char : public Primitive
    {
    public:
        Char() : Primitive(PrimitiveTypes::CHAR, SETTINGS::SYSTEM_WORD_SIZE_BYTES) {}
    };

    //! \brief The Double type
    class Double : public Primitive
    {
    public:
        Double() : Primitive(PrimitiveTypes::DOUBLE, SETTINGS::SYSTEM_WORD_SIZE_BYTES) {}
    };

    //! \brief The Integer type
    class Integer : public Primitive
    {
    public:
        Integer() : Primitive(PrimitiveTypes::INTEGER, SETTINGS::SYSTEM_WORD_SIZE_BYTES) {}
    };

    //! \brief The List type
    class List : public Primitive
    {
    public:
        List() : Primitive(PrimitiveTypes::LIST, SETTINGS::SYSTEM_WORD_SIZE_BYTES) {}
    };

    //! \brief The Reference type
    class Reference : public Primitive
    {
    public:
        Reference() : Primitive(PrimitiveTypes::REFERENCE, SETTINGS::SYSTEM_WORD_SIZE_BYTES) {}
    };
}


#endif
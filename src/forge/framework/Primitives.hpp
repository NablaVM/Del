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
        NIL,
        UNDEFINED // For marking that given type was undefined, not an undefined type
    };

    static std::string PrimitiveType_to_string(PrimitiveTypes type)
    {
        switch(type)
        {
            case PrimitiveTypes::CHAR:      return "char";     
            case PrimitiveTypes::DOUBLE:    return "double";   
            case PrimitiveTypes::INTEGER:   return "integer";  
            case PrimitiveTypes::LIST:      return "list";     
            case PrimitiveTypes::REFERENCE: return "reference";
            case PrimitiveTypes::NIL:       return "nil";      
            case PrimitiveTypes::UNDEFINED: return "undefined";
            default:                        return "undefined";
        }
    }

    //! \brief A base for "Primitive" data types
    class Primitive : public ForgeDataType
    {
    public:
        Primitive(PrimitiveTypes type, uint8_t minimum_bytes) : 
            ForgeDataType(DataClassificiation::PRIMITIVE),
            type(type), minimum_bytes(minimum_bytes)
        {
            type_name = PrimitiveType_to_string(type);
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
        Reference(PrimitiveTypes referred_type) : Primitive(PrimitiveTypes::REFERENCE, SETTINGS::SYSTEM_WORD_SIZE_BYTES),
            referred_type(referred_type) {}
        PrimitiveTypes referred_type;
    };

    //! \brief The Nil type
    class Nil : public Primitive
    {
    public:
        Nil() : Primitive(PrimitiveTypes::NIL, SETTINGS::SYSTEM_WORD_SIZE_BYTES) {}
    };
}


#endif
#ifndef FORGE_INSTRUCTION_HPP
#define FORGE_INSTRUCTION_HPP

#include "Generator.hpp"

namespace FORGE
{
    enum class InstructionType
    {
        ASSIGN,
        REASSIGN,
        CALL,
        RETURN,
        FOR_LOOP,
        WHILE_LOOP,
        IF,
        ELIF
    };

    class InstructionIf : public Generator
    {
    public:
        InstructionIf(InstructionType type) : type(type){}

        InstructionType type;

        InstructionType getType() const { return type; }
    };
}


#endif
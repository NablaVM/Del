#ifndef FORGE_REASSIGNMENT_HPP
#define FORGE_REASSIGNMENT_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"

namespace FORGE
{
    class Reassignment : public InstructionIf
    {
    public:
        Reassignment(Variable * variable, Expression * expression) : InstructionIf(InstructionType::ASSIGN),
                                                                       variable(variable), expression(expression) {}

        Variable * variable;
        Expression * expression;

        virtual std::vector<std::string> generate_NASM() override;
    };
}


#endif
#ifndef FORGE_ASSIGNMENT_HPP
#define FORGE_ASSIGNMENT_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"

namespace FORGE
{
    //! \brief An assignment instruction
    class Assignment : public InstructionIf
    {
    public:

        //! \brief Create an assignment for a variable
        //! \param variable The variable information
        //! \param expression The expression and instructions to calculate for assignment
        Assignment(Variable * variable, Expression * expression) : InstructionIf(InstructionType::ASSIGN),
                                                                       variable(variable), expression(expression) {}

        Variable * variable;
        Expression * expression;

        virtual void generate_NASM(Codegen & code_gen) override;
    };
}


#endif
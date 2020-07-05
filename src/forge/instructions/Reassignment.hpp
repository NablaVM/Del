#ifndef FORGE_REASSIGNMENT_HPP
#define FORGE_REASSIGNMENT_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"

namespace FORGE
{
    //! \brief A reassignment
    class Reassignment : public InstructionIf
    {
    public:

        //! \brief Create an assignment for a variable
        //! \param variable The variable information
        //! \param expression The expression and instructions to calculate for reassignment
        Reassignment(Variable * variable, Expression * expression) : InstructionIf(InstructionType::REASSIGN),
                                                                       variable(variable), expression(expression) {}

        Variable * variable;
        Expression * expression;

        virtual void generate_NASM(Codegen & code_gen) override;
    };
}


#endif
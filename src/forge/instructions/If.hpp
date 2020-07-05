#ifndef FORGE_IF_STATEMENT_HPP
#define FORGE_IF_STATEMENT_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"
#include "Aggregator.hpp"

namespace FORGE
{
    //! \brief An IF instruction
    class If : public InstructionIf, public Aggregator
    {
    public:

        //! \brief Create an IF instruction
        If(Expression * expression) : InstructionIf(InstructionType::IF), expression(expression) {}

        // Expression for entering into block
        Expression * expression;

        virtual void generate_NASM(Codegen & code_gen) override;
    };

    //! \brief An ELIF instruction
    class Elif : public InstructionIf, public Aggregator
    {
    public:

        //! \brief Create an ELIF instruction
        Elif(Expression * expression) : InstructionIf(InstructionType::ELIF), expression(expression) {}

        // Expression for entering into block
        Expression * expression;

        virtual void generate_NASM(Codegen & code_gen) override;
    };
}


#endif
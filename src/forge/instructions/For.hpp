#ifndef FORGE_FOR_STATEMENT_HPP
#define FORGE_FOR_STATEMENT_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"
#include "Aggregator.hpp"

#include "Continuable.hpp"

namespace FORGE
{
    //! \brief A For instruction
    class For : public InstructionIf, public Aggregator, public Continuable
    {
    public:

        //! \brief Create a while instruction
        For(Expression * expression) : InstructionIf(InstructionType::FOR_LOOP), expression(expression) {}

        // Expression for entering into block
        Expression * expression;

        virtual std::vector<std::string> generate_NASM(SymbolTable & symbol_table) override;

        virtual void add_continue_statement() override;
    };
}

#endif
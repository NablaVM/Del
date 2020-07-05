#ifndef FORGE_WHILE_STATEMENT_HPP
#define FORGE_WHILE_STATEMENT_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"
#include "Aggregator.hpp"
#include "Continuable.hpp"
#include "Breakable.hpp"

namespace FORGE
{
    //! \brief A while instruction
    class While : public InstructionIf, public Aggregator, public Continuable, public Breakable
    {
    public:

        //! \brief Create a while instruction
        While(Expression * expression) : InstructionIf(InstructionType::WHILE_LOOP), expression(expression) {}

        // Expression for entering into block
        Expression * expression;

        virtual std::vector<std::string> generate_NASM(SymbolTable & symbol_table) override;

        virtual void add_continue_statement() override;

        virtual void add_break(std::string name) override;
    };
}

#endif
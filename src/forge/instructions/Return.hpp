#ifndef FORGE_RETURN_HPP
#define FORGE_RETURN_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"

namespace FORGE
{
    //! \brief A return instruction
    class Return : public InstructionIf
    {
    public:

        //! \brief Create a return instruction
        //! \param expression An expression to return. If expression is a nullptr, nothing will be returned
        Return(Expression * expression) : InstructionIf(InstructionType::RETURN), expression(expression) {}

        Expression * expression;

        virtual std::vector<std::string> generate_NASM(SymbolTable & symbol_table) override;
    };
}


#endif
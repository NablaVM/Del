#ifndef FORGE_CALL_HPP
#define FORGE_CALL_HPP

#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"
#include "constructs/Expression.hpp"

namespace FORGE
{
    //! \brief A call instruction
    class Call : public InstructionIf
    {
    public:

        //! \brief Create a call instruction
        Call(std::vector<Variable> params) : InstructionIf(InstructionType::CALL), params(params) {}

        std::vector<Variable> params;

        virtual std::vector<std::string> generate_NASM(SymbolTable & symbol_table) override;
    };
}


#endif
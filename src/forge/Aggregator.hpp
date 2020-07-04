#ifndef FORGE_AGGREGATOR_HPP
#define FORGE_AGGREGATOR_HPP

#include "instructions/Instruction.hpp"

namespace FORGE
{
    //! \brief An instruction aggregator 
    //!        Anything that needs to aggregate instructions inherits this object
    class Aggregator
    {
    public:

        //! \brief Add an instruction to the aggregator
        //! \param ins The instruction to add - Note this doesn't do a nullptr check
        void add_instruction(InstructionIf * ins)
        {
           instructions.push_back(ins);
        }

        //! \brief Get the instructions that have been aggregated
        //! \returns vector of instructionif pointers
        std::vector<InstructionIf*> get_instructions() const
        {
            return instructions;
        }
    protected:
        std::vector<InstructionIf*> instructions;
    };
}


#endif
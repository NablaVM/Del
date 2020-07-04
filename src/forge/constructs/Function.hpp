#ifndef FORGE_FUNCTION_HPP
#define FORGE_FUNCTION_HPP

#include "Generator.hpp"

#include "datatypes/DataType.hpp"
#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"

#include <vector>

namespace FORGE
{
    class Function : public Generator
    {
    public:
        Function(std::string name, DataType return_type) : name(name), return_type(return_type){}
        Function(std::string name, DataType return_type, std::vector<Variable*> parameters, std::vector<InstructionIf*> instructions) : 
            name(name), return_type(return_type), parameters(parameters), instructions(instructions){}
            
        std::string name;
        DataType return_type;
        std::vector<Variable*> parameters;
        std::vector<InstructionIf*> instructions;

        void add_instruction(InstructionIf * ins);

        virtual std::vector<std::string> generate_NASM() override;
    };
}


#endif
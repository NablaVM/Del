#ifndef FORGE_FUNCTION_HPP
#define FORGE_FUNCTION_HPP

#include "Generator.hpp"
#include "Aggregator.hpp"

#include "datatypes/DataType.hpp"
#include "constructs/Variable.hpp"
#include "instructions/Instruction.hpp"

#include <vector>

namespace FORGE
{
    //! \brief A function object
    //!        This object is a generator and an aggregator as it generates code and contains instructions
    class Function : public Generator, public Aggregator
    {
    public:

        //! \brief Create the function
        //! \param name The name of the function
        //! \param return_type The return type of the function
        Function(std::string name, DataType return_type) : name(name), return_type(return_type){}

        //! \brief Create the function
        //! \param name The name of the function
        //! \param return_type The return type of the function
        //! \param parameters The variables to be passed in to the function
        Function(std::string name, DataType return_type, std::vector<Variable*> parameters) : 
            name(name), return_type(return_type), parameters(parameters){}
            
        std::string name;
        DataType return_type;
        std::vector<Variable*> parameters;

        virtual std::vector<std::string> generate_NASM(SymbolTable & symbol_table) override;
    };
}


#endif
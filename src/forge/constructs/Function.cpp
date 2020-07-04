#include "Function.hpp"

namespace FORGE
{
    std::vector<std::string> Function::generate_NASM(SymbolTable & symbol_table)
    {
        std::vector<std::string> result;

        result.push_back("\n<" + name + ":\n");

        // create function <name: ... 
        // load parameters

        for(auto & ins : instructions)
        {
            std::vector<std::string> code = ins->generate_NASM(symbol_table);
            result.insert(result.end(), code.begin(), code.end());
        }

        // End of function
        result.push_back("\n>\n");

        return {};
    }
}
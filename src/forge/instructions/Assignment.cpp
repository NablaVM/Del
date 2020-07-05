#include "Assignment.hpp"

#include <iostream>

namespace FORGE
{
    std::vector<std::string> Assignment::generate_NASM(SymbolTable & symbol_table) 
    {
        std::cout << "FORGE::ASSIGN(" << variable->getName() << ")" << std::endl;

        //expression->dump();

        return {};
    }
}
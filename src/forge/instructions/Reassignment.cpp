#include "Reassignment.hpp"

#include <iostream> 

namespace FORGE
{
    std::vector<std::string> Reassignment::generate_NASM(SymbolTable & symbol_table) 
    {
        std::cout << "FORGE::ASSIGN(" << variable->getName() << ")" << std::endl;
        return {};
    }
}
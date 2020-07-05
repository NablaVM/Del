#include "For.hpp"

#include <iostream> 

namespace FORGE
{
    std::vector<std::string> For::generate_NASM(SymbolTable & symbol_table) 
    {
        std::cout << "FORGE::FOR" << std::endl;

        return {};
    }

    void For::add_continue_statement()
    {
        std::cout << "FORGE::FOR::CONTINUE" << std::endl;
    }
}
#include "While.hpp"

#include <iostream> 

namespace FORGE
{
    std::vector<std::string> While::generate_NASM(SymbolTable & symbol_table) 
    {
        std::cout << "FORGE::WHILE" << std::endl;

        return {};
    }

    void While::add_continue_statement()
    {
        std::cout << "FORGE::WHILE::CONTINUE" << std::endl;
    }

    void While::add_break(std::string name)
    {
        std::cout << "FORGE::WHILE::BREAK(" << name << ")" << std::endl;
    }
}
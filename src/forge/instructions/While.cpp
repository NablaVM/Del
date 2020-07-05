#include "While.hpp"

#include <iostream> 

namespace FORGE
{
    void While::generate_NASM(Codegen & code_gen) 
    {
        std::cout << "FORGE::WHILE" << std::endl;

        return;
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
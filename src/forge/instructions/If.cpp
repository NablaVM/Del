#include "If.hpp"

#include <iostream>

namespace FORGE
{
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    std::vector<std::string> If::generate_NASM(SymbolTable & symbol_table)
    {
        std::cout << "FORGE::IF" << std::endl;
        return {};
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    std::vector<std::string> Elif::generate_NASM(SymbolTable & symbol_table)
    {
        std::cout << "FORGE::ELIF" << std::endl;
        return {};
    }
}
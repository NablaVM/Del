#include "If.hpp"

#include <iostream>

namespace FORGE
{
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void If::generate_NASM(Codegen & code_gen)
    {
        std::cout << "FORGE::IF" << std::endl;

        return;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Elif::generate_NASM(Codegen & code_gen)
    {
        std::cout << "FORGE::ELIF" << std::endl;
        return;
    }
}
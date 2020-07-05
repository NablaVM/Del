#include "Return.hpp"

#include <iostream> 

namespace FORGE
{
    void Return::generate_NASM(Codegen & code_gen) 
    {
        std::cout << "FORGE::RETURN" << std::endl;

        return;
    }
}
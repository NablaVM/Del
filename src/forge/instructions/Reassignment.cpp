#include "Reassignment.hpp"

#include <iostream> 

namespace FORGE
{
    void Reassignment::generate_NASM(Codegen & code_gen) 
    {
        std::cout << "FORGE::ASSIGN(" << variable->getName() << ")" << std::endl;
        return;
    }
}
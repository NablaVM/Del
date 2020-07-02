#ifndef FORGE_CODE_STRUCTURES_HPP
#define FORGE_CODE_STRUCTURES_HPP

#include "Primitives.hpp"

namespace FORGE
{
    struct FunctionParam
    {
        std::string symbol; 
        Primitive * type_info;
    };

}


#endif
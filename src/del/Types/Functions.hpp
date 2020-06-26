#ifndef DEL_TYPES_FUNCTIONS_HPP
#define DEL_TYPES_FUNCTIONS_HPP

#include "Types/Variables.hpp"

#include <string>
#include <vector>

namespace DEL
{
    struct FunctionRepresentation
    {
        std::vector<VariableNameTypePair> parameters;
        DataType returnType;
        std::string functionName;
    };
}

#endif
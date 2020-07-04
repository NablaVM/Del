#ifndef FORGE_VARIABLE_HPP
#define FORGE_VARIABLE_HPP

#include "datatypes/DataType.hpp"
#include <string>

namespace FORGE
{
    class Variable
    {
    public:
        Variable(std::string name, DataType type) : name(name), type(type) {}

        std::string  getName() const { return name; }
        DataType     getType() const { return type; }

        std::string name;
        DataType type;
    };
}


#endif
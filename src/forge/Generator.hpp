#ifndef FORGE_GENERATOR_HPP
#define FORGE_GENERATOR_HPP

#include <string>
#include <vector>

namespace FORGE
{
    class Generator
    {
    public:
        virtual std::vector<std::string> generate_NASM() = 0;
    };
}

#endif
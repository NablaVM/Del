#ifndef FORGE_BREAK_HPP
#define FORGE_BREAK_HPP

namespace FORGE
{
    class Breakable
    {
    public:
        virtual void add_break(std::string name_to_break) = 0;
    };
}

#endif
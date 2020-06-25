#include "Elements.hpp"

namespace DEL
{
    void Function::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Assignment::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }
}
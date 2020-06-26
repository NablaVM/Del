#include "Elements.hpp"

namespace DEL
{
    void Function::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Return::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Assignment::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Reassignment::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void ObjectAssignment::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void ObjectReassignment::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void ObjectMember::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Object::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }
}
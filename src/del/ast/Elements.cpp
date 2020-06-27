#include "Elements.hpp"

namespace DEL
{
    void Function::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Call::visit(Visitor &visitor)
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

    void If::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void WhileLoop::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void ForLoop::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void NamedLoop::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Continue::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }

    void Break::visit(Visitor &visitor)
    {
        visitor.accept(*this);
    }
}
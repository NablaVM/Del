#ifndef DEL_AST_ELEMENTS_HPP
#define DEL_AST_ELEMENTS_HPP

#include "Ast.hpp"
#include "Types/Variables.hpp"

#include <vector>

namespace DEL
{
    //! \brief A forward for the visitor class
    class Visitor;

    //! \brief An element 
    class Element 
    {
    public:
        Element(){}
        Element(int line_number) : line_number(line_number){}

        virtual ~Element() = default;
        virtual void visit(Visitor &visitor) = 0;

        int line_number;
    };

    //! \brief List of elements
    typedef std::vector<Element*> ElementList;

    // ===========--------------------- Elements ---------------------===========

    //! \brief Function element
    class Function : public Element
    {
    public:
        Function(std::string name, std::vector<std::string> params, ElementList elements, EncodedDataType * return_type, int line_number) : 
            Element(line_number), params(params), name(name), elements(elements), return_type(return_type) {}

        ~Function() { delete return_type; }

        virtual void visit(Visitor &visit) override;

        std::vector<std::string> params;
        std::string name;
        ElementList elements;
        EncodedDataType * return_type;
    };

    //! \brief Return statement
    class Return : public Element
    {
    public:
        Return(Ast * ast_expression, int line_number) : 
            Element(line_number), ast(ast_expression) {}

        ~Return(){ delete ast; }

        virtual void visit(Visitor &visit) override;

        Ast * ast;
    };

    //! \brief Assignment of a variable
    class Assignment : public Element
    {
    public:
        Assignment(bool immutable, Ast * ast, EncodedDataType* type_info, int line_number) : 
            Element(line_number), is_immutable(immutable), ast(ast), type_info(type_info) {}

        ~Assignment(){ delete ast; delete type_info; }

        virtual void visit(Visitor &visit) override;

        bool is_immutable;
        Ast * ast;
        EncodedDataType * type_info;
    };

    //! \brief Reassignment of a variable
    class Reassignment : public Element
    {
    public:
        Reassignment(Ast * ast, int line_number) : 
            Element(line_number), ast(ast) { }

        virtual void visit(Visitor &visit) override;
        Ast * ast;
    };

    //! \brief Assignment of an object
    class ObjectAssignment : public Element 
    {
    public:
        ObjectAssignment(bool immutable, std::string name, EncodedDataType * type, ElementList elements, int line_number) : 
            Element(line_number), is_immutable(immutable), name(name), type_info(type), elements(elements) {}

        virtual void visit(Visitor &visit) override;

        bool is_immutable;
        std::string name;
        EncodedDataType * type_info;
        ElementList elements;
    };

    //! \brief ObjectReassignment of an object
    class ObjectReassignment : public Element
    {
    public:
        ObjectReassignment(std::string var, ElementList elements, int line_number) : 
            Element(line_number), var(var), elements(elements) { }

        virtual void visit(Visitor &visit) override;
        std::string var;
        ElementList elements;
    };

    //! \brief Object member definition
    class ObjectMember : public Element 
    {
    public:
        ObjectMember(EncodedDataType * type, std::string name, int line_numner) :
            Element(line_numner), type_info(type), name(name){}

        ~ObjectMember(){ delete type_info; }

        virtual void visit(Visitor &visit) override;

        EncodedDataType * type_info;
        std::string name;
    };

    //! \brief Object definition
    class Object : public Element 
    {
    public:
        Object(std::string name, ElementList pub, ElementList priv, int line_number) : 
            Element(line_number), name(name), pub(pub), priv(priv) {}

        virtual void visit(Visitor &visit) override;

        std::string name;
        ElementList pub;
        ElementList priv;
    };

    //! \brief A visitor that takes in elements on accept
    class Visitor
    {
    public:
        virtual void accept(Function   &stmt) = 0;
        virtual void accept(Return     &stmt) = 0;
        virtual void accept(Assignment &stmt) = 0;
        virtual void accept(Reassignment &stmt) = 0;
        virtual void accept(ObjectAssignment &stmt) = 0;
        virtual void accept(ObjectReassignment &stmt) = 0;
        virtual void accept(ObjectMember &stmt) = 0;
        virtual void accept(Object     &stmt) = 0;
    };
}

#endif 
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
        Function(std::string name, std::vector<std::string> params, ElementList elements, int line_number) : 
            Element(line_number), params(params), name(name), elements(elements){}

        virtual void visit(Visitor &visit) override;

        std::vector<std::string> params;
        std::string name;
        ElementList elements;
    };

    //! \brief Assignment of a variable
    class Assignment : public Element
    {
    public:
        Assignment(Ast * ast, EncodedDataType* type_info, int line_number) : 
            Element(line_number), ast(ast), type_info(type_info) {}

        ~Assignment(){ delete ast; }

        virtual void visit(Visitor &visit) override;

        Ast * ast;
        EncodedDataType * type_info;
    };


    class Visitor
    {
    public:
        virtual void accept(Function   &stmt) = 0;
        virtual void accept(Assignment &stmt) = 0;
    };
}

#endif 
#ifndef DEL_AST_ELEMENTS_HPP
#define DEL_AST_ELEMENTS_HPP

#include "Ast.hpp"
#include "Types/Variables.hpp"

#include "forge/constructs/Variable.hpp"

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

    //-----------------------------------------------------------------------
    //
    //                          FUNCTION RELATED ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    //! \brief Function element
    class Function : public Element
    {
    public:
        Function(std::string name, std::vector<FORGE::Variable*> params, ElementList elements, EncodedDataType * return_type, int line_number) : 
            Element(line_number), params(params), name(name), elements(elements), return_type(return_type) {}

        ~Function() { if(return_type){ delete return_type; } }

        virtual void visit(Visitor &visit) override;

        std::vector<FORGE::Variable*> params;
        std::string name;
        ElementList elements;
        EncodedDataType * return_type;
    };

    //! \param Call element
    //  Calls need to inherit AST as well as they can be placed into an expression
    //  or used as an element.
    class Call : public Element, public Ast
    {
    public:
        Call(std::string function_name, std::vector<FORGE::Variable*> params, int line_number) : 
            Element(line_number), 
            Ast(NodeType::CALL, nullptr, nullptr),
            function_name(function_name), params(params){}

        virtual void visit(Visitor &visit) override;

        std::string function_name;
        std::vector<FORGE::Variable*> params;
    };

    //! \brief Return statement
    class Return : public Element
    {
    public:
        Return(Ast * ast_expression, int line_number) : 
            Element(line_number), ast(ast_expression) {}

        virtual void visit(Visitor &visit) override;

        Ast * ast;
    };

    //-----------------------------------------------------------------------
    //
    //                          ASSIGNMENT ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    //! \brief Assignment of a variable
    class Assignment : public Element
    {
    public:
        Assignment(bool immutable, Ast * ast, EncodedDataType* type_info, int line_number) : 
            Element(line_number), is_immutable(immutable), ast(ast), type_info(type_info) {}

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

    //-----------------------------------------------------------------------
    //
    //                          CONDITIONAL ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    //! \brief If Statement
    class If : public Element
    {
    public:
        enum class Type
        {
            IF,
            ELIF
        };

        If(If::Type type, Ast * ast, ElementList elements, Element * trail, int line_number) : 
            Element(line_number), type(type), ast(ast), elements(elements), trail(trail)
        {}

        ~If(){ if(trail) { delete trail; } }

        virtual void visit(Visitor &visit) override;

        If::Type type;          // Type of if
        Ast * ast;              // Expression
        ElementList elements;   // Statements inside the if
        Element * trail;        // 
    };

    //-----------------------------------------------------------------------
    //
    //                          LOOP ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    //! \brief While
    class WhileLoop  : public Element
    {
    public:
        WhileLoop(Ast * ast, ElementList elements, int line_number) : 
            Element(line_number), ast(ast), elements(elements) {}
            
        virtual void visit(Visitor &visit) override;

        Ast * ast;
        ElementList elements;
    };

    //! \brief ForLoop
    class ForLoop  : public Element
    {
    public:
        ForLoop(Element * loop_var, Ast * condition, Element* step, ElementList elements, int line_number) : 
            Element(line_number), loop_var(loop_var), condition(condition), step(step), elements(elements) {}

        ~ForLoop(){ if(loop_var){ delete loop_var; }
                    if(condition){delete condition;}
                    if(step){ delete step; } }

        virtual void visit(Visitor &visit) override;

        Element * loop_var;
        Ast * condition;
        Element * step;
        ElementList elements;
    };

    //! \brief Named Loop
    class NamedLoop : public Element
    {
    public:
        NamedLoop(std::string name, ElementList elements, int line_number) : 
            Element(line_number), name(name), elements(elements) {}

        virtual void visit(Visitor &visit) override;

        std::string name;
        ElementList elements;
    };

    //! \brief Continue Statement
    class Continue : public Element
    {
    public:
        Continue(int line_number) : Element(line_number){}

        virtual void visit(Visitor &visit) override;
    };

    //! \brief Break Statement
    class Break : public Element
    {
    public:
        Break(std::string name, int line_number) : 
            Element(line_number), name(name){}

        virtual void visit(Visitor &visit) override;

        std::string name;
    };

    //-----------------------------------------------------------------------
    //
    //                          VISITOR SETUP
    //
    //-----------------------------------------------------------------------
    
    //! \brief A visitor that takes in elements on accept
    class Visitor
    {
    public:
        virtual void accept(Function   &stmt) = 0;
        virtual void accept(Call       &stmt) = 0;
        virtual void accept(Return     &stmt) = 0;
        
        virtual void accept(Assignment &stmt) = 0;
        virtual void accept(Reassignment &stmt) = 0;

        virtual void accept(If         &stmt) = 0;

        virtual void accept(WhileLoop  &stmt) = 0;
        virtual void accept(ForLoop    &stmt) = 0;
        virtual void accept(NamedLoop  &stmt) = 0;
        virtual void accept(Continue   &stmt) = 0;
        virtual void accept(Break      &stmt) = 0;
    };
}

#endif 
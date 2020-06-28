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

    //-----------------------------------------------------------------------
    //
    //                          FUNCTION RELATED ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    //! \brief Function element
    class Function : public Element
    {
    public:
        Function(std::string name, std::vector<Parameter*> params, ElementList elements, EncodedDataType * return_type, int line_number) : 
            Element(line_number), params(params), name(name), elements(elements), return_type(return_type) {}

        ~Function() { if(return_type){ delete return_type; } }

        virtual void visit(Visitor &visit) override;

        std::vector<Parameter*> params;
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
        Call(std::string function_name, std::vector<Parameter*> params, int line_number) : 
            Element(line_number), 
            Ast(NodeType::CALL, nullptr, nullptr),
            function_name(function_name), params(params){}

        virtual void visit(Visitor &visit) override;

        std::string function_name;
        std::vector<Parameter*> params;
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
            ELIF,
            ELSE
        };

        If(If::Type type, Ast * ast, ElementList elements, Element * trail, int line_number) : 
            Element(line_number), type(type), elements(elements), trail(trail)
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
    //                          OBJECT ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    //! \brief Assignment of an object
    class ObjectAssignment : public Element 
    {
    public:
        ObjectAssignment(bool immutable, std::string name, EncodedDataType * type, ElementList elements, int line_number) : 
            Element(line_number), is_immutable(immutable), name(name), type_info(type), elements(elements) {}

        ~ObjectAssignment(){ if(type_info){ delete type_info; } }

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

        ~ObjectMember(){ if(type_info) { delete type_info; } }

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

    //-----------------------------------------------------------------------
    //
    //                          DYN ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    class Dyn
    {
    public:
        enum class CommandVariant
        {
            CREATE, DELETE, CLEAR,
            EXPAND, INSERT, APPEND, 
            GET,    SIZE,   FRONT,
            BACK
        };

        Dyn(CommandVariant variant, std::string var_name) : 
            variant(variant), var_name(var_name){}

        CommandVariant variant;
        std::string var_name;
    };

    //! \brief Dyn Create 
    class DynCreate : public Element, public Dyn 
    {
    public:
        DynCreate(std::string name, EncodedDataType * type, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::CREATE, name),
            type(type){}

        ~DynCreate(){ delete type; }

        virtual void visit(Visitor &visit) override;

        EncodedDataType * type;
    };

    //! \brief Dyn Expand 
    class DynExpand : public Element, public Dyn 
    {
    public:
        DynExpand(std::string name, std::string amount, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::EXPAND, name),
            amount(amount){}

        virtual void visit(Visitor &visit) override;

        std::string amount;
    };

    //! \brief Dyn Insert 
    class DynInsert : public Element, public Dyn 
    {
    public:
        DynInsert(std::string name, std::string idx, Ast * ast, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::INSERT, name),
            idx(idx),
            ast(ast){}

        virtual void visit(Visitor &visit) override;

        std::string idx;
        Ast * ast;
    };

    //! \brief Dyn Append 
    class DynAppend : public Element, public Dyn 
    {
    public:
        DynAppend(std::string name, Ast * ast, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::APPEND, name),
            ast(ast){}

        virtual void visit(Visitor &visit) override;

        Ast * ast;
    };

    //! \brief Dyn Clear 
    class DynClear : public Element, public Dyn 
    {
    public:
        DynClear(std::string name, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::CLEAR, name){}

        virtual void visit(Visitor &visit) override;
    };

    //! \brief Dyn Delete 
    class DynDelete : public Element, public Dyn 
    {
    public:
        DynDelete(std::string name, std::string idx, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::DELETE, name),
            idx(idx) {}

        virtual void visit(Visitor &visit) override;

        std::string idx;
    };

    //! \brief Dyn Get 
    class DynGet : public Element, public Dyn 
    {
    public:
        DynGet(std::string name, std::string idx, std::string dest, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::GET, name),
            idx(idx),
            dest(dest) {}

        virtual void visit(Visitor &visit) override;

        std::string idx;
        std::string dest;
    };

    //! \brief Dyn Size 
    class DynSize : public Element, public Dyn 
    {
    public:
        DynSize(std::string name, std::string dest, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::SIZE, name),
            dest(dest) {}

        virtual void visit(Visitor &visit) override;

        std::string dest;
    };

    //! \brief Dyn Front 
    class DynFront : public Element, public Dyn 
    {
    public:
        DynFront(std::string name, std::string dest, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::FRONT, name),
            dest(dest) {}

        virtual void visit(Visitor &visit) override;

        std::string dest;
    };

    //! \brief Dyn Back 
    class DynBack : public Element, public Dyn 
    {
    public:
        DynBack(std::string name, std::string dest, int line_number) :
            Element(line_number),
            Dyn(Dyn::CommandVariant::FRONT, name),
            dest(dest) {}

        virtual void visit(Visitor &visit) override;

        std::string dest;
    };

    //-----------------------------------------------------------------------
    //
    //                          UNIT ELEMENTS
    //
    //-----------------------------------------------------------------------
    
    class UnitSpace : public Element
    {
    public:
        UnitSpace(std::string name, ElementList elements, int line_number) : 
            Element(line_number), name(name), elements(elements) {}

        virtual void visit(Visitor &visit) override;

        std::string name;
        ElementList elements;
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

        virtual void accept(ObjectAssignment &stmt) = 0;
        virtual void accept(ObjectReassignment &stmt) = 0;
        virtual void accept(ObjectMember &stmt) = 0;
        virtual void accept(Object     &stmt) = 0;

        virtual void accept(WhileLoop  &stmt) = 0;
        virtual void accept(ForLoop    &stmt) = 0;
        virtual void accept(NamedLoop  &stmt) = 0;
        virtual void accept(Continue   &stmt) = 0;
        virtual void accept(Break      &stmt) = 0;

        virtual void accept(DynCreate  &stmt) = 0;
        virtual void accept(DynExpand  &stmt) = 0;
        virtual void accept(DynInsert  &stmt) = 0;
        virtual void accept(DynAppend  &stmt) = 0;
        virtual void accept(DynClear   &stmt) = 0;
        virtual void accept(DynDelete  &stmt) = 0;

        virtual void accept(DynGet     &stmt) = 0;
        virtual void accept(DynSize    &stmt) = 0;
        virtual void accept(DynFront   &stmt) = 0;
        virtual void accept(DynBack    &stmt) = 0;

        virtual void accept(UnitSpace &stmt) = 0;
    };
}

#endif 
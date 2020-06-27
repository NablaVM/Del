#ifndef DEL_ANALYZER_HPP
#define DEL_ANALYZER_HPP

#include "Ast/Elements.hpp"

namespace DEL
{
    //! \brief The statement analyzer
    class Analyzer : public Visitor
    {
    public:

        //! \brief Construct
        Analyzer();

        //! \brief Deconstruct
        ~Analyzer();

        // From Elements/Visitor
        virtual void accept(Function & stmt) override;
        virtual void accept(Call & stmt) override;
        virtual void accept(Return   & stmt) override;
        virtual void accept(Assignment & stmt) override;
        virtual void accept(Reassignment & stmt) override;
        virtual void accept(ObjectAssignment & stmt) override;
        virtual void accept(ObjectReassignment & stmt) override;
        virtual void accept(ObjectMember & stmt) override;
        virtual void accept(Object & stmt) override;
        virtual void accept(If & stmt) override;
        virtual void accept(WhileLoop  &stmt) override;
        virtual void accept(ForLoop    &stmt) override;
        virtual void accept(NamedLoop  &stmt) override;
        virtual void accept(Continue   &stmt) override;
        virtual void accept(Break      &stmt) override;

    private:


    };
}

#endif
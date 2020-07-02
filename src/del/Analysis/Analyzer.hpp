#ifndef DEL_ANALYZER_HPP
#define DEL_ANALYZER_HPP

#include "Ast/Elements.hpp"

namespace DEL
{
    class DEL_Driver;

    //! \brief The statement analyzer
    class Analyzer : public Visitor
    {
    public:

        //! \brief Construct
        Analyzer(DEL_Driver & driver);

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


        virtual void accept(DynCreate  &stmt) override;
        virtual void accept(DynExpand  &stmt) override;
        virtual void accept(DynInsert  &stmt) override;
        virtual void accept(DynAppend  &stmt) override;
        virtual void accept(DynClear   &stmt) override;
        virtual void accept(DynDelete  &stmt) override;

        virtual void accept(DynGet     &stmt) override;
        virtual void accept(DynSize    &stmt) override;
        virtual void accept(DynFront   &stmt) override;
        virtual void accept(DynBack    &stmt) override;

        virtual void accept(UnitSpace &stmt) override;

    private:

        DEL_Driver & driver;

        struct program_watcher
        {
            bool has_main;
        };

        void report_incomplete(std::string what);
    };
}

#endif
#ifndef DEL_ANALYZER_HPP
#define DEL_ANALYZER_HPP

#include "Ast/Elements.hpp"
#include "forge/constructs/Function.hpp"
#include "forge/constructs/Expression.hpp"

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
        virtual void accept(If & stmt) override;
        virtual void accept(WhileLoop  &stmt) override;
        virtual void accept(ForLoop    &stmt) override;
        virtual void accept(NamedLoop  &stmt) override;
        virtual void accept(Continue   &stmt) override;
        virtual void accept(Break      &stmt) override;


    private:

        DEL_Driver & driver;

        struct ProgramRequirements
        {
            bool has_main;
        };
        ProgramRequirements program_watcher;

        struct FunctionRequirements
        {
            bool has_return;
        };
        FunctionRequirements function_watcher;

        FORGE::Aggregator * current_forge_aggregator;
        Function *        current_front_function;

        std::vector<FORGE::Expression::ExpressionItem> forge_expression_items;

        void report_incomplete(std::string what);


        void ensure_id_in_current_context(std::string id, int line_no, std::vector<FORGE::DataType> allowed);

        void validate_call(Call & stmt);

        void validate_and_build_assignment(std::string var_name, Ast * ast, FORGE::DataType type, int line_number);
    };
}

#endif
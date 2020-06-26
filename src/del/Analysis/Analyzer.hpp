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
        virtual void accept(Assignment & stmt) override;

    private:


    };
}

#endif
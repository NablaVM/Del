#include "Analysis/Analyzer.hpp"

#include <iostream>

/*
    Right now we want to just accept things and make sure that stuff is getting through from the grammar. 
    Once everything comes in, THEN develop the emitter. 

    Not before. 


*/

namespace DEL
{
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    Analyzer::Analyzer()
    {
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    Analyzer::~Analyzer()
    {

    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Function & stmt) 
    {
        for(auto & el : stmt.elements)
        {
            // Visit Element
            el->visit(*this);

            // Delete element
            delete el;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Assignment & stmt) 
    {
        std::cout << "Analyzer::accept(Assignment & stmt)" << std::endl;
        std::cout << "\t Var  : " << stmt.ast->left->node.data << std::endl;
        std::cout << "\t Type : " << stmt.type_info->raw << std::endl;
    }
}
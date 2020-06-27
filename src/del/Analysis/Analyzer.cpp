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
        std::cout << "Analyzer::accept(Function & stmt)" << std::endl 
                  << "\t Name    : " << stmt.name << std::endl 
                  << "\t Params  : " << stmt.params.size() << std::endl
                  << "\t Returns : " << stmt.return_type->raw << std::endl;
                
        // This way we dont show the begin/ end bars if there are no params
        if(stmt.params.size() > 0)
        {
            std::cout << "-------------------- PARAMS --------------------" << std::endl;

            for(auto & p : stmt.params)
            {
                std::cout << "PARAM >>>" << std::endl;
                std::cout << "\t Type : " << DataType_to_string(p->dataType) << std::endl;
                std::cout << "\t Name : " << p->var << std::endl;
                std::cout << "\t Obj  : " << p->obj_type << std::endl; 
                std::cout << "\t Ref? : " << p->is_ref << std::endl;
            }

            std::cout << "------------------ END PARAMS ------------------" << std::endl;
        }

        for(auto & el : stmt.elements)
        {
            // Visit Element
            el->visit(*this);
            delete el;
        }

    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Call & stmt) 
    {
        std::cout << "Analyzer::accept(Call & stmt)" << std::endl;
        std::cout << "\t Func   : " << stmt.function_name << std::endl;
        std::cout << "\t Params : " << stmt.params.size() << std::endl;
               
        // This way we dont show the begin/ end bars if there are no params
        if(stmt.params.size() > 0)
        {
            std::cout << "-------------------- PARAMS --------------------" << std::endl;

            for(auto & p : stmt.params)
            {
                std::cout << "PARAM >>>" << std::endl;
                std::cout << "\t Type : " << DataType_to_string(p->dataType) << std::endl;
                std::cout << "\t Name : " << p->var << std::endl;
                std::cout << "\t Obj  : " << p->obj_type << std::endl; 
                std::cout << "\t Ref? : " << p->is_ref << std::endl;
            }

            std::cout << "------------------ END PARAMS ------------------" << std::endl;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Return & stmt) 
    {
        bool has_return = (stmt.ast != nullptr);

        std::cout << "Analyzer::accept(Return & stmt)" << std::endl 
                  << "\t Returns something?  : " << has_return << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Assignment & stmt) 
    {
        std::cout << "Analyzer::accept(Assignment & stmt)" << std::endl;
        std::cout << "\t Var       : " << stmt.ast->left->node.data << std::endl;
        std::cout << "\t Type      : " << stmt.type_info->raw << std::endl;
        std::cout << "\t Immutable : " << stmt.is_immutable << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Reassignment & stmt) 
    {
        std::cout << "Analyzer::accept(Reassignment & stmt)" << std::endl;
        std::cout << "\t Var       : " << stmt.ast->left->node.data << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ObjectAssignment & stmt) 
    {
        std::cout << "Analyzer::accept(ObjectAssignment & stmt)" << std::endl;

        std::cout << "-------------------- ASSIGN --------------------" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }

        std::cout << "------------------ END ASSIGN ------------------" << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ObjectReassignment & stmt) 
    {
        std::cout << "Analyzer::accept(ObjectAssignment & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var << std::endl;

        std::cout << "-------------------- ASSIGN --------------------" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }

        std::cout << "------------------ END ASSIGN ------------------" << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ObjectMember & stmt) 
    {
        std::cout << "Analyzer::accept(ObjectMember & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.name << std::endl; 
        std::cout << "\t Type : " << stmt.type_info->raw << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Object & stmt) 
    {
        std::cout << "Analyzer::accept(Object & stmt)" << std::endl;

        std::cout << "-------------------- PUBLIC --------------------" << std::endl;

        for(auto & pub : stmt.pub)
        {
            pub->visit(*this);
            delete pub;
        }

        std::cout << "-------------------- PRIVATE -------------------" << std::endl;

        for(auto & priv : stmt.priv)
        {
            priv->visit(*this);
            delete priv;
        }

        std::cout << "-------------------- END OBJ -------------------" << std::endl;
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(If & stmt) 
    {
        std::cout << "Analyzer::accept(If & stmt)" << std::endl;

        switch(stmt.type)
        {
        case DEL::If::Type::IF:   std::cout << "\t Type: IF  " << std::endl; break;
        case DEL::If::Type::ELIF: std::cout << "\t Type: ELIF" << std::endl; break;
        case DEL::If::Type::ELSE: std::cout << "\t Type: ELSE" << std::endl; break;
        }

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }

        if(stmt.trail)
        {
            std::cout << "\t" << "---> Following trail" << std::endl;

            stmt.trail->visit(*this);
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(WhileLoop  &stmt)
    {
        std::cout << "Analyzer::accept(WhileLoop & stmt)" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ForLoop    &stmt)
    {
        std::cout << "Analyzer::accept(ForLoop & stmt)" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(NamedLoop  &stmt)
    {
        std::cout << "Analyzer::accept(NamedLoop & stmt)" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Continue   &stmt)
    {
        std::cout << "Analyzer::accept(Continue & stmt)" << std::endl;
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Break      &stmt)
    {
        std::cout << "Analyzer::accept(Break & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.name << std::endl; 
    }
}

#include "Analysis/Analyzer.hpp"

#include "del_driver.hpp"

#include <iostream>

namespace DEL
{
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    Analyzer::Analyzer(DEL_Driver & driver) : driver(driver)
    {
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    Analyzer::~Analyzer()
    {

    }

    void Analyzer::report_incomplete(std::string what)
    {
        driver.code_forge.get_reporter().issue_report(
            new FORGE::InternalReport(
                {
                    "DEL::Analyzer",
                    "Analyzer.cpp",
                    "report_incomplete",
                    {
                        "The following has been detected by analyzer but is not yet complete:",
                        ("\t" + what) 
                    }
                }
            )
        );
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(UnitSpace  &stmt)
    {
        // Check if the unit context exists
        if(driver.symbol_table.does_context_exist(stmt.name))
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    stmt.line_number, 27, "Duplicate context name (" + stmt.name + ") detected", {"Rename unit to be unique"}
                )
            );
        }

        // Create the new context
        driver.symbol_table.new_context(stmt.name);

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }

        // We dont clear the unit contexts ! 
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Function & stmt) 
    {
        // Ensure function is a unique context
        if(driver.symbol_table.does_context_exist(stmt.name))
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    stmt.line_number, 27, "Duplicate context name (" + stmt.name + ") detected", {"Rename function to be unique"}
                )
            );
        }

        // Create a new context
        driver.symbol_table.new_context(stmt.name);

        /*


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
*/
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Call & stmt) 
    {
        /*
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
        */
       report_incomplete("Call");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Return & stmt) 
    {
        /*
        bool has_return = (stmt.ast != nullptr);

        std::cout << "Analyzer::accept(Return & stmt)" << std::endl 
                  << "\t Returns something?  : " << has_return << std::endl;
        */
       report_incomplete("Return");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Assignment & stmt) 
    {
        /*
        std::cout << "Analyzer::accept(Assignment & stmt)" << std::endl;
        std::cout << "\t Var       : " << stmt.ast->left->node.data << std::endl;
        std::cout << "\t Type      : " << stmt.type_info->raw << std::endl;
        std::cout << "\t Immutable : " << stmt.is_immutable << std::endl;
        */
       report_incomplete("Assignment");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Reassignment & stmt) 
    {
        /*
        std::cout << "Analyzer::accept(Reassignment & stmt)" << std::endl;
        std::cout << "\t Var       : " << stmt.ast->left->node.data << std::endl;
        */
       report_incomplete("Reassignment");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ObjectAssignment & stmt) 
    {
        /*
        std::cout << "Analyzer::accept(ObjectAssignment & stmt)" << std::endl;

        std::cout << "-------------------- ASSIGN --------------------" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }

        std::cout << "------------------ END ASSIGN ------------------" << std::endl;
        */

       report_incomplete("ObjectAssignment");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ObjectReassignment & stmt) 
    {
        /*
        std::cout << "Analyzer::accept(ObjectAssignment & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var << std::endl;

        std::cout << "-------------------- ASSIGN --------------------" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }

        std::cout << "------------------ END ASSIGN ------------------" << std::endl;
        */

       report_incomplete("ObjectReassignment");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ObjectMember & stmt) 
    {
        /*
        std::cout << "Analyzer::accept(ObjectMember & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.name << std::endl; 
        std::cout << "\t Type : " << stmt.type_info->raw << std::endl;
        */

       report_incomplete("ObjectMember");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Object & stmt) 
    {
        /*
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
        */

       report_incomplete("Object");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(If & stmt) 
    {
        /*
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
        */
       report_incomplete("If");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(WhileLoop  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(WhileLoop & stmt)" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }
        */
       report_incomplete("WhileLoop");
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(ForLoop    &stmt)
    {
        /*
        std::cout << "Analyzer::accept(ForLoop & stmt)" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }
        */

       report_incomplete("ForLoop");
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(NamedLoop  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(NamedLoop & stmt)" << std::endl;

        for(auto & e : stmt.elements)
        {
            e->visit(*this);
            delete e;
        }
        */
       report_incomplete("NamedLoop");
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Continue   &stmt)
    {
        /*
        std::cout << "Analyzer::accept(Continue & stmt)" << std::endl;
        */
       report_incomplete("Continue");
    }
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Break      &stmt)
    {
        /*
        std::cout << "Analyzer::accept(Break & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.name << std::endl; 
        */
       report_incomplete("Break");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynCreate  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynCreate & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Type : " << DataType_to_string(stmt.type->dataType) << std::endl;
        std::cout << "\t Raw  : " << stmt.type->raw << std::endl;
        */
       report_incomplete("DynCreate");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynExpand  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynExpand & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Amnt : " << stmt.amount   << std::endl;
        */
       report_incomplete("DynExpand");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynInsert  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynInsert & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Idx  : " << stmt.idx   << std::endl;
        */
       report_incomplete("DynInsert");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynAppend  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynAppend & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        */
       report_incomplete("DynAppend");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynClear  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynClear & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        */
       report_incomplete("DynClear");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynDelete  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynDelete & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Idx  : " << stmt.idx << std::endl;
        */
       report_incomplete("DynDelete");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynGet  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynGet & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Idx  : " << stmt.idx << std::endl;
        std::cout << "\t Dest : " << stmt.dest << std::endl;
        */
       report_incomplete("DynGet");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynSize  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynSize & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Dest : " << stmt.dest << std::endl;
        */
       report_incomplete("DynSize");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynFront  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynFront & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Dest : " << stmt.dest << std::endl;
        */
       report_incomplete("DynFront");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(DynBack  &stmt)
    {
        /*
        std::cout << "Analyzer::accept(DynBack & stmt)" << std::endl;
        std::cout << "\t Name : " << stmt.var_name << std::endl;
        std::cout << "\t Dest : " << stmt.dest << std::endl;
        */
       report_incomplete("DynBack");
    }
}

#include "Analysis/Analyzer.hpp"

#include "del_driver.hpp"

// Forge
#include "SystemSettings.hpp"
#include "CodeStructures.hpp"
#include "Builder.hpp"


#include <iostream>

namespace DEL
{
    //
    // ===============--------------------------- Converter Methods ---------------------------===============
    //

    std::vector<FORGE::FunctionParam> convert_params_to_forge_params(std::vector<Parameter*> params)
    {
        std::vector<FORGE::FunctionParam> new_params;

        for(auto p : params)
        {
            if(p->is_ref)
            {
                switch(p->dataType)
                {
                    case DataType::INT: new_params.push_back(
                            FORGE::FunctionParam
                            {
                                p->var, 
                                new FORGE::Reference(new FORGE::Integer())
                            }
                        ); 
                    break;
                    case DataType::DOUBLE: new_params.push_back(
                            FORGE::FunctionParam
                            {
                                p->var, 
                                new FORGE::Reference(new FORGE::Double())
                            }
                        ); 
                    break;
                    case DataType::STRING: new_params.push_back(
                            FORGE::FunctionParam
                            {
                                p->var, 
                                new FORGE::Reference(new FORGE::List(FORGE::PrimitiveTypes::CHAR))
                            }
                        ); 
                    break;
                    case DataType::NIL: new_params.push_back(
                            FORGE::FunctionParam
                            {
                                p->var, 
                                new FORGE::Reference(new FORGE::Nil())
                            }
                        ); 
                    break;
                    default:
                        return std::vector<FORGE::FunctionParam>();
                        break;
                }
            }
            else
            {
                switch(p->dataType)
                {
                    case DataType::INT:    new_params.push_back(FORGE::FunctionParam{p->var, new FORGE::Integer()}); break;
                    case DataType::DOUBLE: new_params.push_back(FORGE::FunctionParam{p->var, new FORGE::Double()});  break;
                    case DataType::STRING: new_params.push_back(FORGE::FunctionParam{p->var, new FORGE::List(FORGE::PrimitiveTypes::CHAR)});  break;
                    case DataType::NIL:    new_params.push_back(FORGE::FunctionParam{p->var, new FORGE::Nil()}); break;
                    default:
                        return std::vector<FORGE::FunctionParam>();
                        break;
                }
            }
        }
        return new_params;
    }

    FORGE::PrimitiveTypes convert_type_to_forge_type(DataType type)
    {
        switch(type)
        {
            case DataType::INT:    return FORGE::PrimitiveTypes::INTEGER;
            case DataType::DOUBLE: return FORGE::PrimitiveTypes::DOUBLE;
            case DataType::STRING: return FORGE::PrimitiveTypes::LIST;
            case DataType::NIL:    return FORGE::PrimitiveTypes::NIL;
            case DataType::USER_DEFINED: 
                std::cerr << "convert_type_to_forge_type() hit case not yet complete" << std::endl;
                exit(EXIT_FAILURE);
                return FORGE::PrimitiveTypes::UNDEFINED;
            case DataType::NONE:
            default:
                return FORGE::PrimitiveTypes::UNDEFINED;
        }
    }

    DataType convert_type_to_data_type(FORGE::PrimitiveTypes type)
    {
        switch(type)
        {
            case FORGE::PrimitiveTypes::INTEGER: return DataType::INT;   
            case FORGE::PrimitiveTypes::DOUBLE:  return DataType::DOUBLE;
            case FORGE::PrimitiveTypes::LIST:    return DataType::STRING;
            case FORGE::PrimitiveTypes::NIL:     return DataType::NIL;
            default:
                return DataType::NONE;
        }
    }

    //
    // ===============---------------------------- Analyzer Methods ---------------------------===============
    //
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    Analyzer::Analyzer(DEL_Driver & driver) : driver(driver)
    {
        program_watcher.has_main    = false;
        function_watcher.has_return = false;
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
                    driver.preprocessor.fetch_user_line_number(stmt.line_number), 
                    27, 
                    "Duplicate context name (" + stmt.name + ") detected", 
                    {"Rename unit to be unique"}
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

        // Clear the symbol table for the given unit so elements cant be accessed externally
        // We dont delete the context though, that way can confirm existence later
        driver.symbol_table.clear_existing_context(stmt.name);
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
                    driver.preprocessor.fetch_user_line_number(stmt.line_number), 
                    27, 
                    "Duplicate context name (" + stmt.name + ") detected", 
                    {"Rename function to be unique"}
                )
            );
        }

        // Create a new context
        driver.symbol_table.new_context(stmt.name);

        if(stmt.name == "main")
        {
            program_watcher.has_main = true;
        }

        // Ensure parameters aren't too many in number
        if(stmt.params.size() > FORGE::SETTINGS::GS_FUNC_PARAM_RESERVE)
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(stmt.line_number),
                    -1, 
                    driver.preprocessor.fetch_line(stmt.line_number),
                    {
                        "Function parameters exceed number permitted by system (" + std::to_string(FORGE::SETTINGS::GS_FUNC_PARAM_RESERVE) + ")",
                        "Reduce the number of parameters for the given function"
                    }
                )
            );
        }

        // Convert our representation of the parameters to the representation that Forge uses
        std::vector<FORGE::FunctionParam> params = convert_params_to_forge_params(stmt.params);

        // Ensure conversion worked
        if(params.size() != stmt.params.size())
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::InternalReport(
                    FORGE::InternalReport::InternalInfo
                    {
                        "Analyzer",
                        "Analyzer.cpp",
                        "Analyzer::accept(Function & stmt)",
                        {
                            "There was an error converting function parameters from the front-end representation to the Forge representation.",
                            "This is an error that should have been stopped by the grammar. Please report this error to the contact information at",
                            "https://github.com/NablaVM"
                        }
                    }
                )
            );
        }

        // Add parameters to context
        driver.symbol_table.add_parameters_to_current_context(params);

        // Add return type to context
        driver.symbol_table.add_return_type_to_current_context(
                convert_type_to_forge_type(stmt.return_type->dataType)
        );

        driver.code_forge.get_builder().build(
            new FORGE::Builder::Construct(FORGE::Builder::Construct::Instruction::FUNCTION_BEGIN, stmt.name)
        );

        current_function = &stmt;

        function_watcher.has_return = false;

        // Add elements
        for(auto & el : stmt.elements)
        {
            // Visit Element
            el->visit(*this);
            delete el;
        }

        driver.code_forge.get_builder().build(
            new FORGE::Builder::Construct(FORGE::Builder::Construct::Instruction::FUNCTION_END, stmt.name)
        );

        // Clear the symbol table for the given function so elements cant be accessed externally
        // We dont delete the context though, that way can confirm existence later
        driver.symbol_table.clear_existing_context(stmt.name);

        // Check that the function has been explicitly returned at the end of the function
        if(!function_watcher.has_return)
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(stmt.line_number),
                    -1, 
                    driver.preprocessor.fetch_line(stmt.line_number),
                    {
                        "Given function does not have a matching return. All functions must be explicitly returned"
                    }
                )
            );
        }

        current_function = nullptr;

        // Reset the memory manager for alloc variables in new space
        driver.code_forge.reset_memory();

        // Clean params
        for(auto & p : stmt.params)
        {
            delete p;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Call & stmt) 
    {
        validate_call(stmt);


        #warning Analyzer::accept(Call & stmt) > We need to find a better way to pass param info to Builder
        #warning ---> we lose the fact that its a variable in validation

        int param_number = 0;
        for(auto & p : stmt.params)
        {
            driver.code_forge.get_builder().build(
                new FORGE::Builder::Call(
                    FORGE::Builder::Call::Instruction::PREP_PARAM,      // Indicate we want to prepare for call
                    p->is_ref,                                          // Pass flag for if its a reference
                    convert_type_to_forge_type(p->dataType),            // Pass the type that the data will be
                    p->var,                                             // Pass the variable
                    param_number++                                      // Inidcate what number the variable is
                )
            );
        }

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
       //report_incomplete("Call");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Return & stmt) 
    {
        // If we are in the function context then we can say we have an explicit return
        std::string context_name = driver.symbol_table.get_current_context_name();
        if(context_name == current_function->name)
        {
            function_watcher.has_return = true;
        }
    
        bool has_return = (stmt.ast != nullptr);

        // If there is something to return, we need to construct the return
        if(has_return)
        {
        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    ISSUE AN EXPR RETURN


            report_incomplete("This return indicates that there is something to return; this has not yet been implemented");
        }
        else
        {

        // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>    ISSUE A BLANK RETURN
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Assignment & stmt) 
    {
        // Ensure that the symbol name is unique
        ensure_unique_symbol(stmt.ast->left->node.data, stmt.line_number);

        // Validate and execute the instructions for assignment. This will send commandss to FORGE to build
        // the assignment. All that will be left is to issue the creation of the variable once we
        // actually add the symbol to the symbol table
        validate_and_execute_assignment_ast(
            stmt.ast->left->node.data, 
            stmt.ast->right, 
            stmt.type_info->dataType, 
            stmt.line_number
        );

        // Place in symbol table
        driver.symbol_table.add_symbol(
            stmt.ast->left->node.data,                              // Varable name
            convert_type_to_forge_type(stmt.type_info->dataType),   // Varaible tye
            !stmt.is_immutable                                      // is_mutable (opposite of whats stored by the statement)
        );

        // Now that the variable is in the symbol table the emit receiver can get the memory information
        // to perform the assignment
        driver.code_forge.get_builder().build(
            new FORGE::Builder::Assign(
                FORGE::Builder::Assign::Instruction::CREATE_NEW, 
                stmt.ast->left->node.data
            )
        );
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Reassignment & stmt) 
    {
        // Ensure the variable exists
        ensure_id_in_current_context(stmt.ast->left->node.data, stmt.line_number, {});

        // Get the type that we represent the thing as
        DataType type =  convert_type_to_data_type(
                            driver.symbol_table.get_value_type(stmt.ast->left->node.data)
                            );

        // Validate and execute the instructions for assignment. This will send commandss to FORGE to build
        // the assignment. All that will be left is to issue the reassignment
        validate_and_execute_assignment_ast(
            stmt.ast->left->node.data, 
            stmt.ast->right, 
            type, 
            stmt.line_number
        );

        // Instructions are ready to have reassignment triggered
        driver.code_forge.get_builder().build(
            new FORGE::Builder::Assign(
                FORGE::Builder::Assign::Instruction::REASSIGN, 
                stmt.ast->left->node.data
            )
        );
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
    
    //
    // ===============---------------------------- Analysis Methods ---------------------------===============
    //
    
    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::ensure_unique_symbol(std::string id, int line_no)
    {
        if(driver.symbol_table.does_symbol_exist(id, true))
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(line_no),
                    11, 
                    driver.preprocessor.fetch_line(line_no),
                    {
                        "Symbol \"" + id + "\" is not unique" 
                    }
                )
            );
        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Analyzer::ensure_id_in_current_context(std::string id, int line_no, std::vector<DataType> allowed)
    {
        /*
            Here (in the future) we need to split the variable by '.' if it contains any
            to check the context that it references so we can grab the correct variable
        */

        // Check symbol table to see if an id exists, don't display information yet
        if(!driver.symbol_table.does_symbol_exist(id, false))
        {
            // Reports the error and true marks the program for death
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(line_no),
                    -1, 
                    driver.preprocessor.fetch_line(line_no),
                    {
                        "Unknown identifier \"" + id + "\"" 
                    }
                )
            );
        }

        // If allowed is empty, we just wanted to make sure the thing existed
        if(allowed.size() == 0)
        {
            return;
        }

        // Ensure type is one of the allowed types
        bool is_allowed = false;
        for(auto & v : allowed)
        {
            if(driver.symbol_table.is_existing_symbol_of_type(id, convert_type_to_forge_type(v)))
            {
                is_allowed = true;
            }
        }

        // If the type isn't allowed
        if(!is_allowed)
        {
            // Reports the error and true marks the program for death
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(line_no),
                    -1, 
                    driver.preprocessor.fetch_line(line_no),
                    {
                        "Type of identifier \"" + id + "\" (" + 
                            FORGE::PrimitiveType_to_string (driver.symbol_table.get_value_type(id)) + 
                            ") not permitted in current operation", 
                    }
                )
            );
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::validate_call(Call & stmt)
    {
        // Disallow recursion until we come up with a way to handle it in the ASM
        //
        if(stmt.function_name == current_function->name)
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::InternalReport(
                    {
                        "DEL::Analyzer",
                        "Analyzer.cpp",
                        "validate_call",
                        {
                            "Recursion is not yet supported. A recursive call was detected on line : " + 
                            std::to_string(stmt.line_number) + " of file : " + driver.current_file_from_directive
                        }
                    }
                )
            );
        }

        /*
        
            We will need to 'drill into contexts;

            >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        
        */

        //  Check if the context exists
        //
        if(!driver.symbol_table.does_context_exist(stmt.function_name))
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(stmt.line_number),
                    -1, 
                    driver.preprocessor.fetch_line(stmt.line_number),
                    {
                        "Method name does not exist"
                    }
                )
            );
        }

        std::vector<FORGE::FunctionParam> params = driver.symbol_table.get_context_parameters(stmt.function_name);

        // Ensure that the parameters are the size we expect
        if(params.size() != stmt.params.size())
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(stmt.line_number),
                    -1, 
                    driver.preprocessor.fetch_line(stmt.line_number),
                    {
                        "Mismatched number of parameters given for call to : " + stmt.function_name,
                        "Expected " + std::to_string(params.size()) + ", but given " + std::to_string(stmt.params.size())
                    }
                )
            );
        }

        //  Ensure all parameters exist, and if they do set the type
        //
        for( auto & p : stmt.params)
        {
            // If the data type is an ID_STRING we need to figure out its type
            if(p->dataType == DataType::ID_STRING)
            {
                ensure_id_in_current_context(p->var, stmt.line_number, {});

                p->dataType = convert_type_to_data_type(
                    driver.symbol_table.get_value_type(p->var)
                );
            }
        }

        //  Check that the types match what we expect
        //
        for(uint16_t i = 0; i < stmt.params.size(); i++ )
        {
            if(stmt.params[i]->dataType != convert_type_to_data_type(params[i].type_info->getType()))
            {
                driver.code_forge.get_reporter().issue_report(
                    new FORGE::SemanticReport(
                        FORGE::Report::Level::ERROR,
                        driver.current_file_from_directive,
                        driver.preprocessor.fetch_user_line_number(stmt.line_number),
                        -1, 
                        driver.preprocessor.fetch_line(stmt.line_number),
                        {
                            "Given parameter \"" + stmt.params[i]->var + "\" doesn't match expected data type for call to : " + stmt.function_name,
                            "Received type  : " + DataType_to_string(stmt.params[i]->dataType),
                            "Expected type  : " + DataType_to_string(convert_type_to_data_type(params[i].type_info->getType()))
                        }
                    )
                );
            }
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::validate_and_execute_assignment_ast(std::string var_name, Ast * ast, DataType type, int line_number)
    {
        switch(ast->node.node_type)
        {
            //
            //      IDENTIFIER
            //
            case Ast::NodeType::IDENTIFIER:  
            {
                // Ensure the id is in the current context and its type matches the type of the expression
                // This means ints/ doubles don't mix. 
                ensure_id_in_current_context(ast->node.data, line_number, {type});

                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::LOAD_ID,
                    ast->node.data
                ));

                return;
            }
            //
            //      VALUE
            //
            case Ast::NodeType::VALUE:  
            {
                //  Ensure that the type of the item in the expression matches that of the  variable 
                //  type that its being assigned to
                //
                if(type != ast->node.data_type)
                {
                    driver.code_forge.get_reporter().issue_report(
                        new FORGE::SemanticReport(
                            FORGE::Report::Level::ERROR,
                            driver.current_file_from_directive,
                            driver.preprocessor.fetch_user_line_number(line_number),
                            -1, 
                            driver.preprocessor.fetch_line(line_number),
                            {
                                "Type of \"" + ast->node.data + "\" is \"" + DataType_to_string(ast->node.data_type) + 
                                "\", which is incompatible with type of \"" + var_name + 
                                "\" which is type \"" + DataType_to_string(type) + "\"\n"
                            }
                        )
                    );
                }

                if(type == DataType::STRING)
                {
                    //  If the type is string, we need to do a specific load to handle the string
                    //
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::LOAD_RAW_STR,
                        ast->node.data
                    ));
                }
                else
                {
                    //  If the type isn't a string we do a standard load_raw
                    //
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::LOAD_RAW,
                        ast->node.data
                    ));
                }
                return;
            }
            //
            //      EXPR CALL
            //
            case Ast::NodeType::CALL:  
            {
                std::cout << "EMIT <call>  : " << ast->node.data << std::endl;
                return;
            }
            case Ast::NodeType::ROOT:  
            {
                driver.code_forge.get_reporter().issue_report(
                    new FORGE::InternalReport(
                        {
                            "DEL::Analyzer",
                            "Analyzer.cpp",
                            "validate_and_execute_assignment_ast",
                            {
                                "A ROOT node slipped into function. The setup of Analyzer should not have allowed this"
                            }
                        }
                    )
                );
                return;
            }
            case Ast::NodeType::ADD:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);

                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::ADD
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::ADD_D
                    ));
                }
                else {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::CONCAT
                    ));
                }
                return;
            }
            case Ast::NodeType::SUB:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::SUB
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::SUB_D
                    ));
                }
                return;
            }
            case Ast::NodeType::LTE:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::LTE
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::LTE_D
                    ));
                }
                return;
            }
            case Ast::NodeType::GTE:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::GTE
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::GTE_D
                    ));
                }
                return;
            }
            case Ast::NodeType::GT:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::GT
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::GT_D
                    ));
                }
                return;
            }
            case Ast::NodeType::LT:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::LT
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::LT_D
                    ));
                }
                return;
            }
            case Ast::NodeType::EQ:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::EQ
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::EQ_D
                    ));
                }
                return;
            }
            case Ast::NodeType::NE:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::NE
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::NE_D
                    ));
                }
                return;
            }
            case Ast::NodeType::MUL:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::MUL
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::MUL_D
                    ));
                }
                return;
            }
            case Ast::NodeType::DIV:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::DIV
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::DIV_D
                    ));
                }
                return;
            }
            case Ast::NodeType::POW:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::POW
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::POW_D
                    ));
                }
                return;
            }
            case Ast::NodeType::MOD:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::MOD
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::MOD_D
                    ));
                }
                return;
            }
            case Ast::NodeType::LSH:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::LSH
                ));
                return;
            }
            case Ast::NodeType::RSH:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::RSH
                ));
                return;
            }
            case Ast::NodeType::BW_XOR:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::BW_XOR
                ));
                return;
            }
            case Ast::NodeType::BW_OR:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::BW_OR
                ));
                return;
            } 
            case Ast::NodeType::BW_AND:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::BW_AND
                ));
                return;
            }
            case Ast::NodeType::OR:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::OR
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::OR_D
                    ));
                }
                return;
            }
            case Ast::NodeType::AND:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::AND
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Conditional(
                        FORGE::Builder::Conditional::Instruction::AND_D
                    ));
                }
                return;
            }
            case Ast::NodeType::BW_NOT:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                    FORGE::Builder::Arith::Instruction::BW_NOT
                ));
                return;
            }
            case Ast::NodeType::NEGATE:  
            {
                validate_and_execute_assignment_ast(var_name, ast->left, type, line_number); validate_and_execute_assignment_ast(var_name, ast->right, type, line_number);
                
                if(type == DataType::INT) { 
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::NEGATE
                    ));
                }
                else if (type == DataType::DOUBLE) {
                    driver.code_forge.get_builder().build(new FORGE::Builder::Arith(
                        FORGE::Builder::Arith::Instruction::NEGATE_D
                    ));
                }
                return;
            }
            default:
            {
                driver.code_forge.get_reporter().issue_report(
                    new FORGE::InternalReport(
                        {
                            "DEL::Analyzer",
                            "Analyzer.cpp",
                            "validate_and_execute_assignment_ast",
                            {
                                "Default was accessed while walking the tree. This means a new AST node type was most likely added and not handled."
                            }
                        }
                    )
                );
                break;
            }
        }
    }
}

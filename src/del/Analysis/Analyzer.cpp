#include "Analysis/Analyzer.hpp"

#include "del_driver.hpp"

// Forge
#include "SystemSettings.hpp"

#include "forge/constructs/Variable.hpp"
#include "forge/datatypes/DataType.hpp"
#include "forge/instructions/Assignment.hpp"
#include "forge/instructions/Reassignment.hpp"
#include "forge/instructions/Instruction.hpp"


#include <iostream>

namespace DEL
{
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

    void Analyzer::accept(Function & stmt) 
    {
        // Ensure function name doesn't exist

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

        // Make a copy of the parameters
        std::vector<FORGE::Variable> params;
        for(auto & p : stmt.params)
        {
            params.push_back(*p);
        }

        // Add the parameters
        driver.symbol_table.add_parameters_to_current_context(params);

        // Add the return type
        driver.symbol_table.add_return_type_to_current_context(stmt.return_type->dataType);

        function_watcher.has_return = false;

        FORGE::Function * current_forge_function = new FORGE::Function(stmt.name , stmt.return_type->dataType);
        current_forge_aggregator = current_forge_function;
        current_front_function = &stmt;

        for(auto & el : stmt.elements)
        {
            el->visit(*this);
            delete el;
        }

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

        // Add function to forge for later generation
        driver.code_forge.add_ready_function(current_forge_function);

        // Reset pointers
        current_forge_aggregator = nullptr;
        current_front_function = nullptr;

        // Reset memory manager for alloc variables in new functions
        driver.code_forge.reset_memory();

        // Clean params
        //for(auto & p : stmt.params)
        //{
        //    delete p;
        //}
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Call & stmt) 
    {
      
       report_incomplete("Call");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Return & stmt) 
    {
        // If we are in the function context then we can say we have an explicit return
        std::string context_name = driver.symbol_table.get_current_context_name();

        if(context_name == current_front_function->name)
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
        if(driver.symbol_table.does_symbol_exist(stmt.ast->left->node.data))
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(stmt.line_number),
                    -1, 
                    driver.preprocessor.fetch_line(stmt.line_number),
                    {
                        "Symbol \"" + stmt.ast->left->node.data  + "\" used in assignment is not unique"
                    }
                )
            );
        }

        forge_expression_items.clear();

        // Build the expression items vector
        validate_and_build_assignment(stmt.ast->left->node.data, stmt.ast->right, stmt.type_info->dataType, stmt.line_number);

        FORGE::Assignment * assignment = new FORGE::Assignment(
            new FORGE::Variable(stmt.ast->left->node.data, stmt.type_info->dataType),
            new FORGE::Expression(stmt.type_info->dataType, forge_expression_items)
        );

        current_forge_aggregator->add_instruction(assignment);

        driver.symbol_table.add_symbol(stmt.ast->left->node.data, stmt.type_info->dataType, stmt.is_immutable);
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Reassignment & stmt) 
    {
        // Ensure the symbol to be reassigned has already been defined
        if(!driver.symbol_table.does_symbol_exist(stmt.ast->left->node.data))
        {
            driver.code_forge.get_reporter().issue_report(
                new FORGE::SemanticReport(
                    FORGE::Report::Level::ERROR,
                    driver.current_file_from_directive,
                    driver.preprocessor.fetch_user_line_number(stmt.line_number),
                    -1, 
                    driver.preprocessor.fetch_line(stmt.line_number),
                    {
                        "Symbol \"" + stmt.ast->left->node.data  + "\" for reassignment has not yet been defined"
                    }
                )
            );
        }

        // Get the symbol type
        FORGE::DataType lhs_type = driver.symbol_table.get_value_type(stmt.ast->left->node.data);

        // Clear the temp vector for expression building
        forge_expression_items.clear();

        // Build the expression items vector
        validate_and_build_assignment(stmt.ast->left->node.data, stmt.ast->right, lhs_type, stmt.line_number);

        // Build reassignment 
        FORGE::Reassignment * reassign = new FORGE::Reassignment(
            new FORGE::Variable(stmt.ast->left->node.data, lhs_type),
            new FORGE::Expression(lhs_type, forge_expression_items)
        );

        // Add reassignment instruction to forge
        current_forge_aggregator->add_instruction(reassign);
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

    //
    // ===============---------------------------- Analysis Methods ---------------------------===============
    //
    

    void Analyzer::validate_and_build_assignment(std::string var_name, Ast * ast, FORGE::DataType type, int line_number)
    {
        switch(ast->node.node_type)
        {
            //
            //      IDENTIFIER
            //
            case Ast::NodeType::IDENTIFIER:  
            {

                // Ensure the symbol to be reassigned has already been defined
                if(!driver.symbol_table.does_symbol_exist(ast->node.data))
                {
                    driver.code_forge.get_reporter().issue_report(
                        new FORGE::SemanticReport(
                            FORGE::Report::Level::ERROR,
                            driver.current_file_from_directive,
                            driver.preprocessor.fetch_user_line_number(line_number),
                            -1, 
                            driver.preprocessor.fetch_line(line_number),
                            {
                                "Symbol \"" + ast->node.data  + "\" used in expression does not exist"
                            }
                        )
                    );
                }

                // Ensure variable's type is compliant
                if(driver.symbol_table.get_value_type(ast->node.data) != type)
                {
                    driver.code_forge.get_reporter().issue_report(
                        new FORGE::SemanticReport(
                            FORGE::Report::Level::ERROR,
                            driver.current_file_from_directive,
                            driver.preprocessor.fetch_user_line_number(line_number),
                            -1, 
                            driver.preprocessor.fetch_line(line_number),
                            {
                                "Type of \"" + ast->node.data + "\" is \"" + FORGE::DataType_to_string(ast->node.data_type) + 
                                "\", which is incompatible with type of \"" + var_name + 
                                "\" which is type \"" + FORGE::DataType_to_string(type) + "\"\n"
                            }
                        )
                    );
                }

                // Indicate that value is a variable and hand the data
                forge_expression_items.push_back({FORGE::Expression::Instruction::VARIABLE, ast->node.data});
                return;
            }
            //
            //      VALUE
            //
            case Ast::NodeType::VALUE:  
            {
                // Ensure that the types align. int-int | double-double  | string-string
                // That way we know how to handle things in the rear end
                if(ast->node.data_type != type)
                {
                    driver.code_forge.get_reporter().issue_report(
                        new FORGE::SemanticReport(
                            FORGE::Report::Level::ERROR,
                            driver.current_file_from_directive,
                            driver.preprocessor.fetch_user_line_number(line_number),
                            -1, 
                            driver.preprocessor.fetch_line(line_number),
                            {
                                "Type of \"" + ast->node.data + "\" is \"" + FORGE::DataType_to_string(ast->node.data_type) + 
                                "\", which is incompatible with type of \"" + var_name + 
                                "\" which is type \"" + FORGE::DataType_to_string(type) + "\"\n"
                            }
                        )
                    );
                }

                // Indicate in the expression that we have a value, and store the data
                forge_expression_items.push_back({FORGE::Expression::Instruction::VALUE, ast->node.data});
                return;
            }
            //
            //      EXPR CALL
            //
            case Ast::NodeType::CALL:  
            {
/*
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

                Validate call 

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.
*/
                report_incomplete("CALL - IN EXPRESSION");


                forge_expression_items.push_back({FORGE::Expression::Instruction::CALL, ast->node.data});
                return;
            }
            case Ast::NodeType::ROOT:  
            {
                driver.code_forge.get_reporter().issue_report(
                    new FORGE::InternalReport(
                        {
                            "DEL::Analyzer",
                            "Analyzer.cpp",
                            "validate_and_build_assignment",
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
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::ADD, ""});
                return;
            }
            case Ast::NodeType::SUB:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::SUB, ""});
                return;
            }
            case Ast::NodeType::LTE:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::LTE, ""});
                return;
            }
            case Ast::NodeType::GTE:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::GTE, ""});
                return;
            }
            case Ast::NodeType::GT:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::GT, ""});
                return;
            }
            case Ast::NodeType::LT:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::LT, ""});
                return;
            }
            case Ast::NodeType::EQ:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::EQ, ""});
                return;
            }
            case Ast::NodeType::NE:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::NE, ""});
                return;
            }
            case Ast::NodeType::MUL:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::MUL, ""});
                return;
            }
            case Ast::NodeType::DIV:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::DIV, ""});
                return;
            }
            case Ast::NodeType::POW:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::POW, ""});
                return;
            }
            case Ast::NodeType::MOD:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::MOD, ""});
                return;
            }
            case Ast::NodeType::LSH:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::LSH, ""});
                return;
            }
            case Ast::NodeType::RSH:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::RSH, ""});
                return;
            }
            case Ast::NodeType::BW_XOR:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::BW_XOR, ""});
                return;
            }
            case Ast::NodeType::BW_OR:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::BW_OR, ""});
                return;
            } 
            case Ast::NodeType::BW_AND:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::BW_AND, ""});
                return;
            }
            case Ast::NodeType::OR:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::OR, ""});
                return;
            }
            case Ast::NodeType::AND:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::AND, ""});
                return;
            }
            case Ast::NodeType::BW_NOT:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::BW_NOT, ""});
                return;
            }
            case Ast::NodeType::NEGATE:  
            {
                validate_and_build_assignment(var_name, ast->left, type, line_number); validate_and_build_assignment(var_name, ast->right, type, line_number);
                forge_expression_items.push_back({FORGE::Expression::Instruction::NEGATE, ""});
                return;
            }
            default:
            {
                driver.code_forge.get_reporter().issue_report(
                    new FORGE::InternalReport(
                        {
                            "DEL::Analyzer",
                            "Analyzer.cpp",
                            "validate_and_build_assignment",
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

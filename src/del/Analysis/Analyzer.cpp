#include "Analysis/Analyzer.hpp"

#include "del_driver.hpp"

// Forge
#include "SystemSettings.hpp"

#include "forge/constructs/Variable.hpp"
#include "forge/datatypes/DataType.hpp"
#include "forge/instructions/Assignment.hpp"
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


        FORGE::DataType return_type = FORGE::DataType::STANDARD_INTEGER;

        switch(stmt.return_type->dataType)
        {
            case DataType::DOUBLE:  return_type = FORGE::DataType::STANDARD_DOUBLE;  break;
            case DataType::NIL:     return_type = FORGE::DataType::NIL;              break;
            case DataType::INT:     return_type = FORGE::DataType::STANDARD_INTEGER; break;
            case DataType::STRING:  return_type = FORGE::DataType::STANDARD_STRING;  break;
            default:
                report_incomplete("Default hit in accept(function) which means something isnt implemented");
                break;
        }

        current_forge_function = new FORGE::Function(stmt.name , return_type);
        current_front_function = &stmt;

        for(auto & el : stmt.elements)
        {
            el->visit(*this);
        }
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
        report_incomplete("Return");
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Assignment & stmt) 
    {
        forge_expression_items.clear();

        // Build the expression items vector
        validate_and_build_assignment(stmt.ast->left->node.data, stmt.ast->right, stmt.type_info->dataType, stmt.line_number);

        FORGE::DataType data_type = FORGE::DataType::STANDARD_INTEGER;
        switch(stmt.type_info->dataType )
        {
            case DataType::DOUBLE:  data_type = FORGE::DataType::STANDARD_DOUBLE;  break;
            case DataType::NIL:     data_type = FORGE::DataType::NIL;              break;
            case DataType::INT:     data_type = FORGE::DataType::STANDARD_INTEGER; break;
            case DataType::STRING:  data_type = FORGE::DataType::STANDARD_STRING;  break;
            default:
                report_incomplete("Default hit in accept(function) which means something isnt implemented");
                break;
        }

        FORGE::Assignment * assignment = new FORGE::Assignment(
            new FORGE::Variable(stmt.ast->left->node.data, data_type),
            new FORGE::Expression(data_type, forge_expression_items)
        );

        current_forge_function->add_instruction(assignment);
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Analyzer::accept(Reassignment & stmt) 
    {
        report_incomplete("Reassignment");
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
    

    void Analyzer::validate_and_build_assignment(std::string var_name, Ast * ast, DataType type, int line_number)
    {
        switch(ast->node.node_type)
        {
            //
            //      IDENTIFIER
            //
            case Ast::NodeType::IDENTIFIER:  
            {
/*
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

                Validate Identifier 

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.
*/
                forge_expression_items.push_back({FORGE::Expression::Instruction::VARIABLE, ast->node.data});
                return;
            }
            //
            //      VALUE
            //
            case Ast::NodeType::VALUE:  
            {
/*
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.

                Validate Value 

    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.
*/
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

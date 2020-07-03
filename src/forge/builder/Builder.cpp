#include "Builder.hpp"
#include "Reporter.hpp"

#include <iostream>

namespace FORGE
{

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    Builder::Builder(Forge & forge) : forge(forge)
    {
        
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    Builder::~Builder()
    {

    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    void Builder::build(BuilderInstruction * instruction)
    {
        switch(instruction->builder_type)
        {
        case BuilderType::ARITH:        handle( static_cast<Arith*>(instruction)       ); break;
        case BuilderType::CONDITIONAL:  handle( static_cast<Conditional*>(instruction) ); break;
        case BuilderType::FLOW:         handle( static_cast<Flow*>(instruction)        ); break;
        case BuilderType::ASSIGN:       handle( static_cast<Assign*>(instruction)      ); break;
        case BuilderType::CONSTRUCT:    handle( static_cast<Construct*>(instruction)   ); break; 
        case BuilderType::CALL:         handle( static_cast<Call*>(instruction)        ); break; 
        default:
            forge.get_reporter().issue_report(
                new FORGE::InternalReport(
                    {
                        "FORGE::Builder",
                        "Builder.cpp",
                        "build",
                        {
                            "Default accessed when routing builder instruction to handler method"
                        }
                    }
                )
            );
            break;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------

    void Builder::handle(Arith       * ins)
    {
        std::cout << "BUILDER::handle(arith) >> ";

        // Temp for dev
        switch(ins->ins)
        {
            case Builder::Arith::Instruction::ADD:      std::cout << "ADD     " << std::endl;  break;
            case Builder::Arith::Instruction::ADD_D:    std::cout << "ADD_D   " << std::endl;  break;
            case Builder::Arith::Instruction::SUB:      std::cout << "SUB     " << std::endl;  break;
            case Builder::Arith::Instruction::SUB_D:    std::cout << "SUB_D   " << std::endl;  break;
            case Builder::Arith::Instruction::DIV:      std::cout << "DIV     " << std::endl;  break;
            case Builder::Arith::Instruction::DIV_D:    std::cout << "DIV_D   " << std::endl;  break;
            case Builder::Arith::Instruction::MOD:      std::cout << "MOD     " << std::endl;  break;
            case Builder::Arith::Instruction::MOD_D:    std::cout << "MOD_D   " << std::endl;  break;
            case Builder::Arith::Instruction::POW:      std::cout << "POW     " << std::endl;  break;
            case Builder::Arith::Instruction::POW_D:    std::cout << "POW_D   " << std::endl;  break;
            case Builder::Arith::Instruction::MUL:      std::cout << "MUL     " << std::endl;  break;
            case Builder::Arith::Instruction::MUL_D:    std::cout << "MUL_D   " << std::endl;  break;
            case Builder::Arith::Instruction::BW_OR:    std::cout << "BW_OR   " << std::endl;  break;
            case Builder::Arith::Instruction::BW_XOR:   std::cout << "BW_XOR  " << std::endl;  break;
            case Builder::Arith::Instruction::BW_AND:   std::cout << "BW_AND  " << std::endl;  break;
            case Builder::Arith::Instruction::BW_NOT:   std::cout << "BW_NOT  " << std::endl;  break;
            case Builder::Arith::Instruction::LSH:      std::cout << "LSH     " << std::endl;  break;
            case Builder::Arith::Instruction::RSH:      std::cout << "RSH     " << std::endl;  break;
            case Builder::Arith::Instruction::NEGATE:   std::cout << "NEGATE  " << std::endl;  break;
            case Builder::Arith::Instruction::NEGATE_D: std::cout << "NEGATE_D" << std::endl;  break;
            case Builder::Arith::Instruction::CONCAT:   std::cout << "CONCAT  " << std::endl;  break;

            case Builder::Arith::Instruction::LOAD_RAW: 
                 std::cout << "LOAD_RAW >> " << ins->data << std::endl;  
                 break;

            case Builder::Arith::Instruction::LOAD_RAW_STR: 
                 std::cout << "LOAD_RAW_STR >> " << ins->data << std::endl;  
                 break;

            case Builder::Arith::Instruction::LOAD_ID:  
                 std::cout << "LOAD_ID >> " << ins->data << std::endl;  
                 break;
        }

    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    void Builder::handle(Conditional * ins)
    {
        std::cout << "BUILDER::handle(Conditional) >> ";

        // Temp for dev
        switch(ins->ins)
        {
            case Builder::Conditional::Instruction::LTE   : std::cout << "LTE  " << std::endl; break;
            case Builder::Conditional::Instruction::LTE_D : std::cout << "LTE_D" << std::endl; break;
            case Builder::Conditional::Instruction::GTE   : std::cout << "GTE  " << std::endl; break;
            case Builder::Conditional::Instruction::GTE_D : std::cout << "GTE_D" << std::endl; break;
            case Builder::Conditional::Instruction::GT    : std::cout << "GT   " << std::endl; break;
            case Builder::Conditional::Instruction::GT_D  : std::cout << "GT_D " << std::endl; break;
            case Builder::Conditional::Instruction::LT    : std::cout << "LT   " << std::endl; break;
            case Builder::Conditional::Instruction::LT_D  : std::cout << "LT_D " << std::endl; break;
            case Builder::Conditional::Instruction::EQ    : std::cout << "EQ   " << std::endl; break;
            case Builder::Conditional::Instruction::EQ_D  : std::cout << "EQ_D " << std::endl; break;
            case Builder::Conditional::Instruction::NE    : std::cout << "NE   " << std::endl; break;
            case Builder::Conditional::Instruction::NE_D  : std::cout << "NE_D " << std::endl; break;
            case Builder::Conditional::Instruction::OR    : std::cout << "OR   " << std::endl; break;
            case Builder::Conditional::Instruction::OR_D  : std::cout << "OR_D " << std::endl; break;
            case Builder::Conditional::Instruction::AND   : std::cout << "AND  " << std::endl; break;
            case Builder::Conditional::Instruction::AND_D : std::cout << "AND_D" << std::endl; break;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    void Builder::handle(Flow        * ins)
    {
        std::cout << "BUILDER::handle(Flow) >> ";

        // Temp for dev
        switch(ins->ins)
        {
            case Builder::Flow::Instruction::RETURN   : std::cout << "RETURN  " << std::endl; break;
            case Builder::Flow::Instruction::RETURN_D : std::cout << "RETURN_D" << std::endl; break;
            case Builder::Flow::Instruction::FOR      : std::cout << "FOR     " << std::endl; break;
            case Builder::Flow::Instruction::FOR_D    : std::cout << "FOR_D   " << std::endl; break;
            case Builder::Flow::Instruction::WHILE    : std::cout << "WHILE   " << std::endl; break;
            case Builder::Flow::Instruction::WHILE_D  : std::cout << "WHILE_D " << std::endl; break;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    void Builder::handle(Assign      * ins)
    {
        std::cout << "BUILDER::handle(Assign) >> ";

        // Temp for dev
        switch(ins->ins)
        {
            case Builder::Assign::Instruction::CREATE_NEW: std::cout << "CREATE_NEW : " << ins->data << std::endl; break;
            case Builder::Assign::Instruction::REASSIGN:   std::cout << "REASSIGN   : " << ins->data << std::endl; break;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    void Builder::handle(Construct   * ins)
    {
        std::cout << "BUILDER::handle(Construct) >> ";

        // Temp for dev
        switch(ins->ins)
        {
            case Builder::Construct::Instruction::FUNCTION_BEGIN: std::cout << "FUNCTION_BEGIN : " << ins->data << std::endl; break;
            case Builder::Construct::Instruction::FUNCTION_END:   std::cout << "FUNCTION_END   : " << ins->data << std::endl; break;
        }
    }

    // -----------------------------------------------------
    //
    // -----------------------------------------------------
    
    void Builder::handle(Call   * ins)
    {
        std::cout << "BUILDER::handle(Call) >> ";

        // Temp for dev
        switch(ins->ins)
        {
            case Builder::Call::Instruction::CALL      : std::cout << "CALL       : " << std::endl; break;
            case Builder::Call::Instruction::CALL_R    : std::cout << "CALL_R     : " << std::endl; break;
            case Builder::Call::Instruction::PREP_PARAM:
            
                std::cout << "PREP_PARAM | data:  " << ins->data << " is_ref : " << ins->is_ref
                          << " type : " << PrimitiveType_to_string(ins->type) << std::endl; 
                
                break;
            case Builder::Call::Instruction::LOAD_PARAM: std::cout << "LOAD_PARAM : " << ins->data << std::endl; break;
        }
    }
}
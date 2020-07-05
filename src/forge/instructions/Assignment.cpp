#include "Assignment.hpp"

#include <iostream>

#include "forge/asm/AsmMath.hpp"

namespace FORGE
{
    namespace
    {
        static uint64_t label_id = 0;
    }

    std::vector<std::string> Assignment::generate_NASM(SymbolTable & symbol_table) 
    {
        /*
        CODE::BlockAggregator * current_aggregator = new CODE::BlockAggregator();

        std::cout << "FORGE::ASSIGN(" << variable->getName() << ")" << std::endl;

        for(auto & item : expression->expression)
        {
            switch(item.instruction)
            {
            case Expression::Instruction::ADD     : current_aggregator->add_block(new CODE::Addition(expression->type));        break;
            case Expression::Instruction::SUB     : current_aggregator->add_block(new CODE::Subtraction(expression->type));     break;
            case Expression::Instruction::DIV     : current_aggregator->add_block(new CODE::Division(expression->type));        break;
            case Expression::Instruction::MUL     : current_aggregator->add_block(new CODE::Multiplication(expression->type));  break;
            case Expression::Instruction::LTE     : current_aggregator->add_block(new CODE::Lte(label_id++, expression->type)); break;
            case Expression::Instruction::GTE     : current_aggregator->add_block(new CODE::Gte(label_id++, expression->type)); break;
            case Expression::Instruction::GT      : current_aggregator->add_block(new CODE::Gt (label_id++, expression->type)); break;
            case Expression::Instruction::LT      : current_aggregator->add_block(new CODE::Lt (label_id++, expression->type)); break;
            case Expression::Instruction::EQ      : current_aggregator->add_block(new CODE::Eq (label_id++, expression->type)); break;
            case Expression::Instruction::NE      : current_aggregator->add_block(new CODE::Neq(label_id++, expression->type)); break;
            case Expression::Instruction::RSH     : current_aggregator->add_block(new CODE::RightShift());   break;
            case Expression::Instruction::LSH     : current_aggregator->add_block(new CODE::LeftShift());    break;
            case Expression::Instruction::BW_OR   : current_aggregator->add_block(new CODE::BwOr());         break;
            case Expression::Instruction::BW_XOR  : current_aggregator->add_block(new CODE::BwXor());        break;
            case Expression::Instruction::BW_AND  : current_aggregator->add_block(new CODE::BwAnd());        break;
            case Expression::Instruction::OR      : current_aggregator->add_block(new CODE::Or (label_id++, expression->type)); break;
            case Expression::Instruction::AND     : current_aggregator->add_block(new CODE::And(label_id++, expression->type)); break;
            case Expression::Instruction::BW_NOT  : current_aggregator->add_block(new CODE::BwNot());        break;
            case Expression::Instruction::NEGATE  : current_aggregator->add_block(new CODE::Negate(label_id++, expression->type)); break;
            case Expression::Instruction::MOD     : 
                current_aggregator->add_block(new CODE::BuiltIn("MOD", (DataType_is_double(expression->type)) ? FORGE::BUILT_IN::ASM_MOD_D_FUNCTION_NAME : FORGE::BUILT_IN::ASM_MOD_FUNCTION_NAME));
            break;
            case Expression::Instruction::POW     : 
                current_aggregator->add_block(new CODE::BuiltIn("POW", (DataType_is_double(expression->type)) ? FORGE::BUILT_IN::ASM_POW_D_FUNCTION_NAME : FORGE::BUILT_IN::ASM_POW_FUNCTION_NAME));
            break;
            case Expression::Instruction::VALUE   : break;
            case Expression::Instruction::VARIABLE: break;
            case Expression::Instruction::CALL    : break;   
            }
        }

        // Store the item
        Memory::MemAlloc mem_info = symbol_table.get_mem_info(variable->getName());
        current_aggregator->add_block(new CODE::Store(mem_info.start_pos, variable->getName()));

        std::vector<std::string> result = current_aggregator->instructions;
        delete current_aggregator;
*/
        return {};
    }
}
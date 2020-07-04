#ifndef FORGE_EXPRESSION_HPP
#define FORGE_EXPRESSION_HPP

#include "datatypes/DataType.hpp"
#include <string>
#include <vector>

// testing
#include <iostream>

namespace FORGE
{
    class Expression
    {
    public:

        enum class Instruction
        {
            ADD   ,
            SUB   ,
            DIV   ,
            MUL   ,
            MOD   ,
            POW   ,
            LTE   ,
            GTE   ,
            GT    ,
            LT    ,
            EQ    ,
            NE    ,
            LSH   ,
            RSH   ,
            BW_OR ,
            BW_XOR,
            BW_AND,
            OR    ,
            AND   ,
            BW_NOT,
            NEGATE,
            VALUE,      // Load a raw value
            VARIABLE,   // Use a variable
            CALL        // Call something and use returned item
        };

        struct ExpressionItem
        {
            Instruction instruction;
            std::string value;       // Depending on the instruction this could be blank, a variable name, a raw value, or a function name for a call
        };

        Expression(DataType type) : type(type) {}
        Expression(DataType type, std::vector<ExpressionItem> expression) : type(type), expression(expression){ }

        DataType type;
        std::vector<ExpressionItem> expression;

        // For testing
        void dump()
        {
            for(auto & e : expression)
            {
                switch(e.instruction)
                {
                    case Instruction::ADD       : std::cout << "ADD      " << std::endl; break;
                    case Instruction::SUB       : std::cout << "SUB      " << std::endl; break;
                    case Instruction::DIV       : std::cout << "DIV      " << std::endl; break;
                    case Instruction::MUL       : std::cout << "MUL      " << std::endl; break;
                    case Instruction::MOD       : std::cout << "MOD      " << std::endl; break;
                    case Instruction::POW       : std::cout << "POW      " << std::endl; break;
                    case Instruction::LTE       : std::cout << "LTE      " << std::endl; break;
                    case Instruction::GTE       : std::cout << "GTE      " << std::endl; break;
                    case Instruction::GT        : std::cout << "GT       " << std::endl; break;
                    case Instruction::LT        : std::cout << "LT       " << std::endl; break;
                    case Instruction::EQ        : std::cout << "EQ       " << std::endl; break;
                    case Instruction::NE        : std::cout << "NE       " << std::endl; break;
                    case Instruction::LSH       : std::cout << "LSH      " << std::endl; break;
                    case Instruction::RSH       : std::cout << "RSH      " << std::endl; break;
                    case Instruction::BW_OR     : std::cout << "BW_OR    " << std::endl; break;
                    case Instruction::BW_XOR    : std::cout << "BW_XOR   " << std::endl; break;
                    case Instruction::BW_AND    : std::cout << "BW_AND   " << std::endl; break;
                    case Instruction::OR        : std::cout << "OR       " << std::endl; break;
                    case Instruction::AND       : std::cout << "AND      " << std::endl; break;
                    case Instruction::BW_NOT    : std::cout << "BW_NOT   " << std::endl; break;
                    case Instruction::NEGATE    : std::cout << "NEGATE   " << std::endl; break;
                    case Instruction::VALUE     : std::cout << "VALUE    : " << e.value <<  std::endl; break;
                    case Instruction::VARIABLE  : std::cout << "VARIABLE : " << e.value << std::endl; break;
                    case Instruction::CALL      : std::cout << "CALL     : " << e.value << std::endl; break;
                }
            }
        }
    };
}

#endif
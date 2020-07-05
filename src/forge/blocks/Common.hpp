#ifndef FORGE_COMMON_ASM_HPP
#define FORGE_COMMON_ASM_HPP

#include <string>
#include <vector>
#include <stdint.h>
#include "forge/datatypes/DataType.hpp"
#include "forge/SystemSettings.hpp"
#include <sstream>
#include <libnabla/endian.hpp>
#include <libnabla/util.hpp>
#include <iostream>

namespace FORGE
{
namespace CODE
{

    /*
        Note : While these are technically configurable, care should be taken if they are updated, as it is 
                possible that register trampling could occur if they change. There aren't any checks built in
                to ensure this doesn't happen.
    */
    // Reg where we put stack pointer
    static const int REG_ADDR_SP = 1;

    // Return 
    static const int REG_ADDR_RO = 0;

    // Built in function operands
    static const int REG_BIF_LHS = 1;
    static const int REG_BIF_RHS = 2;

    // Generated operands
    static const int REG_ARITH_LHS = 8;
    static const int REG_ARITH_RHS = 9;

    // Conditional and comparison registers
    static const int REG_CONDITIONAL = 8;
    static const int REG_COMPARISON  = 7;

    // Line placement
    static constexpr char NL[]  = "\n";
    static constexpr char TAB[] = "\t";
    static constexpr char NLT[] = "\n\t";
    static constexpr char WS[]  = " ";
    static constexpr char WST[] = " \t";
    static constexpr char CALC_STACK[] = "ls";
    static constexpr char MEM_STACK[]  = "gs";

    //  Check if something is a double
    //
    static bool is_double_variant(DataType type)
    {
        return DataType_is_double(type);
    }

    //  Utilize the mov instruction to place a value of any size upto uint64_t in a register
    //
    static std::vector<std::string> load_64_into_r0(uint64_t le_64, std::string comment)
    {
        std::vector<std::string> result;

        result.push_back("\n\t; Load_64_into_r0 for : " + comment + "\n\n");

        // Mov instruction only handles 32-bit signed. So, if it starts to get bigger,
        // we need to dice it into parts
        if(le_64 > 2147483647)
        {
            uint32_t part_0 = (le_64 & 0xFFFF000000000000) >> 48;
            uint32_t part_1 = (le_64 & 0x0000FFFF00000000) >> 32;
            uint32_t part_2 = (le_64 & 0x00000000FFFF0000) >> 16;
            uint32_t part_3 = (le_64 & 0x000000000000FFFF) >> 0;

            result.push_back("\tmov r0 $" + std::to_string(part_0) + "\t ; part_0 \n");
            result.push_back("\tlsh r0 r0 $48\t\n");

            result.push_back("\tmov r1 $" + std::to_string(part_1) + "\t ; part_1 \n");
            result.push_back("\tlsh r1 r1 $32\t\n");
            result.push_back("\tor  r0 r0 r1\t\n");

            result.push_back("\tmov r1 $" + std::to_string(part_2) + "\t ; part_2 \n");
            result.push_back("\tlsh r1 r1 $16\t\n");
            result.push_back("\tor  r0 r0 r1\t\n");

            result.push_back("\tmov r1 $" + std::to_string(part_3) + "\t ; part_3 \n");
            result.push_back("\tor  r0 r0 r1\t\n");
        }
        else
        {
            result.push_back("\tmov r0 $" + std::to_string(static_cast<uint32_t>(le_64)) + "\t ; Data\n");
        }
        return result;
    }

    //
    //  A base 'block' that represents a code block. Contains representations of commonly used actions
    //  as well as a storage unit for generated code that a caller can access to get the data
    //
    class Block
    {
    public:

        Block(bool is_unary=false)
        {
            std::stringstream ss; 
            std::stringstream ss1; 
            std::stringstream ss2; 
            if(is_unary)
            {
                ss << NLT << "popw" << WS << "r" << REG_ARITH_LHS << WS << CALC_STACK << WST << "; Calculation - Unary";
                remove_for_calc = ss.str();
                ss.clear();

                ss1 << WS << "r" << REG_ARITH_LHS << WS  << "r" << REG_ARITH_LHS << WST << "; Perform unary operation"
                   << NLT << "pushw" << WS << CALC_STACK << WS << "r" << REG_ARITH_LHS << WST << "; Put result into calc stack";
                calculate_and_store = ss1.str();
            }
            else
            {
                ss << NLT  << "popw" << WS << "r" << REG_ARITH_RHS << WS << CALC_STACK << WST << "; Calculation RHS"
                   << NLT  << "popw" << WS << "r" << REG_ARITH_LHS << WS << CALC_STACK << WST << "; Calculation LHS";
                remove_for_calc = ss.str();

                ss1 << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WST << "; Perform operation"
                   << NLT << "pushw" << WS << CALC_STACK << WS << "r" << REG_ARITH_LHS << WST << "; Put result into calc stack";

                calculate_and_store = ss1.str();
            }

            ss2 << NLT  << "popw" << WS << "r" << REG_BIF_RHS << WS << CALC_STACK << WST << "; Calculation built in function RHS"
                << NLT  << "popw" << WS << "r" << REG_BIF_LHS << WS << CALC_STACK << WST << "; Calculation built in function LHS";
            bif_remove_for_calc = ss2.str();
            ss2.clear();
        }

        std::vector<std::string> get_code() const
        {
            return code;
        }

    protected:
        std::string remove_for_calc;
        std::string calculate_and_store;
        std::string bif_remove_for_calc;

        std::vector<std::string> code;
    };

    //
    //    A base conditional block
    //
    class Conditional : public Block
    {
    public:
        Conditional(uint64_t label_id, std::string comparison) : Block()
        {
            std::string label = "conditional_check_" + std::to_string(label_id);
            std::string complete = "conditional_complete_" + std::to_string(label_id);
            comparison = comparison + label;

            std::stringstream ss;

            ss << NLT 
               << remove_for_calc << NL 
               << comparison      << NL << NLT
               << "mov" << WS << "r" << REG_CONDITIONAL << WS << "$0" << TAB << "; False" << NL << NLT
               << "jmp" << WS << complete << NL << NL
               << label << ":" << NL << NLT
               << "mov" << WS << "r" << REG_CONDITIONAL << WS << "$1" << TAB << "; True" << NL << NL
               << complete << ":" << NL << NLT
               << "pushw" << WS << CALC_STACK << WS << "r" << REG_CONDITIONAL << TAB << "; Put result into calc stack" << NL;

            code.push_back(ss.str());
        }
    };

    //
    //      Addition
    //
    class Addition : public Block
    {
    public:
        Addition(FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "add.d" : "add";

            std::stringstream ss; 
            ss << NLT << "; <<< ADDITION >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      Subtraction
    //
    class Subtraction : public Block
    {
    public:
        Subtraction(FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "sub.d" : "sub";

            std::stringstream ss; 
            ss << NLT << "; <<< SUBTRACTION >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      Division
    //
    class Division : public Block
    {
    public:
        Division(FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "div.d" : "div";

            std::stringstream ss; 
            ss << NLT << "; <<< DIVISION >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      Multiplication
    //
    class Multiplication : public Block
    {
    public:
        Multiplication(FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "mul.d" : "mul";

            std::stringstream ss; 
            ss << NLT << "; <<< MULTIPLICATION >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      Right Shift
    //
    class RightShift : public Block
    {
    public:
        RightShift() : Block()
        {
            std::string cmd = "rsh";

            std::stringstream ss; 
            ss << NLT << "; <<< RIGHT SHIFT >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      Left Shift
    //
    class LeftShift : public Block
    {
    public:
        LeftShift() : Block()
        {
            std::string cmd = "lsh";

            std::stringstream ss; 
            ss << NLT << "; <<< LEFT SHIFT >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };
    
    //
    //      BW Or
    //
    class BwOr : public Block
    {
    public:
        BwOr() : Block()
        {
            std::string cmd = "or";

            std::stringstream ss; 
            ss << NLT << "; <<< BITWISE OR >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      BW Not
    //
    class BwNot : public Block
    {
    public:
        BwNot() : Block(true)
        {
            std::string cmd = "not";

            std::stringstream ss; 
            ss << NLT << "; <<< BITWISE NOT >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //      BW Xor
    //
    class BwXor : public Block
    {
    public:
        BwXor() : Block()
        {
            std::string cmd = "xor";

            std::stringstream ss; 
            ss << NLT << "; <<< BITWISE XOR >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };
    
    //
    //      BW And
    //
    class BwAnd : public Block
    {
    public:
        BwAnd() : Block()
        {
            std::string cmd = "and";

            std::stringstream ss; 
            ss << NLT << "; <<< BITWISE AND >>> " << NL
               << remove_for_calc
               << NLT
               << cmd << WS << calculate_and_store;

            code.push_back(ss.str());
        }
    };

    //
    //  LTE
    //
    class Lte : public Block
    {
    public: 
        Lte(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "blte.d" : "blte";

            std::stringstream sscmd;
            sscmd << TAB << cmd << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WS;

            std::stringstream ss;
            ss << NLT << "; <<< LTE >>>" << NL;

            code.push_back(ss.str());

            Conditional * c = new Conditional(label_id, sscmd.str());

            std::vector<std::string> ccode = c->get_code();

            code.insert(code.end(), ccode.begin(), ccode.end());

            delete c;
        }
    };

    //
    //  LT
    //
    class Lt : public Block
    {
    public: 
        Lt(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "blt.d" : "blt";

            std::stringstream sscmd;
            sscmd << TAB << cmd << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WS;

            std::stringstream ss;
            ss << NLT << "; <<< LT >>>" << NL;

            code.push_back(ss.str());

            Conditional * c = new Conditional(label_id, sscmd.str());

            std::vector<std::string> ccode = c->get_code();

            code.insert(code.end(), ccode.begin(), ccode.end());

            delete c;
        }
    };
    
    //
    //  GTE
    //
    class Gte : public Block
    {
    public: 
        Gte(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "bgte.d" : "bgte";

            std::stringstream sscmd;
            sscmd << TAB << cmd << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WS;

            std::stringstream ss;
            ss << NLT << "; <<< GTE >>>" << NL;

            code.push_back(ss.str());

            Conditional * c = new Conditional(label_id, sscmd.str());

            std::vector<std::string> ccode = c->get_code();

            code.insert(code.end(), ccode.begin(), ccode.end());

            delete c;
        }
    };
    
    //
    //  GT
    //
    class Gt : public Block
    {
    public: 
        Gt(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "bgt.d" : "bgt";

            std::stringstream sscmd;
            sscmd << TAB << cmd << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WS;

            std::stringstream ss;
            ss << NLT << "; <<< GT >>>" << NL;

            code.push_back(ss.str());

            Conditional * c = new Conditional(label_id, sscmd.str());

            std::vector<std::string> ccode = c->get_code();

            code.insert(code.end(), ccode.begin(), ccode.end());

            delete c;
        }
    };
    
    //
    //  EQ
    //
    class Eq : public Block
    {
    public: 
        Eq(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "beq.d" : "beq";

            std::stringstream sscmd;
            sscmd << TAB << cmd << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WS;

            std::stringstream ss;
            ss << NLT << "; <<< EQ >>>" << NL;

            code.push_back(ss.str());

            Conditional * c = new Conditional(label_id, sscmd.str());

            std::vector<std::string> ccode = c->get_code();

            code.insert(code.end(), ccode.begin(), ccode.end());

            delete c;
        }
    };
    
    //
    //  NEQ
    //
    class Neq : public Block
    {
    public: 
        Neq(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string cmd = (is_double_variant(classification)) ? "bne.d" : "bne";

            std::stringstream sscmd;
            sscmd << TAB << cmd << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_ARITH_RHS << WS;

            std::stringstream ss;
            ss << NLT << "; <<< NEQ >>>" << NL;

            code.push_back(ss.str());

            Conditional * c = new Conditional(label_id, sscmd.str());

            std::vector<std::string> ccode = c->get_code();

            code.insert(code.end(), ccode.begin(), ccode.end());

            delete c;
        }
    };

    //
    //  OR
    //
    class Or : public Block
    {
    public:
        Or(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string true_label = "OR_is_true_"    + std::to_string(label_id);
            std::string complete   = "OR_is_complete_" + std::to_string(label_id);
            std::string comparison = (is_double_variant(classification)) ? "bgt.d" : "bgt";

            std::stringstream ss;
            ss << NLT << "; <<< OR >>>" << NL
               << remove_for_calc << NL << NLT
               << "mov" << WS << "r" << REG_COMPARISON << WS << "$0" << TAB << "; Comparison Value" << NL << NLT
               << comparison << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_COMPARISON << WS << true_label << NL << NLT
               << comparison << WS << "r" << REG_ARITH_RHS << WS << "r" << REG_COMPARISON << WS << true_label << NL << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$0" << TAB << "; False" << NL << NLT
               << "jmp" << WS << complete << NL << NL
               << true_label << ":" << NL << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$1" << TAB << "; True" << NL << NL
               << complete << ":" << NL << NLT
               << "pushw" << WS << CALC_STACK << WS << "r" << REG_ARITH_LHS << TAB << "; Put result in calc stack" << NL;

            code.push_back(ss.str());
        }
    };


    //
    //  And
    //
    class And : public Block
    {
    public:
        And(uint64_t label_id, FORGE::DataType classification) : Block()
        {
            std::string first_true  = "AND_first_true_" + std::to_string(label_id);
            std::string second_true = "AND_second_true_" + std::to_string(label_id);
            std::string complete    = "AND_complete_" + std::to_string(label_id);
            std::string comparison = (is_double_variant(classification)) ? "bgt.d" : "bgt";

            std::stringstream ss;
            ss << NLT << "; <<< AND >>> " << NL 
               << remove_for_calc << NL << NLT 

               // Load comparison value
               << "mov" << WS << "r" << REG_COMPARISON << WS << "$0" << TAB << "; Comparison value" << NL << NLT
               
               // Check the lhs
               << comparison << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_COMPARISON << WS << first_true << NL << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$0" << TAB << "; False" << NL << NLT
               << "jmp" << WS << complete << NL << NL

               // Check the rhs
               << first_true << ":" << NL << NLT
               << comparison << WS << "r" << REG_ARITH_RHS << WS << "r" << REG_COMPARISON << WS << second_true << NL << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$0" << TAB << "; False" << NL << NLT
               << "jmp" << WS << complete << NL << NL

               // Both were true, mark true
               << second_true << ":" << NL << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$1" << TAB << "; True" << NL << NL 
               
               // Complete the check
               << complete << ":" << NL << NLT
               << "pushw"  << WS  << CALC_STACK << WS << "r" << REG_ARITH_LHS << TAB << "; Put result into calc stack" << NL;
            
            code.push_back(ss.str());
        }
    };

    //
    //  Negate
    //
    class Negate : public Block
    {
    public:
        Negate(uint64_t label_id, FORGE::DataType classification) : Block(true)
        {
            std::string set_zero = "NEGATE_set_zero_" + std::to_string(label_id);
            std::string set_comp = "NEGATE_complete_" + std::to_string(label_id);
            std::string comparison = (is_double_variant(classification)) ? "bgt.d" : "bgt";

            std::stringstream ss;
            ss << NLT << "; <<< NEGATE >>>" << NL
               << remove_for_calc << NL << NLT
               << "mov" << WS << "r" << REG_COMPARISON << WS << "$0" << TAB << "; Comparison" << NL << NLT
               << comparison << WS << "r" << REG_ARITH_LHS << WS << "r" << REG_COMPARISON << WS << set_zero << NL << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$1" << NLT
               << "jmp" << WS << set_comp << NL << NL
               << set_zero << ":" << NLT
               << "mov" << WS << "r" << REG_ARITH_LHS << WS << "$0" << NL << NL 
               << set_comp << ":" << NLT
               << "pushw" << WS << CALC_STACK << WS << "r" << REG_ARITH_LHS << TAB << "; Push result into calcl stack" << NL;

            code.push_back(ss.str());
        }
    };

    //
    //  BuiltIn
    //
    class BuiltIn : public Block
    {
    public: 
        BuiltIn(std::string title_comment, std::string function_name) : Block()
        {
            std::stringstream ss;

            ss << NLT
               << "; <<< " << title_comment << " >>> "    << NL << NLT
               << bif_remove_for_calc << NL << NLT 
               << "call"  << WS << function_name << TAB << "; Call built-in function " + title_comment << NL << NLT
               << "pushw" << WS << CALC_STACK << WS << "r" << REG_ADDR_RO << TAB << "; Push value on calc stack" << NL;

            code.push_back(ss.str()); 
        }
    };
}
}


#endif
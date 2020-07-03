#ifndef FORGE_BUILDER_HPP
#define FORGE_BUILDER_HPP

#include "Forge.hpp"
#include <string>

namespace FORGE
{
    class Builder
    {
    public:
        enum class BuilderType
        {
            ARITH,
            CONDITIONAL,
            FLOW,
            ASSIGN,
            CONSTRUCT
        };

        class BuilderInstruction
        {
        public:
            BuilderInstruction(BuilderType builder_type) : builder_type(builder_type)
            {}
            BuilderType builder_type;
        };

        class Arith : public BuilderInstruction
        {
        public:
            
            enum class Instruction
            {
                ADD, ADD_D,
                SUB, SUB_D,
                DIV, DIV_D,
                MOD, MOD_D,
                POW, POW_D,
                MUL, MUL_D,
                BW_OR,
                BW_XOR,
                BW_AND,
                BW_NOT,
                LSH, RSH,
                NEGATE, NEGATE_D,
                CONCAT,
                LOAD_RAW,
                LOAD_ID
            };

            Arith(Instruction ins) : BuilderInstruction(BuilderType::ARITH), ins(ins) {}
            Arith(Instruction ins, std::string data) : BuilderInstruction(BuilderType::ARITH), ins(ins), data(data) {}

            Instruction ins;
            std::string data;
        };

        class Conditional : public BuilderInstruction
        {
        public:

            enum class Instruction
            {
                LTE, LTE_D,
                GTE, GTE_D,
                GT , GT_D ,
                LT , LT_D ,
                EQ , EQ_D ,
                NE , NE_D ,
                OR , OR_D ,
                AND, AND_D
            };

            Conditional(Instruction ins) : BuilderInstruction(BuilderType::CONDITIONAL), ins(ins) {}
            Instruction ins;
        };

        class Flow : public BuilderInstruction
        {
        public:

            enum class Instruction
            {
                RETURN, RETURN_D,
                CALL,   CALL_D  ,
                FOR,    FOR_D   ,
                WHILE,  WHILE_D 
            };

            Flow(Instruction ins) : BuilderInstruction(BuilderType::FLOW), ins(ins) {}
            Instruction ins;
        };

        class Assign : public BuilderInstruction
        {
        public:

            enum class Instruction
            {
                CREATE_NEW,
                //ASSIGN_REF,
            };

            Assign(Instruction ins, std::string data) : BuilderInstruction(BuilderType::ASSIGN), ins(ins), data(data) {}
            Instruction ins;
            std::string data;
        };

        class Construct : public BuilderInstruction
        {
        public:

            enum class Instruction
            {
                FUNCTION_BEGIN,
                FUNCTION_END
            };

            Construct(Instruction ins, std::string data) : BuilderInstruction(BuilderType::CONSTRUCT), ins(ins), data(data) {}
            Instruction ins;
            std::string data;
        };

        Builder(Forge & forge);
        ~Builder();

        void build(BuilderInstruction * instruction);

    private:

        Forge & forge;

        void handle(Arith       * ins);
        void handle(Conditional * ins);
        void handle(Flow        * ins);
        void handle(Assign      * ins);
        void handle(Construct   * ins);
    };
}
#endif
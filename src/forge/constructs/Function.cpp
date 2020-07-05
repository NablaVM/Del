#include "Function.hpp"

namespace FORGE
{
    void Function::generate_NASM(Codegen & code_gen)
    {
        std::vector<CODEGEN::TYPES::ParamInfo> params;

        SymbolTable * symbol_table = code_gen.getSymbolTable();

        uint16_t param_index = 0;
        for(auto & p : parameters)
        {

            Memory::MemAlloc mem_info = symbol_table->get_mem_info(p->getName());
            params.push_back(
                {
                    mem_info.start_pos,
                    mem_info.start_pos + SETTINGS::SYSTEM_WORD_SIZE_BYTES,
                    param_index++
                }
            );
        }

        code_gen.begin_function(name, params);

        for(auto & ins : instructions)
        {
            ins->generate_NASM(code_gen);
        }

        code_gen.end_function();

        return;
    }
}
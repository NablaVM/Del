#include "Forge.hpp"
#include "managers/SymbolTable.hpp"
#include "ForgeSignal.hpp"
#include "reporting/Reporter.hpp"
#include <fstream>
#include <iostream>
#include "asm/AsmSupport.hpp"

#include <libnabla/assembler.hpp>
#include "SystemSettings.hpp"

namespace FORGE
{

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Forge::Forge()
    {
        reporter = new Reporter(*this);
        forge_signal_handler = Default_forge_signal_handler;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Forge::~Forge()
    {
        delete reporter;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Forge::add_forge_signal_receiver(std::function<void(ForgeSignal)> handler)
    {
        forge_signal_handler = handler;     
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------
    void Forge::reset_memory()
    {
        memory_manager.reset();
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Reporter & Forge::get_reporter() const
    {
        return *reporter;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Forge::add_ready_function(Function * function)
    {
        ready_to_build.push_back(function);
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::vector<uint8_t> Forge::generate_binary(SymbolTable & symbol_table)
    {
        symbol_table.lock();

        AsmSupport asm_support;

        std::vector<std::string> gen_asm; 

        asm_support.import_init_start(gen_asm);

        // Add space for where stack frame offset is stored
        gen_asm.push_back(".int64 __STACK_FRAME_OFFSET__\t" + std::to_string(SETTINGS::GS_INDEX_PROGRAM_SPACE) + "\n");

        // Add space in memory for parameter passing
        for(int i = 0; i < SETTINGS::GS_FUNC_PARAM_RESERVE; i++)
        {
            gen_asm.push_back(".int64 __PARAMETER__SPACE__" + std::to_string(i) + "__\t 0\n");
        }

        // Add reserved space
        for(int i = 0; i < SETTINGS::GS_RETURN_RESERVE; i++)
        {
            gen_asm.push_back(".int64 __RETURN_RESERVE__" + std::to_string(i) + "__\t 0\n");
        }

        asm_support.import_sl_funcs(gen_asm);
        asm_support.import_math(AsmSupport::Math::MOD_D, gen_asm);
        asm_support.import_math(AsmSupport::Math::MOD_I, gen_asm);
        asm_support.import_math(AsmSupport::Math::POW_D, gen_asm);
        asm_support.import_math(AsmSupport::Math::POW_I, gen_asm);
        asm_support.import_init_func(gen_asm);

        /*
            For now we are just generating ASM and then assembling it. Once the POC is all set
            we will move this to generating bytecode in each of the generator objects
        */

        for(auto & item : ready_to_build)
        {
            std::vector<std::string> nasm = item->generate_NASM(symbol_table);

            gen_asm.insert(gen_asm.end(), nasm.begin(), nasm.end());
        }

        /*
            -------------------------------------------------------------
                THIS NEEDS TO BE UPDATED - WRITTEN THIS WAY FOR DEV
            -------------------------------------------------------------
        */

        {
            std::ofstream asm_out;
            asm_out.open("FORGE.ASM");
            if(!asm_out.is_open())
            {
                std::cerr << "UNABLE TO OPEN FORGE.ASM FOR WRITING !@ " << std::endl;
                exit(EXIT_FAILURE);
            }
            for(auto & l : gen_asm)
            {
                asm_out << l;
            }
            asm_out.close();
            gen_asm.clear();
        }

        /*
            -------------------------------------------------------------
                THIS NEEDS TO BE UPDATED - WRITTEN THIS WAY FOR DEV
            -------------------------------------------------------------
        */

        std::vector<uint8_t> binary_data;

        if(!ASSEMBLER::ParseAsm("FORGE.ASM", binary_data, true))
        {
           reporter->issue_report(
                new FORGE::InternalReport(
                    {
                        "FORGE::Forge",
                        "Forge.cpp",
                        "generate_binary",
                        {
                            "Unable to assemble the generated NASM"
                        }
                    }
                )
           );
        }

        return binary_data;
    }
}
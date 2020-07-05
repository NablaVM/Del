#include "Forge.hpp"
#include "managers/SymbolTable.hpp"
#include "ForgeSignal.hpp"
#include "reporting/Reporter.hpp"
#include <fstream>
#include <iostream>

#include <libnabla/assembler.hpp>
#include "SystemSettings.hpp"

namespace FORGE
{

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Forge::Forge() : code_gen(memory_manager)
    {
        reporter = new Reporter(*this);
        forge_signal_handler = Default_forge_signal_handler;

        code_gen.set_symbol_table(symbol_table);
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Forge::setSymbolTable(SymbolTable & symbol_table)
    {
        this->symbol_table = &symbol_table;
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
        //  Trigger all functions to begin generating the code built into them
        //
        for(auto & item : ready_to_build)
        {
            item->generate_NASM(code_gen);
        }

        // Lock the symbol table
        symbol_table.lock();

        // Tell code gen to build the code requested by the functions 
        //
        std::vector<std::string> gen_asm = code_gen.indicate_complete();

        //  Write the asm to file
        //
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

        //  Assemble the code using the nabla assembler 
        //
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

        //  Return the resulting bytes
        //
        return binary_data;
    }
}
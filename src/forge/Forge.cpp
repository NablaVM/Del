#include "Forge.hpp"
#include "managers/SymbolTable.hpp"
#include "ForgeSignal.hpp"
#include "reporting/Reporter.hpp"

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

        /*
            For now we are just generating ASM and then assembling it. Once the POC is all set
            we will move this to generating bytecode in each of the generator objects
        */

        std::vector<std::string> gen_asm; 
        for(auto & item : ready_to_build)
        {
            std::vector<std::string> nasm = item->generate_NASM(symbol_table);

            gen_asm.insert(gen_asm.end(), nasm.begin(), nasm.end());
        }

        std::vector<uint8_t> byte_code;


        return byte_code;
    }
}
#include "Forge.hpp"

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
}
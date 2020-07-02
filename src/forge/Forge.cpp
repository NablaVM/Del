#include "Forge.hpp"

#include "ForgeSignal.hpp"
#include "Reporter.hpp"

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

    Reporter & Forge::get_reporter() const
    {
        return *reporter;
    }
}
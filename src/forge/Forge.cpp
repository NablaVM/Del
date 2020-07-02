#include "Forge.hpp"

#include "ForgeSignal.hpp"
#include "Reporter.hpp"
#include "Builder.hpp"

namespace FORGE
{

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Forge::Forge()
    {
        reporter = new Reporter(*this);
        builder  = new Builder(*this);
        forge_signal_handler = Default_forge_signal_handler;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Forge::~Forge()
    {
        delete builder;
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

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Builder & Forge::get_builder() const
    {
        return *builder;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Forge::reset_memory()
    {
        memory_manager.reset();
    }
}
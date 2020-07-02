#ifndef FORGE_FORGE_HPP
#define FORGE_FORGE_HPP

#include "ForgeFwd.hpp"
#include "Memory.hpp"

#include <functional>

namespace FORGE
{
    //! \brief The Forge object used for instructing systems of code generation
    class Forge
    {
    public:
    
        //! \brief Create a forge
        Forge();

        //! \brief Destruct forge
        ~Forge();

        //! \brief Add a custom forge signal receiver
        //! \param handler The function that will receieve forge signals
        void add_forge_signal_receiver(std::function<void(ForgeSignal)> handler);

        //! \brief Retrieve the reporter to report something
        //! \returns Reference to the Forge reporting mechanism
        Reporter & get_reporter() const;

        //! \brief Retrive the builder
        //! \returns Regerence to the Forge code builder
        Builder & get_builder() const;

        //! \brief Trigger a memory reset
        void reset_memory();

        friend Reporter;
        friend SymbolTable;
        friend Builder;

    private:

        Memory memory_manager;
        Builder  * builder;
        Reporter * reporter;
        std::function<void(ForgeSignal)> forge_signal_handler;
    };
}


#endif
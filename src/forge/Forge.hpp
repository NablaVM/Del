#ifndef FORGE_FORGE_HPP
#define FORGE_FORGE_HPP

#include "ForgeSignal.hpp"
#include "Reporter.hpp"
#include <functional>


/*
    Todo : Define callbacks that can be setup so owner of a forge can
           can be directed to handle fatal errors, if the cb isn't defined
           the forge can kill the program - This will be triggered from
           an "ERROR" type report, or an error that occurs within the forge itsself



*/

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

        friend Reporter;

    private:

        Reporter * reporter;
        std::function<void(ForgeSignal)> forge_signal_handler;
    };
}


#endif
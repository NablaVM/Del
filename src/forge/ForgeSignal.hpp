#ifndef FORGE_FORGE_SIGNAL_HPP
#define FORGE_FORGE_SIGNAL_HPP

#include <string>
#include <iostream>


namespace FORGE
{
    enum class FSignalType
    {
        FATAL_ERROR
    };

    //! \brief A signal that can be sent to trigger notifications 
    class ForgeSignal
    {
    public: 

        //! \brief Create a signal - Default message
        //! \param signal_type The type of signal that is being sent
        //! \param sender The origin of the singal
        ForgeSignal(FSignalType signal_type, std::string sender) : 
            signal_type(signal_type), sender(sender)
        {
            // Build a message associated with the signal
            switch(signal_type)
            {
            case FSignalType::FATAL_ERROR: signal_message = "A fatal error has occured"; break;
            default: signal_message = "Unknown signal type given to ForgeSignal"; break;
            }
        }

        //! \brief Create a signal - Custom message
        //! \param signal_type The type of signal that is being sent
        //! \param sender The origin of the singal
        ForgeSignal(FSignalType signal_type, std::string sender, std::string message) : 
            signal_type(signal_type), sender(sender), signal_message(message)
        {
        }

        //! \brief Get the signal type
        //! \returns Enum FSignalType 
        FSignalType get_signal_type() const { return signal_type;    }

        //! \brief Get name of sender who sent signal
        //! \returns string representing the name of the sender
        std::string get_sender()      const { return sender;         }

        //! \brief Get message associated with the signal
        //! \returns string detailing the signal
        std::string get_message()     const { return signal_message; }

    private:
        FSignalType signal_type;
        std::string sender;
        std::string signal_message;
    };


    //! \brief The default signal handler used by Forge, to override which signal handler
    //!        is used, pass the function to the forge object's "add_forge_signal_receiver" method
    //! \param signal The signal that was sent
    static void Default_forge_signal_handler(ForgeSignal signal)
    {
        // Start signal message
        std::cerr << "FORGE::Default_signal_handler :: From (" 
                  << signal.get_sender()
                  << ") :: Singnal ( ";

        // Indicate signal type
        switch(signal.get_signal_type())
        {
        case FSignalType::FATAL_ERROR: std::cerr << "FATAL_ERROR";  break;
        default: std::cerr << "UNKNOWN_SIGNAL"; break;
        }
                  
        std::cerr << " ) :: Message (" 
                  << signal.get_message()
                  << ")" << std::endl;

        if(signal.get_signal_type() == FSignalType::FATAL_ERROR)
        {
            std::cerr << "Forge issuing force exit" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

#endif 
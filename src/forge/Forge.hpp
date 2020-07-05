#ifndef FORGE_FORGE_HPP
#define FORGE_FORGE_HPP

#include "ForgeFwd.hpp"
#include "constructs/Function.hpp"
#include "managers/Memory.hpp"
#include "codegen/Codegen.hpp"

#include <functional>

namespace FORGE
{
    //! \brief The Forge object used for instructing systems of code generation
    class Forge
    {
    public:
    
        //! \brief Create a forge
        Forge();

        void setSymbolTable(SymbolTable & symbol_table);

        //! \brief Destruct forge
        ~Forge();

        //! \brief Add a custom forge signal receiver
        //! \param handler The function that will receieve forge signals
        void add_forge_signal_receiver(std::function<void(ForgeSignal)> handler);

        //! \brief Retrieve the reporter to report something
        //! \returns Reference to the Forge reporting mechanism
        Reporter & get_reporter() const;

        //! \brief Reset the memory object
        void reset_memory();

        //! \brief Add a function that has been validated and ready to be build
        //! \param The pointer to the function that is ready to build
        void add_ready_function(Function * function);

        //! \brief Generate the binary
        //! \returns Vector of bytes for execution
        //! \post Once this is called the symbol table will be locked and no more code can 
        //!       be readied for generation
        std::vector<uint8_t> generate_binary(SymbolTable & symbol_table);

        friend Reporter;
        friend SymbolTable;

    private:

        SymbolTable * symbol_table;
        Codegen code_gen;
    
        Memory memory_manager;
        Reporter * reporter;
        std::function<void(ForgeSignal)> forge_signal_handler;

        std::vector<Function*> ready_to_build;
    };
}


#endif
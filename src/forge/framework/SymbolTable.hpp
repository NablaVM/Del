#ifndef FORGE_SYMBOL_TABLE_HPP
#define FORGE_SYMBOL_TABLE_HPP

#include "Forge.hpp"
#include "DataTypes.hpp"
#include "Primitives.hpp"
#include "CodeStructures.hpp"

#include <vector>
#include <map>
#include <string>

namespace FORGE
{
    //!\class SymbolTable
    //!\brief A symbol table for keeping track of variables and contexts
    class SymbolTable
    {
    public:
        //!\brief Setup a symbol table
        SymbolTable(Forge & forge);

        //!\brief Destruct table
        ~SymbolTable();

        //! \brief Create a new named context
        //! \param name The name of the context
        //! \param remove_previous If true, the current context will be removed from knowledge base
        void new_context(std::string name, bool remove_previous=false);

        //! \brief Check if a symbol exists
        //! \param symbol The symbol to check for
        //! \param show_if_found Displays symbol information if true
        //! \retval True if exists, false otherwise
        bool does_symbol_exist(std::string symbol, bool show_if_found=false);

        //! \brief Check if a context exits
        //! \param context The context name to check for
        //! \retval True if exists, false otherwise
        bool does_context_exist(std::string context);

        //! \brief Check if a symbol is a given type
        //! \param symbol The symbol to check
        //! \param type The type to check
        //! \retval True if is type, false otherwise (including if the symbol doesn't exist)
        bool is_existing_symbol_of_type(std::string symbol, PrimitiveTypes type);

        //! \brief Add a symbol to the current context
        //! \param symbol The symbol to add
        //! \param type The value type to add as
        //! \param is_mutable Flag to indicate if the symbol can be changed
        void add_symbol(std::string symbol, PrimitiveTypes type, bool is_mutable);

        //! \brief Add parameters to the current context
        //! \param params The parameters to add to the context
        //! \note  This assumes that parameters don't yet exist. If they do, they'll be overwritten
        void add_parameters_to_current_context(std::vector<FunctionParam> params);

        //! \brief Get the parameters for a given context
        //! \param context The name of the context to get the parameters of
        std::vector<FunctionParam> get_context_parameters(std::string context);

        //! \brief Add a return type to the current context
        //! \param type The type that the context is expected to return
        void add_return_type_to_current_context(PrimitiveTypes type);

        //! \brief Get the return type of the given context
        //! \param context The context name to get the return type of
        PrimitiveTypes get_return_type_of_context(std::string context);

        //! \brief Clear the existing context of all symbols
        //! \param context The context to clear
        void clear_existing_context(std::string context);

        //! \brief Remove the current operating context 
        //! \post  This will remove all information regarding current context,
        //!        and will not remove current context if it is the last remaining context.
        //!        This will remove any reference to variables within this context from the memory manager
        //!        as well.
        void remove_current_context();

        //! \brief Retrieve the data type of a symbol
        //! \param symbol The symbol to check for
        //! \returns Type of the given symbol, PrimitiveTypes::UNDEFINED if it doesn't exist
        PrimitiveTypes get_value_type(std::string symbol);

        //! \brief Retrieve the data type of a symbol
        //! \param symbol The symbol to check for
        //! \returns True if symbol can be mutated
        bool get_value_mutability(std::string symbol);

        //! \brief Get the name of the current context
        //! \returns String of the current context name
        std::string get_current_context_name() const;

        //! \brief Generate a unique symbol for a return
        //! \returns Unique symbol to use as a return item in assignment
        std::string generate_unique_return_symbol();

        //! \brief Generate a unique symbol for a raw item in a function call
        //! \returns Unique symbol to use as a call parameter in a call
        std::string generate_unique_call_param_symbol();

        //! \brief Generate a unique symbol for an artificial variable
        //! \returns Unique symbol to use as a variable name
        std::string generate_unique_variable_symbol();

        //! \brief Generate a unique context name
        //! \returns Unique symbol to use as a context
        std::string generate_unique_context();

    private:
        Forge & forge;
        bool is_locked;
        uint64_t unique_counter;
        
        void lock();

        void report_lock() const;

        void report_error(std::string from, std::string regarding) const;

        std::string generate_unique(std::string start);

        struct SymbolInfo
        {
            PrimitiveTypes type;
            bool is_mutable;
        };

        class Context
        {
        public:
            Context(std::string name) : context_name(name), return_type(PrimitiveTypes::UNDEFINED) {}
            std::string context_name;
            std::map< std::string, SymbolInfo > symbol_map; 
            std::vector<FunctionParam> context_parameters;
            PrimitiveTypes return_type;
        };

        std::vector<Context * > contexts; 
    };
}

#endif
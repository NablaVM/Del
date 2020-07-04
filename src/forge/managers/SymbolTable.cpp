#include "SymbolTable.hpp"
#include "ForgeSignal.hpp"
#include "SystemSettings.hpp"

#include "Forge.hpp"
#include <iostream>

namespace FORGE
{

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    SymbolTable::SymbolTable(Forge & forge) : 
                                forge(forge),
                                is_locked(false),
                                unique_counter(0)
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    SymbolTable::~SymbolTable()
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::new_context(std::string name, bool remove_previous)
    {
        if(is_locked) { report_lock(); }

        if(remove_previous)
        {
            remove_current_context();
        }

        contexts.push_back(new Context(name));
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    bool SymbolTable::does_symbol_exist(std::string symbol, bool show_if_found)
    {
        if(is_locked) { report_lock(); }
        
        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->symbol_map.find(symbol) != c->symbol_map.end())
                {
                    if(show_if_found)
                    {
                        std::cout << "\"" << symbol 
                                  << "\" found in context \"" 
                                  << c->context_name 
                                  << "\" defined as : " 
                                  << DataType_to_string(c->symbol_map[symbol].type)
                                  << std::endl;
                    }
                    return true;
                }
            }
        }
        return false;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    bool SymbolTable::does_context_exist(std::string context)
    {
        if(is_locked) { report_lock(); }
        
        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->context_name == context)
                {
                    return true;
                }
            }
        }
        return false;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::vector<Variable> SymbolTable::get_context_parameters(std::string context)
    {
        if(is_locked) { report_lock(); }

        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->context_name == context)
                {
                    return c->context_parameters;
                }
            }
        }
        report_error("FORGE::SymbolTable::get_context_parameters()", 
                     "Asked to get parameters from a context that did not exist");

        return std::vector<Variable>(); // For the compiler
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::clear_existing_context(std::string context)
    {
        if(is_locked) { report_lock(); }
        
        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->context_name == context)
                {
                    c->symbol_map.clear();
                    return;
                }
            }
        }
        report_error("FORGE::SymbolTable::clear_existing_context()",
                     "Asked to clear existing context, but that context did not exist");
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::remove_current_context()
    {
        // Don't allow the removal of the global context
        if(contexts.size() > 1)
        {
            Context * ciq = contexts.back();

            // Remove symbols from map for reuse
            for(auto & el : ciq->symbol_map)
            {
                forge.memory_manager.remove_item(el.first);
            }

            delete ciq;
            contexts.pop_back();
        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    bool SymbolTable::is_existing_symbol_of_type(std::string symbol, DataType type)
    {
        if(is_locked) { report_lock(); }
        
        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->symbol_map.find(symbol) != c->symbol_map.end())
                {
                    if(c->symbol_map[symbol].type == type)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
        return false;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::add_symbol(std::string symbol, DataType type, bool is_mutable)
    {
        if(is_locked) { report_lock(); }

        if(contexts.size() == 0)
        {
            report_error("FORGE::SymbolTable::add_symbo()",
                         "No contexts present in symbol table");
        }
        
        contexts.back()->symbol_map[symbol] = { type, is_mutable };

        if(forge.memory_manager.is_id_mapped(symbol))
        {
            report_error("FORGE::SymbolTable::add_symbo()",
                         "Item given to symbol table already exists within the memory map");
        }

        // Attempt to 'allocate' memory
        if(!forge.memory_manager.alloc_mem(symbol))
        {
            report_error("FORGE::SymbolTable::add_symbo()",
                         "Allocation of [" + symbol + "] would exceed maximum system memory : " + std::to_string(SETTINGS::MAX_GLOBAL_MEMORY));
        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::add_parameters_to_current_context(std::vector<Variable> params)
    {
        contexts.back()->context_parameters.clear();
        contexts.back()->context_parameters = params;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    DataType SymbolTable::get_value_type(std::string symbol)
    {
        if(is_locked) { report_lock(); }
        
        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->symbol_map.find(symbol) != c->symbol_map.end())
                {
                    return c->symbol_map[symbol].type;
                }
            }
        }
        return DataType::NIL;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    bool SymbolTable::get_value_mutability(std::string symbol)
    {
        if(is_locked) { report_lock(); }
        
        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->symbol_map.find(symbol) != c->symbol_map.end())
                {
                    return c->symbol_map[symbol].is_immutable;
                }
            }
        }
        return false;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::add_return_type_to_current_context(DataType type)
    {
        contexts.back()->return_type = type;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    DataType SymbolTable::get_return_type_of_context(std::string context)
    {
        if(is_locked) { report_lock(); }

        for(auto & c : contexts)
        {
            if(c != nullptr)
            {
                if(c->context_name == context)
                {
                    return c->return_type;
                }
            }
        }

        report_error("FORGE::SymbolTable::get_return_type_of_context()", 
                     "Asked to get parameters from a context that did not exist");

        return DataType::UNDEFINED; // For the compiler
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SymbolTable::get_current_context_name() const
    {
        if(is_locked) { report_lock(); }
        
        return contexts.back()->context_name;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SymbolTable::generate_unique_return_symbol()
    {
        return generate_unique("__return__assignment__");
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SymbolTable::generate_unique_call_param_symbol()
    {
        return generate_unique("__call__param__");
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SymbolTable::generate_unique_variable_symbol()
    {
        return generate_unique("__artifical__variable__");
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SymbolTable::generate_unique_context()
    {
        std::string start = "__artificial__context__";
        std::string context_name = start + std::to_string(unique_counter);

        uint64_t stop_count = 0;
        while(does_context_exist(context_name))
        {
            unique_counter++;
            context_name = start + std::to_string(unique_counter);

            stop_count++;
            if(stop_count == 2048)
            {
                report_error("FORGE::SymbolTable::generate_unique_context()", 
                             "2048 Attempts were made to generate a unique context. If you're seeing this, you're doing something silly. Stop it.");
            }
        }
        unique_counter++;

        return context_name;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SymbolTable::generate_unique(std::string start)
    {
        std::string unique_label = start + std::to_string(unique_counter);

        // Whis should never happen unless a user decides to use "__return__assignment__" as a variable name, which I guess we COULD deny, but why would we? 
        // its a perfectly good name.
        uint64_t stop_count = 0;
        while(does_symbol_exist(unique_label, false))
        {
            unique_counter++;
            unique_label = start + std::to_string(unique_counter);

            stop_count++;
            if(stop_count == 2048)
            {
                report_error("FORGE::SymbolTable::generate_unique()",
                             "2048 Attempts were made to generate a unique symbol. If you're seeing this, you're doing something silly. Stop it.");
            }
        }
        unique_counter++;

        return unique_label;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::lock()
    {
        if(is_locked) { report_lock(); }
        
        is_locked = true;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::report_lock() const
    {
        report_error("FORGE::SymbolTable::report_lock()", "Update to symbol table attempted after table was locked for code generation");
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void SymbolTable::report_error(std::string from, std::string regarding) const
    {
        forge.forge_signal_handler(
            ForgeSignal(
                FSignalType::FATAL_ERROR, 
                from, 
                regarding
                )
        );
    }
}
#ifndef DEL_SYMBOL_TABLE
#define DEL_SYMBOL_TABLE

#include "Managers/DefinedTypes.hpp"
#include <vector>
#include <map>

namespace DEL
{
    //! \brief A table of symbols in the program
    class SymbolTable
    {
    public:

        struct TableEntry
        {
            std::string variable_name;
            std::string type_name;
        };


    private:

        std::map< std::string, std::vector< TableEntry>  > symbol_table;
    };
}


#endif
#ifndef DEL_DEFINED_TYPES_HPP
#define DEL_DEFINED_TYPES_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Types/Variables.hpp"

namespace DEL
{
    //! \class A type that was defined by user code
    class DefinedTypes
    {
    public:

        //! \brief A definition of a type (object)
        struct TypeDefinition
        {
            // Name of the type
            std::string type_name;

            // Variables within the type
            std::vector<VariableNameTypePair> variables;

            // Functions associated with the type
            std::vector<FunctionRepresentation> member_functions;
        };

        //! \brief Check if a type exists given a name
        //! \param name The name to check
        //! \retval True if the name was found, False otherwise
        bool does_type_exist(std::string name)
        {
            for(auto & t : types)
            {
                if(t.type_name == name)
                {
                    return true;
                }
            }
            return false;
        }

        //! \brief Add a type to the list. This does not protect against duplicates
        //!        Ensure that it is unique by checking does_type_exist
        //! \param type The type to add
        void add_type(TypeDefinition type)
        {
            types.push_back(type);
        }

        //! \brief Attempt to get a type by name, if that type doesn't exist
        //!        the program will exit. Ensure it exists using does_type_exist
        //! \param name The name of the type to retrieve
        TypeDefinition get_type(std::string name)
        {
            for(auto & t : types)
            {
                if(t.type_name == name)
                {
                    return t;
                }
            }

            std::cerr << "DefinedTypes::get_type() >> Requested type name \"" << name << "\" not found!" << std::endl;
            exit(EXIT_FAILURE);
        }

    private:
        std::vector<TypeDefinition> types;
    };
}

#endif 
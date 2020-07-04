#ifndef FORGE_VARIABLE_HPP
#define FORGE_VARIABLE_HPP

#include "datatypes/DataType.hpp"
#include <string>

namespace FORGE
{
    //! \brief A variable object
    class Variable
    {
    public:

        //! \brief Construct the variable
        //! \param name The given name of the variable
        //! \param type The datatype of the variable
        Variable(std::string name, DataType type) : name(name), type(type) {}

        //! \brief Retrieve the name
        //! \returns The stringed name 
        std::string  getName() const { return name; }

        //! \brief Retrieve the type
        //! \returns The variable's type
        DataType     getType() const { return type; }

        //! \brief Set the type
        //! \param t The new type to assign
        void setType(DataType t) 
        {
            type = t;
        }

    protected:
        std::string name;
        DataType type;
    };
}


#endif
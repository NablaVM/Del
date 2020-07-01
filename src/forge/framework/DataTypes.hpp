#ifndef FORGE_DATA_TYPES_HPP
#define FORGE_DATA_TYPES_HPP

#include <string>

namespace FORGE
{
    //! \brief A classification to indicate what level of complexity the data type represents
    enum class DataClassificiation
    {
        PRIMITIVE,  // Simple representation
        COMPLEX     // Complex representation
    };

    //! \brief A base class for all data types in the Forge
    class ForgeDataType
    {
    public: 

        //! \brief Create the data type
        //! \param classification The classification of the type (Primitive / complex)
        ForgeDataType(DataClassificiation classification) : 
            classification(classification), type_name("UNDEFINED"){}

        //! \brief Create the data type
        //! \param classification The classification of the type (Primitive / complex)
        //! \param type_name The name of the type
        ForgeDataType(DataClassificiation classification, std::string type_name) : 
            classification(classification), type_name(type_name){}

        DataClassificiation classification;
        std::string type_name;
    };
}


#endif
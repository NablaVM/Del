#ifndef FORGE_REPORT_TYPES_HPP
#define FORGE_REPORT_TYPES_HPP

namespace FORGE
{
    //! \brief Types of reports that can be generated and handled
    enum class ReportType
    {
        SYNTAX,     //! A syntax report
        SEMANTIC,   //! A semantic report
        INTERNAL,   //! An intermal report
        CUSTOM      //! A custom report
    };

}
#endif 
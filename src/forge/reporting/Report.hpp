#ifndef FORGE_REPORT_HPP
#define FORGE_REPORT_HPP

#include <ostream>
#include "ReportTypes.hpp"

namespace FORGE
{
    //! \brief A base for the different reports
    class Report
    {
    public:

        //! \brief Levels of reports
        enum class Level
        {
            WARNING,    //! A Warning report        - non fatal
            INFO,       //! An informational report - non fatal
            ERROR,      //! An error report         - fatal
        };

        //! \brief Create a report
        //! \param report_type  Type of report 
        //! \param report_level The level given to the report
        Report(ReportType report_type, Level report_level) : report_type(report_type), report_level(report_level) {  }

        //! \brief Display the report
        //! \param stream The stream to use for displaying the report
        virtual std::ostream & display( std::ostream & stream ) = 0;

        //! \brief Check if the report should trigger fatality
        //! \returns true if fatal, false otherwise
        bool is_fatal(){ return report_level == Level::ERROR; }

        //! \brief Get the report type
        //! \returns the type of report this object represents
        ReportType get_report_type() const { return report_type; }

    protected:
        ReportType report_type;
        Level report_level;
    };

}

#endif
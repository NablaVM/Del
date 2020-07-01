#ifndef FORGE_CUSTOM_REPORT_HPP
#define FORGE_CUSTOM_REPORT_HPP

#include "Report.hpp"

namespace FORGE
{
    //! \brief A custom report
    class CustomReport : public Report 
    {
    public:
        //! \brief Build a custom report
        //! \param level The level of the report
        //! \param from Originator of the report
        //! \param report Report to display
        CustomReport(Report::Level level, std::string from, std::string report);

        // From Report
        virtual std::ostream & display( std::ostream & stream ) override;

    private:
        std::string from;
        std::string report;
    };
}


#endif 
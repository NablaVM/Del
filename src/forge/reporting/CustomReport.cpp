#include "CustomReport.hpp"
#include "ReportTypes.hpp"

#include <libnabla/termcolor.hpp>

namespace FORGE
{
    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    CustomReport::CustomReport(Report::Level level, std::string from, std::string report) : 
            Report(ReportType::CUSTOM, level), from(from), report(report)
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::ostream & CustomReport::display( std::ostream & stream )
    {
        switch (this->report_level)
        {
        case Report::Level::WARNING:
            stream << "[" <<  termcolor::yellow << "WARNING" << termcolor::reset << "]";
            break;
        
        case Report::Level::INFO:
            stream << "[" <<  termcolor::cyan << "INFO" << termcolor::reset << "]";
            break;
        
        case Report::Level::ERROR:
            stream << "[" <<  termcolor::red << "INFO" << termcolor::reset << "]";
            break;
        default:
            break;
        }

        stream << termcolor::magenta << from << termcolor::reset << " >>" << report << "\n";
        return stream; 
    }
}
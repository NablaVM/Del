#include "InternalReport.hpp"
#include "ReportTypes.hpp"

#include <libnabla/termcolor.hpp>

namespace FORGE
{
    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    InternalReport::InternalReport(InternalReport::InternalInfo information) : 
            Report(ReportType::INTERNAL, Report::Level::ERROR), information(information)
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::ostream & InternalReport::display( std::ostream & stream )
    {
        stream << termcolor::cyan   << "INTERNAL ERROR REPORT\n-------------------------\n" 
               << termcolor::magenta << "Module  : " << termcolor::reset << information.module_name << "\n"
               << termcolor::magenta << "File    : " << termcolor::reset << information.file_name   << "\n"
               << termcolor::magenta << "Method  : " << termcolor::reset << information.method_name << "\n"
               << termcolor::magenta << "Info    : " << termcolor::reset;

        for(auto & i : information.information)
        {
            stream << i << "\n          ";

        }
        stream << "\n\n";
        return stream; 
    }
}
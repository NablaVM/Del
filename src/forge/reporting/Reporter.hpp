#ifndef FORGE_REPORTER_HPP
#define FORGE_REPORTER_HPP

#include "Report.hpp"
#include "CustomReport.hpp"
#include "SyntaxReport.hpp"

namespace FORGE
{
    // Forward definition of the Forge
    class Forge; 

    //! \brief Reporting mechanism
    class Reporter
    {
    public:

        //! \brief Create a reporter
        //! \param forge A reference to the forge for actions triggered by a report
        Reporter(Forge & forge);

        //! \brief Deconstruct the reporter
        ~Reporter();

        //! \brief Issue a report to the user
        //! \param report A report pointer, if null control returns
        //! \post If a report is a "fatal" type, the forge will be notified and
        //!       the error sequence will be kicked off
        void issue_report(Report * report);

    private:
        Forge & forge;

        void handle_syntax_report(SyntaxReport * syntax_report);
        void handle_custom_report(CustomReport * custom_report);
    };
}


#endif
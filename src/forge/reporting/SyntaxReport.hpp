#ifndef FORGE_SYNTAX_REPORT_HPP
#define FORGE_SYNTAX_REPORT_HPP

#include "Report.hpp"

namespace FORGE
{
    //! \brief A syntax report
    class SyntaxReport : public Report 
    {
    public:
        //! \brief Build a syntax report
        //! \param level The level of the report
        //! \param file The file that the syntax error occured in         - If size is 0, no file name displayed
        //! \param line_number Line of input file syntax error is located - If < 0 No line number displayed
        //! \param col The column where error was identified              - If < 0 Whole line will be assumed to be error
        //! \param line The line that contains the error information
        SyntaxReport(Report::Level level, std::string file, int64_t line_number, int col, std::string line);

        // From Report
        virtual std::ostream & display( std::ostream & stream ) override;

    private:
        std::string file;
        int64_t line_number;
        int col;
        std::string line;

        std::string build_error_line();
    };
}


#endif 
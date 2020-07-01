#ifndef FORGE_SEMANTIC_REPORT_HPP
#define FORGE_SEMANTIC_REPORT_HPP

#include "Report.hpp"
#include <vector>

namespace FORGE
{
    //! \brief A semantic report
    class SemanticReport : public Report 
    {
    public:
        //! \brief Build a semantic report
        //! \param level The level of the report
        //! \param file The file that the semantic error occured in         - If size is 0, no file name displayed
        //! \param line_number Line of input file semantic error is located - If < 0 No line number displayed
        //! \param col The column where error was identified              - If < 0 Whole line will be assumed to be error
        //! \param line The line that contains the error information
        //! \param recommendations Any recommendations that are generated for the user
        SemanticReport(Report::Level level, std::string file, int64_t line_number, int col, std::string line, std::vector<std::string> recommendations);

        // From Report
        virtual std::ostream & display( std::ostream & stream ) override;

    private:
        std::string file;
        int64_t line_number;
        int col;
        std::string line;
        std::vector<std::string> recommendations;

        std::string build_error_line();
    };
}


#endif 
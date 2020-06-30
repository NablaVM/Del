#include "SyntaxReport.hpp"
#include "ReportTypes.hpp"

#include <libnabla/termcolor.hpp>

namespace FORGE
{
    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    SyntaxReport::SyntaxReport(Report::Level level, std::string file, int64_t line_number, int col, std::string line) : 
            Report(ReportType::SYNTAX, level), file(file), line_number(line_number), col(col), line(line)
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::ostream & SyntaxReport::display( std::ostream & stream )
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
            stream << "[" <<  termcolor::red << "ERROR" << termcolor::reset << "]";
            break;
        default:
            break;
        }

        // Check if there is information to show regarding the file 
        if(this->file.size() > 0) 
        {
            stream << " (File: " << termcolor::cyan << this->file << termcolor::reset << ")";
        }

        // Check if there is information regarding the line 
        if(this->line_number > 0)
        {
            stream << "(Line : " << termcolor::magenta << std::to_string(this->line_number) << termcolor::reset << ")";
        }

        stream << "\n";

        // If we have a line, then we can build some colored error location stuff (or we can at least try)
        if(this->line.size() > 0)
        {
            stream << line << "\n";

            if(this->is_fatal())
            {
                stream << termcolor::red << build_error_line() << termcolor::reset << "\n";
            }
            else 
            {
                stream << termcolor::yellow << build_error_line() << termcolor::reset << "\n";
            }
        }

        return stream;
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    std::string SyntaxReport::build_error_line() 
    {

        // Edge case
        if(this->line.size() == 1)
        {
            return "^";
        }

        std::string pointer_line;

        if(this->col > 0)
        {
            int start = (this->col < 5) ? 0 : this->col-5;
            int end   = ((uint64_t)(this->col + 5) > this->line.size()) ? this->line.size() : this->col+5;
            for(uint64_t i = 0; i < this->line.size(); i++)
            {
                if(i == this->col)
                {
                    pointer_line += "^";
                }
                else if(i >= start && i <= end )
                {
                    if(i != this->col)
                    {
                        pointer_line += "~";
                    }
                }
                else
                {
                    pointer_line += " ";
                }
            }
        }
        else
        {
            // Place tilde under the line , but not until actual data starts ( skip ws in front of the line )
            bool found_item = false;
            for(uint64_t i = 0; i < this->line.size(); i++)
            {
                if(!found_item && !isspace(line[i]))
                {
                    found_item = true;
                }

                if(found_item)
                {
                    pointer_line += "~";
                }
                else
                {
                    pointer_line += " ";
                }
            }
        }
        return pointer_line;
    }
}
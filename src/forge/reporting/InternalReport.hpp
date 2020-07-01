#ifndef FORGE_INTERNAL_REPORT_HPP
#define FORGE_INTERNAL_REPORT_HPP

#include "Report.hpp"

#include <vector>

namespace FORGE
{
    //! \brief An internal report
    class InternalReport : public Report 
    {
    public:

        //! \brief Information to dump on an internal report
        struct InternalInfo
        {
            std::string module_name;
            std::string file_name;
            std::string method_name;
            std::vector<std::string> information;
        };

        //! \brief Build an internal report - This is a default-fatal report
        //! \param information The information that needs to be dumped
        InternalReport(InternalInfo information);

        // From Report
        virtual std::ostream & display( std::ostream & stream ) override;

    private:
        InternalInfo information;
    };
}


#endif 
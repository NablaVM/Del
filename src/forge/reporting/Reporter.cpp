#include "Reporter.hpp"
#include "Forge.hpp"
#include "ForgeSignal.hpp"

namespace FORGE
{

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Reporter::Reporter(Forge & forge) : forge(forge)
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    Reporter::~Reporter()
    {

    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Reporter::issue_report(Report * report)
    {
        switch(report->get_report_type())
        {
            case ReportType::SYNTAX:   handle_syntax_report  (static_cast<SyntaxReport*>  (report)); break;
            case ReportType::INTERNAL: handle_internal_report(static_cast<InternalReport*>(report)); break;
            case ReportType::SEMANTIC: handle_semantic_report(static_cast<SemanticReport*>(report)); break;
            case ReportType::CUSTOM:   handle_custom_report  (static_cast<CustomReport*>  (report)); break;
            default:
            {
                forge.forge_signal_handler(
                    ForgeSignal(
                        FSignalType::FATAL_ERROR, 
                        "FORGE::Reporter", 
                        "Default accessed when attempting to handle report by type"
                        )
                );
                break;
            }

        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Reporter::handle_custom_report(CustomReport * custom_report)
    {
        // Display the error
        custom_report->display(std::cerr);

        // Send a fatal error signal to forge signal handler
        if(custom_report->is_fatal())
        {
            forge.forge_signal_handler(
                ForgeSignal(
                    FSignalType::FATAL_ERROR, 
                    "FORGE::Reporter::handle_custom_report()", 
                    "Fatal error"
                    )
            );
        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Reporter::handle_internal_report(InternalReport * internal_report)
    {
        // Display the error
        internal_report->display(std::cerr);

        // internal reports trigger kill no matter what
        forge.forge_signal_handler(
            ForgeSignal(
                FSignalType::FATAL_ERROR, 
                "FORGE::Reporter::handle_internal_report()", 
                "Fatal internal error"
                )
        );
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Reporter::handle_semantic_report(SemanticReport * semantic_report)
    {
        // Display the error
        semantic_report->display(std::cerr);

        // Send a fatal error signal to forge signal handler
        if(semantic_report->is_fatal())
        {
            forge.forge_signal_handler(
                ForgeSignal(
                    FSignalType::FATAL_ERROR, 
                    "FORGE::Reporter::handle_semantic_report()", 
                    "Fatal semantic error"
                    )
            );
        }
    }

    // ----------------------------------------------------------
    //
    // ----------------------------------------------------------

    void Reporter::handle_syntax_report(SyntaxReport * syntax_report)
    {
        // Display the error
        syntax_report->display(std::cerr);

        // Send a fatal error signal to forge signal handler
        if(syntax_report->is_fatal())
        {
            forge.forge_signal_handler(
                ForgeSignal(
                    FSignalType::FATAL_ERROR, 
                    "FORGE::Reporter::handle_syntax_report()", 
                    "Fatal syntax error"
                    )
            );
        }
    }

}
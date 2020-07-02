#include "Builder.hpp"
#include "Reporter.hpp"

#include <iostream>

namespace FORGE
{
    Builder::Builder(Forge & forge) : forge(forge)
    {
        
    }

    Builder::~Builder()
    {

    }

    void Builder::build(BuilderInstruction * instruction)
    {
        switch(instruction->builder_type)
        {
        case BuilderType::ARITH:        handle( static_cast<Arith*>(instruction)       ); break;
        case BuilderType::CONDITIONAL:  handle( static_cast<Conditional*>(instruction) ); break;
        case BuilderType::FLOW:         handle( static_cast<Flow*>(instruction)        ); break;
        case BuilderType::ASSIGN:       handle( static_cast<Assign*>(instruction)      ); break;
        case BuilderType::CONSTRUCT:    handle( static_cast<Construct*>(instruction)   ); break; 
        default:
            forge.get_reporter().issue_report(
                new FORGE::InternalReport(
                    {
                        "FORGE::Builder",
                        "Builder.cpp",
                        "build",
                        {
                            "Default accessed when routing builder instruction to handler method"
                        }
                    }
                )
            );
            break;
        }
    }

    void Builder::handle(Arith       * ins)
    {
        std::cout << "BUILDER::handle(arith)" << std::endl;
    }

    void Builder::handle(Conditional * ins)
    {
        std::cout << "BUILDER::handle(Conditional)" << std::endl;
    }

    void Builder::handle(Flow        * ins)
    {
        std::cout << "BUILDER::handle(Flow)" << std::endl;
    }

    void Builder::handle(Assign      * ins)
    {
        std::cout << "BUILDER::handle(Assign)" << std::endl;
    }

    void Builder::handle(Construct   * ins)
    {
        std::cout << "BUILDER::handle(Construct)" << std::endl;
    }
}
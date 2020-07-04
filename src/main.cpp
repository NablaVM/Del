#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#include "del/del_driver.hpp"


namespace
{
    struct Args
    {
        std::string short_arg;
        std::string long_arg;
        std::string description;
    };

    std::vector<Args> DelArguments;
}

int handle_compilation(std::string file);

void show_help();

void show_version();

// --------------------------------------------
// Entry
// --------------------------------------------
    
int main(int argc, char ** argv)
{
    if(argc == 1)
    {
        std::cout << "No input given. Use -h for help" << std::endl;
        return 0;
    }

    DelArguments = {

        { "-h", "--help   ",    "Display help message."},
        { "-v", "--version",    "Display the version of Del." }
    };
    
    std::vector<std::string> args(argv, argv + argc);

    for(int i = 0; i < argc; i++)
    {
        //  Help
        //
        if(args[i] == "-h" || args[i] == "--help")
        {
            show_help();
            return 0;
        }

        // Version info
        //
        if(args[i] == "-v" || args[i] == "--version")
        {
            show_version();
            return 0;
        }
    }

    return handle_compilation(args[1]);
}

// --------------------------------------------
// Compile
// --------------------------------------------
    
int handle_compilation(std::string file)
{
    DEL::DEL_Driver driver;
    driver.parse(file.c_str());

    return 0;
}

// --------------------------------------------
// Show help
// --------------------------------------------
    
void show_help()
{
    std::cout << " Δ Del ∇ | Help" << std::endl 
              << "----------------------------------------------" 
              << std::endl
              << "Short\tLong\t\tDescription" << std::endl
              << "----------------------------------------------"
              << std::endl;

    for(auto & na : DelArguments)
    {
        std::cout << na.short_arg << "\t" << na.long_arg << "\t" << na.description << std::endl;
    }
}

// --------------------------------------------
// Show version
// --------------------------------------------
    
void show_version()
{
    std::cout << " Δ Del ∇ | Version and build information"     << std::endl
              << "-------------------------------------------"  << std::endl 
              << "Del Version  : " << DEL_VERSION_INFO          << std::endl
              << "-------------------------------------------"  << std::endl;
    return;
}
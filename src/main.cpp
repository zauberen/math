#include <iostream>
#include <iterator>
#include <string>

#include "math.h" // contains the math function

void help(); // displays basic usage instructions

int main(int argc, char **argv)
{
    bool displaySteps = false;
    // Startup args parsing
    if (argc > 1)
    {
        bool shouldExit = false;
        // Run each problem and exit after finishing all math problems
        // If a flag is passed, the program will not exit unless problems are given
        for (int i = 1; i < argc; i++)
        {
            std::string arg = std::string(argv[i]);
            if (arg[0] == '-')
            {
                if (arg == "-v")
                {
                    displaySteps = true;
                }
                if (arg == "-h")
                {
                    help();
                    shouldExit = true;
                }
            }
            else {
                std::cout << arg << " = " << doMath(arg, displaySteps) << std::endl;
                shouldExit = true;
            }
        }
        if (shouldExit)
        {
            return 0;
        }
    }
    // Variables
    // Minor constants (only used here)
    const double VERSION = 0.20;
    const std::string DESCRIPTION = "2024-06-22";
    // Normal variables
    std::string sInput; // Used to grab and store user input

    std::cout << "CMath version: " << VERSION << " " << DESCRIPTION << std::endl;
    std::cout << "For basic usage help send \"H\"" << std::endl;

    while (true)
    {
        std::cout << "> ";
        getline(std::cin,sInput);

        if (sInput == "H")
        {
            help();
        }
        else if (sInput == "E" || sInput == "Q")
        {
            std::cout << "Exiting Application..." << std::endl;
            return 0;
        }
        else if (sInput == "S")
        {
            setVariable(); // Sets a permanent variable
        }
        else
        {
            std::cout << doMath(sInput, displaySteps) << std::endl; // Parses the statement and returns a result into the variable a
        }
    }
    return 0;
}

void help()
{
    std::cout << "cmath [-v -h] [1+1*1/1%1^1*(1+1)]" << std::endl
              << "Command flags:" << std::endl
              << "v :: Show work (verbose output)" << std::endl
              << "h :: Show this help output and exit" << std::endl
              << "Instructions:" << std::endl
              << "Either pass math as arguments, each arg will be processed separately in order, or launch math with no arguments to enter interactive mode." << std::endl
              << "Results are automatically dumped into the variable 'answer' and printed to the screen" << std::endl
              << "Commands in interactive mode:" << std::endl
              << "H = Basic usage information" << std::endl
              << "E or Q = Exit the application" << std::endl
              << "S = Set a variable (note: variables are stored for use between sessions)" << std::endl
              << std::endl
              << "This application is licensed under the terms of the GPLv3." << std::endl
              << "To report bugs/issues:" << std::endl
              << "Submit an issue at: www.github.com/dandreas/math" << std::endl;
}

#include <iostream>
#include <string>

#include "math.h" // contains the math function

void help(); // displays basic usage instructions

int main(int argc, char **argv)
{
    // Startup args parsing
    if (argc > 0)
    {
        if (DEBUG)
        for(int i = 0; i < argc; i++)
        {
            std::cout << i << ": " << argv[i] << std::endl;
        }
    }
    // Variables
    // Minor constants (only used here)
    const double VERSION = 0;
    const std::string DESCRIPTION = "Alpha Build";
    // Normal variables
    std::string sInput; // Used to grab and store user input

    std::cout << "Math version: " << VERSION << " " << DESCRIPTION << std::endl;
    std::cout << "For basic usage help send \"H\"" << std::endl;
    
    while(true)
    {
        std::cout << "> ";
        getline(std::cin,sInput);

        if (sInput[0] == 'H')
        {
            help();
        }
        else if (sInput[0] == 'E')
        {
            std::cout << "Exiting Application..." << std::endl;
            return 0;
        }
        else if (sInput[0] == 'S')
        {
            setVariable(); // Sets a permanent variable
        }
        else
        {
            doMath(sInput, false); // Parses the statement and returns a result into the variable a
        }
    }
}

void help()
{
    std::cout << "Basic usage: <value> <modifier(*/+-)> <value>" << std::endl
        << "Results are automatically printed into the variable 'a' and printed to the screen" << std::endl
        << "Commands: (note: if a command is sent, it must be the first char)" << std::endl
        << "H = Basic usage information" << std::endl
        << "E = Exit the application" << std::endl
        << "S = Set a variable (note: variables are stored for use between sessions)" << std::endl
        << "Note: variable 'a' should not be used, as it will be overwritten by the next expression's result" << std::endl
        << std::endl
        << "For more documentation on using this application, consult the source repo's documentation" << std::endl
        << "This application is licensed under the terms of the GPLv3." << std::endl
        << "To report bugs/issues:" << std::endl 
        << "Submit an issue at: www.github.com/dandreas/math" << std::endl
        << "or email the dev at: belfieldcecil@gmail.com" << std::endl;
}

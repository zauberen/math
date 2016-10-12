#include <iostream>
#include <stdio.h>
#include <sstream>
#include <ctype.h>
#include <string>
#include <vector>

#include "math.h"

double dVariables[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // where [0] = a
char cOperators[] = {'*','/','+','-','%','^','='};

double doMath(std::string sProblem)
{
    double dSolution = 0; // Stores the solution
    std::string sParsedProblem = ""; // This stores the parsed version of sProblem.
    bool bDoubleOperator = false; // Indicates that an operator precedes the char currently being parsed. (this is for spacing purposes)

    for(int i = 0; i < sProblem.length(); i++)
    {
        bool bDone = false; // streamlines following for if finished before all operators have been checked.

        if (isdigit(sProblem[i]) || sProblem[i] == '.')
        {
            sParsedProblem += sProblem[i];

            bDoubleOperator = false;
        }
        else if (islower(sProblem[i]))
        {
            sParsedProblem += readVariable(sProblem[i]);
            
            bDoubleOperator = false;
        }
        else if (sProblem[i] == '(')
        {
            std::string sParensProblem = "";

            for (int n = i + 1; sProblem[n] != ')' && n < sProblem.length(); n++)
            {
                sParensProblem += sProblem[n];
                i++;
            }
            sParsedProblem += doMath(sParensProblem);
            
            bDoubleOperator = false;
        }
        else
        {
            // Checks if the item is an operator, if so, adds a space.
            for (int n = 0; n < sizeof(cOperators); n++)
            {
                if (sProblem[i] == cOperators[n])
                {
                    if (DEBUG)
                    {
                        std::cout << "<Debug feed>" << std::endl
                            << n << " hits on operators." << std::endl
                            << "sizeof: " << sizeof(cOperators) << std::endl
                            << "</Debug feed>" << std::endl;
                    }

                    if (bDoubleOperator)
                    {
                        sParsedProblem += sProblem[i];
                        sParsedProblem += " ";
                    }
                    else
                    {
                        sParsedProblem += " ";
                        sParsedProblem += sProblem[i];
                        sParsedProblem += " ";
                    }
            
                    bDoubleOperator = true;
                    bDone = true;
                }
            }

            if (DEBUG)
            {
                std::cout << "<Debug feed>" << std::endl
                    << "Operators loop was hit successfully" << std::endl
                    << "</Debug feed>" << std::endl;
            }
        }
        
        if (DEBUG)
        {
            std::cout << "<Debug feed>" << std::endl
                << "i = " << i << std::endl
                << "sParsedProblem = " << sParsedProblem << std::endl
                << "</Debug feed>" << std::endl;
        }
    }
    
    if (DEBUG)
    {
        std::cout << "<Debug feed>" << std::endl
            << "End of logical portion of doMath..." << std::endl
            << "input: " << sProblem << std::endl
            << "output: " << sParsedProblem << std::endl
            << "</Debug feed>" << std::endl;
    }


    // Throws the parsed string to the solve function to actually solve the math
    dSolution = solve(sParsedProblem);

    setVariable('a',dSolution);

    return dSolution;
}

double solve(std::string sParsedProblem)
{
    double dSolution = 0; // Stores the solution, returned at the end
    double dTempLeft = 0; // Stores the temporary value that represents the (left) or (first) operative number (*1* + 1)
    double dTempRight = 0; // Stores the temporary value that represents the (right) or (second) operative number (1 + *1*)
    int count = 0; // Used in the debug feed
    bool bSetter = true; // true = left, false = right
    char cOperator; // stores the most recently used operator
    std::string sRaw = ""; // Stores the raw output of stringstream
    std::stringstream ss;
    
    ss << sParsedProblem;
    // First for parse: Solve multiplication, division, and mod
    while (ss >> sRaw)
    {
        if (DEBUG)
        {
            std::cout << count << ": " << sRaw << std::endl;
            count++;
        }
        
        if (isdigit(sRaw[0]) || sRaw[0] == '.')
        {
            if (bSetter)
            {
                dTempLeft = std::stod(sRaw);
                bSetter = false;
            }
            else
            {
                dTempRight = std::stod(sRaw);

                // Now do the appropriate calculation for the two items.
                dSolution += calculate(dTempLeft,dTempRight,cOperator);
                bSetter = true;
            }
        }
        else
        {
            for(int i = 0; i < 2; i++)
            {
                if (sRaw[0] == cOperators[i])
                {
                    cOperator = cOperators[i];
                }
            }
        }
    }
}

double calculate(double dLeft, double dRight, char cOperator)
{
    double dSolution = 0;

    if (cOperator == cOperators[0])
    { // Multiplication
        dSolution = dLeft * dRight;
    }
    else if (cOperator == cOperators[1])
    { // Division
        dSolution = dLeft / dRight;
    }
    else if (cOperator == cOperators[2])
    { // Addition
        dSolution = dLeft + dRight;
    }
    else if (cOperator == cOperators[3])
    { // Subtraction
        dSolution = dLeft - dRight;
    }
    else if (cOperator == cOperators[4])
    { // Mod
        std::cout << "Not ready" << std::endl;
    }
    else if (cOperator == cOperators[5])
    { // Power
        std::cout << "Not ready" << std::endl;
    }
    else if (cOperator == cOperators[6])
    { // is (=)
        std::cout << "Not ready" << std::endl;
    }

    return dSolution;
}

double readVariable(char cVariable)
{
    int iIndex = 0;

    if (islower(cVariable))
    {
        iIndex = cVariable - 97;

        return dVariables[iIndex];
    }
    else
    {
        std::cout << "Invalid variable: " << cVariable << std::endl
            << "Variables must be lowercase alphanumeric characters" << std::endl;
    }

    return iIndex; // This is for bug testing
}

void setVariable()
{
    std::string sInput; // Used for user input
    char cVar; // Used as a reference for the final cout
    int iIndex = 0; // Stores the array location (dVariables)

    std::cout << "character: ";
    getline(std::cin,sInput);
    
    if(islower(sInput[0]))
    {
        iIndex = sInput[0] - '0';
        cVar = sInput[0];
    }
    else
    {
        std::cout << "Invalid Input: " << sInput << std::endl
            << "Input must be a lowercase alphanumeric character" << std::endl;
        return; // exits the function
    }
    
    std::cout << "value: ";
    getline(std::cin,sInput);
    dVariables[iIndex] = std::stod(sInput);

    std::cout << "Variable " << cVar << " set to " << dVariables[iIndex] << std::endl;
}

bool setVariable(char cVariable,double dValue)
{
    int iIndex = 0;

    if(islower(cVariable))
    {
        iIndex = cVariable - 97;
    }
    else
    {
        std::cout << "Invalid Input: " << cVariable << std::endl
            << "Input must be a lowercase alphanumeric character" << std::endl;
        return false; // exits the function
    }

    dVariables[iIndex] = dValue;
    
    std::cout << "var " << cVariable << " = " << dVariables[iIndex] << std::endl;
    return true;
}

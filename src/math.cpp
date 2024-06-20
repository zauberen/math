#include <iostream>
#include <stdio.h>
#include <sstream>
#include <ctype.h>
#include <cmath>
#include <string>
#include <vector>

#include "math.h"

// Stores the 26 variables that this can store (a to z)
double dVariables[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // where [0] = a
// Operators
char cOperators[] = {
    '^',
    '*',
    '/',
    '%',
    '+',
    '-',
    '='
};
const int POWER = 0;
const int MULTIPLY = 1;
const int DIVIDE = 2;
const int MODULUS = 3;
const int ADD = 4;
const int SUBTRACT = 5;
const int SET = 6;

double doMath(std::string sProblem, bool displaySteps)
{
    double dSolution = 0; // Stores the solution
    std::string sParsedProblem = ""; // Store the normalised problem string
    bool bDoubleOperator = false; // Indicates that an operator precedes the char currently being parsed. (this is for spacing purposes)

    for(int i = 0; i < sProblem.length(); i++)
    {
        bool bDone = false; // streamlines following for if finished before all operators have been checked.

        // Check for numbers
        if (isdigit(sProblem[i]) || sProblem[i] == '.')
        {
            sParsedProblem += sProblem[i];

            bDoubleOperator = false;
        }
        // Check for variables
        else if (islower(sProblem[i]))
        {
            sParsedProblem += std::to_string(readVariable(sProblem[i]));

            bDoubleOperator = false;
        }
        // Check for sub-problems
        else if (sProblem[i] == '(')
        {
            std::string sParensProblem = "";

            for (int n = i + 1; sProblem[n] != ')' && n < sProblem.length(); n++)
            {
                sParensProblem += sProblem[n];
                i++;
            }
            sParsedProblem += std::to_string(doMath(sParensProblem, displaySteps));

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
    dSolution = solve(sParsedProblem, displaySteps);

    setVariable('a',dSolution);

    return dSolution;
}

double solve(std::string sParsedProblem, bool displaySteps)
{
    std::string sPartiallySolvedProblem = ""; // Store the problem as it gets simplified
    double dSolution = 0; // Stores the solution, returned at the end

    sPartiallySolvedProblem = simplifyProblem(sParsedProblem, POWER, POWER);
    if (sPartiallySolvedProblem[0] != ' ')
    {
        if (displaySteps)
        {
            std::cout << sPartiallySolvedProblem << std::endl;
        }
        sPartiallySolvedProblem = simplifyProblem(sPartiallySolvedProblem, MULTIPLY, MODULUS);
        if (displaySteps)
        {
            std::cout << sPartiallySolvedProblem << std::endl;
        }
    }
    dSolution = std::stod(simplifyProblem(sPartiallySolvedProblem, ADD, SUBTRACT));

    return dSolution;
}

std::string simplifyProblem(std::string sParsedProblem, int operatorLowIdx,
                       int operatorHighIdx) {
    double dTempLeft = 0; // Stores the temporary value that represents the (left) or (first) operative number (*1* + 1)
    double dTempRight = 0; // Stores the temporary value that represents the (right) or (second) operative number (1 + *1*)
    int count = 0; // Used in the debug feed
    bool bSetter = true; // true = left, false = right
    char cOperator = ':'; // stores the most recently used operator
    char cRawOperator = ':';
    std::string sRaw = ""; // Stores the raw output of stringstream
    std::string sSimplifiedProblem = ""; // Store the simplified problem
    std::stringstream ss;

    ss << sParsedProblem;
    while (ss >> sRaw)
    {
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
                // Check if the math should be done now or later
                if (cOperator != ':') {
                    // Set the left operand to the solution and continue parsing the problem
                    dTempLeft = calculate(dTempLeft,dTempRight,cOperator);
                    // Set the operator to an invalid character so we can check whether it is valid next time
                    cOperator = ':';
                }
                else {
                    // Operator is invalid, so we add the raw operator back to the problem and try again next time
                    sSimplifiedProblem += std::to_string(dTempLeft) + " " + cRawOperator + " ";
                    // Set the right operand to the left operand and continue
                    dTempLeft = dTempRight;
                }
            }
        }
        else
        {
            // Cache the raw operator, we may want this to put the problem back together
            cRawOperator = sRaw[0];

            for(int i = operatorLowIdx; i < operatorHighIdx; i++)
            {
                if (sRaw[0] == cOperators[i])
                {
                    cOperator = cOperators[i];
                }
            }
        }
    }

    sSimplifiedProblem += std::to_string(dTempLeft);

    return sSimplifiedProblem;
}

double calculate(double dLeft, double dRight, char cOperator)
{
    double dSolution = 0;

    if (cOperator == POWER)
    { // Power
        std::cout << "Not ready" << std::endl;
    }
    else if (cOperator == MULTIPLY)
    { // Multiplication
        dSolution = dLeft * dRight;
    }
    else if (cOperator == DIVIDE)
    { // Division
        dSolution = dLeft / dRight;
    }
    else if (cOperator == MODULUS)
    { // Mod
        dSolution = std::fmod(dLeft, dRight);
    }
    else if (cOperator == ADD)
    { // Addition
        dSolution = dLeft + dRight;
    }
    else if (cOperator == SUBTRACT)
    { // Subtraction
        dSolution = dLeft - dRight;
    }
    else if (cOperator == SET)
    { // is (=)
        std::cout << "Not ready" << std::endl;
    }

    if (DEBUG)
    {
        std::cout << dLeft << " " << cOperator << " " << dRight << " = " << dSolution << std::endl;
    }

    return dSolution;
}

double readVariable(char cVariable)
{
    int iIndex = 0;

    if (islower(cVariable))
    {
        iIndex = cVariable - 97;
        std::cout << "Index: " << iIndex << std::endl;

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
        iIndex = sInput[0] - 97;
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

bool setVariable(char cVariable, double dValue)
{
    int iIndex = 0;

    if (islower(cVariable))
    {
        iIndex = cVariable - 97;
        std::cout << "Index: " << iIndex << std::endl;
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

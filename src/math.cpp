#include <_ctype.h>
#include <cctype>
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
// Whether to show "work" when processing a request
bool displaySteps = false;
// Operators
char cOperators[] = {'^', '*', '/', '%', '+', '-', '='};
const int POWER = 0;
const int MULTIPLY = 1;
const int DIVIDE = 2;
const int MODULUS = 3;
const int ADD = 4;
const int SUBTRACT = 5;
const int SET = 6;

double doMath(std::string sProblem, bool showSteps)
{
    displaySteps = showSteps;
    return doMath(sProblem);
}

double doMath(std::string sProblem)
{
    double dSolution = 0; // Stores the solution
    bool bDoubleOperator = false; // Indicates that an operator precedes the char currently being parsed. (this is for spacing purposes)
    std::string sParsedProblem = ""; // Store the normalised problem string

    for(unsigned long i = 0; i < sProblem.length(); i++)
    {
        // Check for numbers
        if (isdigit(sProblem[i]) || sProblem[i] == '.')
        {
            sParsedProblem += sProblem[i];
            bDoubleOperator = false;
        }
        // Check for variables
        else if (islower(sProblem[i]))
        {
            sParsedProblem += sProblem[i];
            bDoubleOperator = false;
        }
        // Check for sub-problems
        else if (sProblem[i] == '(')
        {
            std::string sParensProblem = "";

            for (unsigned long n = i + 1; sProblem[n] != ')' && n < sProblem.length(); n++)
            {
                sParensProblem += sProblem[n];
                i++;
            }
            sParsedProblem += std::to_string(doMath(sParensProblem));

            bDoubleOperator = false;
        }
        else
        {
            // Checks if the item is an operator, if so, adds a space.
            for (unsigned long n = 0; n < sizeof(cOperators); n++)
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
    setVariable('a', dSolution);

    return dSolution;
}

double solve(std::string sParsedProblem)
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
    sPartiallySolvedProblem = simplifyProblem(sPartiallySolvedProblem, ADD, SUBTRACT);
    if (displaySteps)
    {
        std::cout << sPartiallySolvedProblem << std::endl;
    }
    sPartiallySolvedProblem = simplifyProblem(sPartiallySolvedProblem, SET, SET);
    // Handle situations where just a variable is passed, just print the value of the variable
    if (std::islower(sPartiallySolvedProblem[0]))
    {
        dSolution = readVariable(sPartiallySolvedProblem[0]);
    }
    else
    {
        dSolution = std::stod(sPartiallySolvedProblem);
    }

    return dSolution;
}

std::string simplifyProblem(std::string sParsedProblem, int operatorLowIdx,
                       int operatorHighIdx) {
    bool bSetter = true; // true = left, false = right
    char cOperator = ':'; // Stores the most recently used operator (if valid)
    char cRawOperator = ':'; // Stores the most recently used operator
    std::string sTempLeft = "0"; // Stores the temporary value that represents the (left) or (first) operative number (*1* + 1)
    std::string sTempRight = "0"; // Stores the temporary value that represents the (right) or (second) operative number (1 + *1*)
    std::string sRaw = ""; // Stores the raw output of stringstream
    std::string sSimplifiedProblem = ""; // Store the simplified problem
    std::stringstream ss;

    ss << sParsedProblem;
    while (ss >> sRaw)
    {
        // Handle numbers and variables
        if (islower(sRaw[0]) || isdigit(sRaw[0]) || sRaw[0] == '.')
        {
            if (bSetter)
            {
                sTempLeft = sRaw;
                bSetter = false;
            }
            else
            {
                sTempRight = sRaw;
                // Check if the math should be done now or later
                if (cOperator != ':')
                {
                    // Set the left operand to the solution and continue parsing the problem
                    sTempLeft = calculate(sTempLeft,sTempRight,cOperator);
                    // Set the operator to an invalid character so we can check whether it is valid next time
                    cOperator = ':';
                }
                else
                {
                    // Operator is invalid, so we add the raw operator back to the problem and try again next time
                    sSimplifiedProblem += sTempLeft + " " + cRawOperator + " ";
                    // Set the right operand to the left operand and continue
                    sTempLeft = sTempRight;
                }
            }
        }
        // Handle operators
        else
        {
            // Cache the raw operator, we may want this to put the problem back together
            cRawOperator = sRaw[0];
            // Check if this is a valid operator for this loop
            for(int i = operatorLowIdx; i <= operatorHighIdx; i++)
            {
                if (sRaw[0] == cOperators[i])
                {
                    cOperator = cOperators[i];
                }
            }
        }
    }

    sSimplifiedProblem += sTempLeft;

    return sSimplifiedProblem;
}

std::string calculate(std::string sLeft, std::string sRight, char cOperator)
{
    double dSolution = 0;
    double dLeft = 0;
    double dRight = 0;

    // Get the right operand as a double
    if (islower(sRight[0]))
    {
        dRight = readVariable(sRight[0]);
    }
    else
    {
        dRight = std::stod(sRight);
    }

    // Handle SET separately since dLeft cannot be set
    if (cOperator == cOperators[SET])
    { // is (=)
        setVariable(sLeft[0], dRight);
        dSolution = dRight;
    }
    else
    {
        // Get the left operand as a double
        if (islower(sLeft[0]))
        {
            dLeft = readVariable(sLeft[0]);
        }
        else
        {
            dLeft = std::stod(sLeft);
        }
    }

    if (cOperator == cOperators[POWER])
    { // Power
        dSolution = std::pow(dLeft, dRight);
    }
    else if (cOperator == cOperators[MULTIPLY])
    { // Multiplication
        dSolution = dLeft * dRight;
    }
    else if (cOperator == cOperators[DIVIDE])
    { // Division
        dSolution = dLeft / dRight;
    }
    else if (cOperator == cOperators[MODULUS])
    { // Mod
        dSolution = std::fmod(dLeft, dRight);
    }
    else if (cOperator == cOperators[ADD])
    { // Addition
        dSolution = dLeft + dRight;
    }
    else if (cOperator == cOperators[SUBTRACT])
    { // Subtraction
        dSolution = dLeft - dRight;
    }

    if (DEBUG)
    {
        std::cout << dLeft << " " << cOperator << " " << dRight << " = " << dSolution << std::endl;
    }

    return std::to_string(dSolution);
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
    }
    else
    {
        std::cout << "Invalid Input: " << cVariable << std::endl
            << "Input character must be a valid alpha character and on the left side of assignment (b = 1 + 2)" << std::endl;
        return false; // exits the function
    }

    dVariables[iIndex] = dValue;

    if (displaySteps)
    {
        std::cout << "var " << cVariable << " = " << dVariables[iIndex] << std::endl;
    }
    return true;
}

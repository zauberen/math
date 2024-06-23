#include <iostream>
#include <stdio.h>
#include <sstream>
#include <cmath>
#include <string>
#include <map>

#include "math.h"

// Stores an unlimited amount of variables
std::map<std::string, double> mVariables;
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
        // Check for sub-problems
        if (sProblem[i] == '(')
        {
            int parenCount = 0;
            std::string sParensProblem = "";
            for (unsigned long n = i + 1;
                 n < sProblem.length()
                     && ((parenCount == 0 && sProblem[n] != ')') || parenCount > 0); n++)
            {
                sParensProblem += sProblem[n];
                if (sProblem[n] == '(') parenCount += 1;
                if (sProblem[n] == ')') parenCount -= 1;
                i++;
            }
            // Skip the closing parentheses
            i++;

            // Solve the problem and add the result to the parsed problem
            double parenSolution = doMath(sParensProblem);
            sParsedProblem += std::to_string(parenSolution);
            if (displaySteps)
            {
                std::cout << sParensProblem  << " = " << parenSolution << std::endl;
            }
            bDoubleOperator = false;
        }
        // Handle numbers/variables
        else if (!isOperator(sProblem[i]))
        {
            sParsedProblem += sProblem[i];
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
    // Avoid double printing when assigning the answer
    if (displaySteps) {
        displaySteps = false;
        setVariable("answer", dSolution);
        displaySteps = true;
    }
    else {
        setVariable("answer", dSolution);
    }

    return dSolution;
}

double solve(std::string sParsedProblem)
{
    std::string sPartiallySolvedProblem = ""; // Store the problem as it gets simplified
    std::string sCachedOutput = ""; // Store the prior output for when displaying steps
    double dSolution = 0; // Stores the solution, returned at the end

    sPartiallySolvedProblem = simplifyProblem(sParsedProblem, POWER, POWER);
    if (sPartiallySolvedProblem[0] != ' ')
    {
        if (displaySteps)
        {
            std::cout << sPartiallySolvedProblem << std::endl;
            sCachedOutput = sPartiallySolvedProblem;
        }
        sPartiallySolvedProblem = simplifyProblem(sPartiallySolvedProblem, MULTIPLY, MODULUS);
        if (displaySteps && sCachedOutput != sPartiallySolvedProblem)
        {
            std::cout << sPartiallySolvedProblem << std::endl;
            sCachedOutput = sPartiallySolvedProblem;
        }
    }
    sPartiallySolvedProblem = simplifyProblem(sPartiallySolvedProblem, ADD, SUBTRACT);
    if (displaySteps && sCachedOutput != sPartiallySolvedProblem)
    {
        std::cout << sPartiallySolvedProblem << std::endl;
    }
    sPartiallySolvedProblem = simplifyProblem(sPartiallySolvedProblem, SET, SET);
    // Handle situations where just a variable is passed, just print the value of the variable
    if (!isNumber(sPartiallySolvedProblem))
    {
        dSolution = readVariable(sPartiallySolvedProblem);
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
        if (!isOperator(sRaw))
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
    if (!isNumber(sRight))
    {
        dRight = readVariable(sRight);
    }
    else
    {
        dRight = std::stod(sRight);
    }

    // Handle SET separately since dLeft cannot be set
    if (cOperator == cOperators[SET])
    { // is (=)
        setVariable(sLeft, dRight);
        dSolution = dRight;
    }
    else
    {
        // Get the left operand as a double
        if (!isNumber(sLeft))
        {
            dLeft = readVariable(sLeft);
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

double readVariable(std::string sVariable)
{
    if (mVariables.count(sVariable))
    {
        return mVariables[sVariable];
    }
    else
    {
        std::cout << "Invalid variable: " << sVariable << std::endl
                  << "Variables must be assigned before use, defaulting to 0" << std::endl;
    }
    return 0;
}

void setVariable()
{
    std::string sName;
    std::string sValue;

    // Get the values to enter into the variable map
    std::cout << "variable name: ";
    getline(std::cin,sName);
    std::cout << "value: ";
    getline(std::cin,sValue);
    // Update the map and notify the user
    if (isNumber(sValue) && setVariable(sName, std::stod(sValue))) {
        std::cout << "Variable " << sName << " set to " << mVariables[sName] << std::endl;
    }
}

bool setVariable(std::string sVariable, double dValue)
{
    // Check if the variable sent in is just a number
    if (isNumber(sVariable))
    {
        std::cout << "Invalid input: " << sVariable << std::endl
                  << "Input character must be a valid string and on the left side of assignment (b = 1 + 2)" << std::endl;
        return false; // exits the function
    }

    mVariables[sVariable] = dValue;
    if (displaySteps)
    {
        std::cout << sVariable << " = " << mVariables[sVariable] << std::endl;
    }
    return true;
}

bool isNumber(std::string s)
{
    return !s.empty() && s.find_first_not_of("0123456789.") == std::string::npos;
}

bool isOperator(std::string s)
{
    return !s.empty() && s.find_first_not_of("^*/%+-=") == std::string::npos;
}

bool isOperator(char s)
{
    return isOperator(std::string() + s);
}

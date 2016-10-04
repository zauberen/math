#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>

#include "math.h"

double dVariables[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // where [0] = a

double doMath(std::string sProblem,bool bRecursive)
{
    double dSolution = 0;
    std::vector<double> dvNumbers;
    std::vector<char> cvOperators;
    double dTemp; // stores the current number being read
    int iTens = 1; // stores the current place for the number being read
    bool bReading = false; // keeps track of whether a number is being read or not (true = reading number)
    bool bOperator = true; // makes sure there is an operator after each number (true = operator found/valid syntax)
    bool bDecimal = false; // makes sure there is only one decimal in a number (true = decimal in num)

    // first for: Iterates through the characters of the string
    // dumping numbers into the dvNumbers vector
    // and dumping operators into the cvOperators vector
    // This also checks for syntactical errors,
    // and when such an error is encountered, the function
    // returns 0
    for(int i = sProblem.length() - 1; i >= 0; i--)
    {
        if (isdigit(sProblem[i]))
        {
            // syntax check
            if (!bReading)
            if (!bOperator)
            {
                std::cout << "Error: Missing operator" << std::endl;
                return 0;
            }
            
            // ticks related booleans
            bReading = true;
            bOperator = false;

            // adds number by the assumed place to the temp variable
            dTemp += (sProblem[i] - '0') * iTens;
            iTens = iTens * 10;
        }
        else if (sProblem[i] == '.' && bReading == true)
        { // sets decimals
            if (bDecimal)
            {
                std::cout << "Invalid syntax: too many decimals" << std::endl;
            }
            else
            {
                dTemp = dTemp/iTens;
                bDecimal = true;
            }
        }
        else if (islower(sProblem[i]))
        { // Grabs variables
            // tick booleans
            bReading = false;
            bDecimal = false;
            
            // If a variable is placed next to a number without an operator, multiplication is assumed
            if (!bOperator)
            {
                cvOperators.push_back('*');
                bOperator = true;
            }
            
            // Pushes variable to the number vector
            dvNumbers.push_back(readVariable(sProblem[i]));
        }
        else if (sProblem[i] == ')')
        {
            // Backs up the sProblem string
            std::string sTemp = sProblem;
            // Removes already calculated parts of the string
            sProblem.erase(sProblem.end(),sProblem.begin()+(i+1));
            // Sends the problem back recursively
            dvNumbers.push_back(doMath(sProblem, true));
            // Restores the backup
            sProblem = sTemp;

            // Recalculate i
            for (int n = sProblem.length() - 1; sProblem[i] != '('; n--)
            { 
                i--;
            }

            bReading = false;
            bDecimal = false;
        }
        else if (sProblem[i] == '(' && bRecursive == true)
        {
            // This is only executed if the function is called recursively.
            // it assumes the recursive part is done, calculates then returns the results
            dSolution = solve(cvOperators,dvNumbers);
            return dSolution;
        }
        else if (sProblem[i] == '(')
        {
            // Assumes improper parens syntax
            std::cout << "Improper syntax: unclosed parens" << std::endl;
            return 0;
        }
        else if (sProblem[i] == '*' || sProblem[i] == '/' || sProblem[i] == '+' || sProblem[i] == '-' || sProblem[i] == '%')
        {
            // Some error checking
            if (bOperator)
            {
                std::cout << "Invalid syntax: too many operators" << std::endl;
                return 0;
            }

            // ticks booleans
            bOperator = true;
            bReading = false;

            // Pushes operators to the operator vector
            cvOperators.push_back(sProblem[i]);
        }
        else
        {
            if (sProblem[i] != ' ')
            {
                std::cout << "char: " << sProblem[i] << " was ignored." << std::endl;
            }

            bReading = false;
            bDecimal = false;
        }
    }

    dSolution = solve(cvOperators,dvNumbers); // does the math

    if (bRecursive == false)
    {
        // Set the answer variable 'a' to dSolution
        dVariables[0] = dSolution;
    }
    return dSolution;
}

// Takes the completed table of numbers and operators and
// actually does the math
double solve(std::vector<char> cvOperators, std::vector<double> dvNumbers)
{
    double dSolution = 0;

    // This part does all division and multiplication and mod
    for (int i = cvOperators.size() - 1; i >= 0; i++)
    {
        if (cvOperators[i] == '*')
        {
            dvNumbers[i - 1] = dvNumbers[i] * dvNumbers[i-1];
        }
        else if (cvOperators[i] == '/')
        {
            dvNumbers[i - 1] = dvNumbers[i] / dvNumbers[i-1];
        }
        else if (cvOperators[i] == '%')
        {
            std::cout << "Mod is not allowed currently" << std::endl;
            // dvNumbers[i - 1] = dvNumbers[i] % dvNumbers[i-1];
            return 0;
        }
    }
    // Remove irrelevant numbers and operators from the vectors
    for (int i = cvOperators.size() - 1; i >= 0; i++)
    {
        if (cvOperators[i] == '*' || cvOperators[i] == '/')
        {
            dvNumbers.erase(dvNumbers.begin()+i);
            cvOperators.erase(cvOperators.begin()+i);
        }
    }
    
    // Next, do addition and subtraction
    for (int i = cvOperators.size() - 1; i >= 0; i++)
    {
        if (cvOperators[i] == '+')
        {
            dvNumbers[i - 1] = dvNumbers[i] + dvNumbers[i-1];
        }
        else if (cvOperators[i] == '-')
        {
            dvNumbers[i - 1] = dvNumbers[i] - dvNumbers[i-1];
        }
    }
    // Add them all up
    for (int i = dvNumbers.size() - 1; i >= 0; i++)
    {
        dSolution += dvNumbers[i];
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
    int iIndex = 0; // Stores the array location (dVariables)

    std::cout << "character: ";
    getline(std::cin,sInput);
    
    if(islower(sInput[0]))
    {
        iIndex = sInput[0] - 97;
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

    std::cout << "Variable " << sInput[0] << " set to " << dVariables[iIndex] << std::endl;
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

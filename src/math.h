#ifndef MATH_H
#define MATH_H
#include <map>
// Global variables
const bool DEBUG = false; // only useful for development
extern std::map<std::string, double> mVariables; // Contains all variables
extern bool displaySteps; // Whether to show "work" when processing a request (math.cpp)

/// This function set is operated linearly (doMath>solve>simplifyProblem>calculate)
///
// Parses the string into an easier to cut string and sends to solve
double doMath(std::string sProblem);
double doMath(std::string sProblem, bool showSteps);
// Parses the string into basic math (sends to calculate) then finds the final solution
double solve(std::string sParsedProblem);
// Simplifies the problem for each step of PEMDAS, specify the inclusive low and high steps
std::string simplifyProblem(std::string sParsedProblem, int operatorLowIdx, int operatorHighIdx);
// Function for solving a simple mathematical operation
std::string calculate(std::string sLeft, std::string sRight, char cOperator);


// Getter and setter functions for the iVariables array
double readVariable(std::string sVariable);
void setVariable();
// Overload for setting variables during math calculations
bool setVariable(std::string sVariable, double dValue);
// Check if the string given is a number (incl float)
bool isNumber(std::string s);
// Check if a string is an operator
bool isOperator(std::string s);
bool isOperator(char s);

#endif

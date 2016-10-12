#ifndef MATH_H
#define MATH_H

#include <vector>

// Global variables
const bool DEBUG = true; // only useful for development

extern double dVariables[]; // contains 26 vars, value in math.cpp


/// This function set is operated linearly (doMath>solve>calculate)
/// The functions get progressively simpler as you go down the line
/// 
// Parses the string into an easier to cut string and sends to solve
double doMath(std::string sProblem); 
// Parses the string into basic math (sends to calculate) then finds the final solution
double solve(std::string sParsedProblem);
// Function for solving a simple mathematical operation
double calculate(double dLeft, double dRight, char cOperator);


// Getter and setter functions for the iVariables array
double readVariable(char cVariable);
void setVariable();
// Overload for setting variables during math calculations
bool setVariable(char cVariable,double dValue);


#endif

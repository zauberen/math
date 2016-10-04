#ifndef MATH_H
#define MATH_H

#include <vector>

// Global variables
const bool DEBUG = true; // only useful for development

extern double dVariables[]; // contains 26 vars, value in math.cpp

double doMath(std::string sInput, bool bRecursive); // This function both parses a string and does the operation

// Getter and setter functions for the iVariables array
double readVariable(char cVariable);
void setVariable();
// Overload for setting variables during math calculations
bool setVariable(char cVariable,double dValue);

// Function for solving parsed strings (only for use in doMath
double solve(std::vector<char> cvOperators, std::vector<double> dvNumbers);

#endif

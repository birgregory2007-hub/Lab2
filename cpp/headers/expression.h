#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
using namespace std;

// задание 1, вариант 1: значение арифметического выражения (два стека)
bool EvalExpression(string expr, long long& result, string& error);
void RunEval(string expr);    // команда EVAL

#endif

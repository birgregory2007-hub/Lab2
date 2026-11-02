#include <iostream>
#include "../headers/expression.h"
#include "../headers/common.h"
#include "../headers/stack.h"
#include "../headers/utils.h"

static const long long LIMIT = 2000000000LL;

static int Prec(string op)
{
    if (op == "*")
        return 2;
    return 1;
}

static bool ApplyTop(Stack& vals, Stack& ops, string& error)
{
    string op;
    string bs;
    string as;
    if (!StackPop(ops, op) || !StackPop(vals, bs) || !StackPop(vals, as))
    {
        error = "invalid expression";
        return false;
    }
    long long a;
    long long b;
    long long r;
    ParseLL(as, a);
    ParseLL(bs, b);
    if (op == "+")
        r = a + b;
    else if (op == "-")
        r = a - b;
    else
        r = a * b;
    if (r > LIMIT || r < -LIMIT)
    {
        error = "value exceeds 2*10^9";
        return false;
    }
    StackPush(vals, to_string(r));
    return true;
}

bool EvalExpression(string expr, long long& result, string& error)
{
    Stack vals;
    Stack ops;
    StackInit(vals);
    StackInit(ops);
    bool ok = true;
    bool expectOperand = true;
    int i = 0;
    while (ok && i < (int)expr.size())
    {
        char c = expr[i];
        if (c == ' ' || c == '\t')
        {
            i++;
            continue;
        }
        if (expectOperand)
        {
            if (c >= '0' && c <= '9')
            {
                int j = i;
                while (j < (int)expr.size() && expr[j] >= '0' && expr[j] <= '9')
                    j++;
                string num = expr.substr(i, j - i);
                long long v;
                if (num.size() > 10 || !ParseLL(num, v) || v > LIMIT)
                {
                    error = "number exceeds 2*10^9";
                    ok = false;
                }
                else
                {
                    StackPush(vals, num);
                    expectOperand = false;
                    i = j;
                }
            }
            else if (c == '(')
            {
                StackPush(ops, "(");
                i++;
            }
            else if (c == '-' || c == '+')    // унарный знак: 0 - x или 0 + x
            {
                StackPush(vals, "0");
                StackPush(ops, string(1, c));
                i++;
            }
            else
            {
                if (c == ')' || c == '*')
                    error = "operand expected";
                else
                    error = "unexpected symbol";
                ok = false;
            }
        }
        else
        {
            if (c == ')')
            {
                bool found = false;
                string top;
                while (StackPeek(ops, top))
                {
                    if (top == "(")
                    {
                        found = true;
                        break;
                    }
                    if (!ApplyTop(vals, ops, error))
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok && !found)
                {
                    error = "unbalanced parentheses";
                    ok = false;
                }
                if (ok)
                {
                    StackPop(ops, top);
                    i++;
                }
            }
            else if (c == '+' || c == '-' || c == '*')
            {
                string cur(1, c);
                string top;
                while (StackPeek(ops, top) && top != "(" && Prec(top) >= Prec(cur))
                {
                    if (!ApplyTop(vals, ops, error))
                    {
                        ok = false;
                        break;
                    }
                }
                if (ok)
                {
                    StackPush(ops, cur);
                    expectOperand = true;
                    i++;
                }
            }
            else if ((c >= '0' && c <= '9') || c == '(')
            {
                error = "operator expected";
                ok = false;
            }
            else
            {
                error = "unexpected symbol";
                ok = false;
            }
        }
    }
    if (ok && expectOperand)
    {
        error = "unexpected end of expression";
        ok = false;
    }
    string top;
    while (ok && StackPeek(ops, top))
    {
        if (top == "(")
        {
            error = "unbalanced parentheses";
            ok = false;
            break;
        }
        if (!ApplyTop(vals, ops, error))
            ok = false;
    }
    if (ok)
    {
        string res;
        if (!StackPop(vals, res) || !StackIsEmpty(vals))
        {
            error = "invalid expression";
            ok = false;
        }
        else
            ParseLL(res, result);
    }
    StackFree(vals);
    StackFree(ops);
    return ok;
}

void RunEval(string expr)
{
    long long result = 0;
    string error;
    if (EvalExpression(expr, result, error))
        cout << "-> " << result << endl;
    else
        Error(error);
}

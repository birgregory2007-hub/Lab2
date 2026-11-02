#include <algorithm>
#include <cstdio>
#include "../headers/utils.h"

bool ToInt(string s, int& value)
{
    if (s.size() == 0 || s.size() > 9)
        return false;
    int i = 0;
    bool minus = false;
    if (s[0] == '-')
    {
        minus = true;
        i = 1;
        if (s.size() == 1)
            return false;
    }
    int result = 0;
    for (; i < (int)s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
        result = result * 10 + (s[i] - '0');
    }
    if (minus)
        value = -result;
    else
        value = result;
    return true;
}

string ToUpper(string s)
{
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (s[i] >= 'a' && s[i] <= 'z')
            s[i] = s[i] - 'a' + 'A';
    }
    return s;
}

bool ParseLL(string s, long long& value)
{
    int i = 0;
    bool minus = false;
    if (s.size() == 0)
        return false;
    if (s[0] == '-' || s[0] == '+')
    {
        minus = (s[0] == '-');
        i = 1;
    }
    if (i >= (int)s.size() || (int)s.size() - i > 18)
        return false;
    long long result = 0;
    for (; i < (int)s.size(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            return false;
        result = result * 10 + (s[i] - '0');
    }
    value = minus ? -result : result;
    return true;
}

bool LessItem(string a, string b)
{
    long long x;
    long long y;
    if (ParseLL(a, x) && ParseLL(b, y))
        return x < y;
    return a < b;
}

void SortArray(Array& a)
{
    sort(a.data, a.data + a.size, LessItem);
}

string JoinArray(Array& a, string sep)
{
    string result = "";
    for (int i = 0; i < a.size; i++)
    {
        if (i > 0)
            result = result + sep;
        result = result + a.data[i];
    }
    return result;
}

unsigned long long Fnv1a(string s)
{
    unsigned long long h = 1469598103934665603ULL;
    for (int i = 0; i < (int)s.size(); i++)
    {
        h = h ^ (unsigned char)s[i];
        h = h * 1099511628211ULL;
    }
    return h;
}

string Fixed2(double x)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%.2f", x);
    return string(buf);
}

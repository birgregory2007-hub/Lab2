#include <iostream>
#include "../headers/foldhash.h"
#include "../headers/common.h"

static string KeyDigits(string key)
{
    bool numeric = key.size() > 0;
    for (int i = 0; i < (int)key.size(); i++)
    {
        if (key[i] < '0' || key[i] > '9')
            numeric = false;
    }
    if (numeric)
        return key;
    string result = "";
    for (int i = 0; i < (int)key.size(); i++)
    {
        int code = (unsigned char)key[i];
        result = result + (char)('0' + code / 100);
        result = result + (char)('0' + (code / 10) % 10);
        result = result + (char)('0' + code % 10);
    }
    return result;
}

unsigned long long FoldHash(string key)
{
    string digits = KeyDigits(key);
    unsigned long long sum = 0;
    for (int i = 0; i < (int)digits.size(); i += 3)
    {
        unsigned long long part = 0;
        for (int j = i; j < i + 3 && j < (int)digits.size(); j++)
            part = part * 10 + (unsigned long long)(digits[j] - '0');
        sum = sum + part;
    }
    return sum;
}

string FoldTrace(string key)
{
    string digits = KeyDigits(key);
    string result = "";
    for (int i = 0; i < (int)digits.size(); i += 3)
    {
        if (i > 0)
            result = result + "+";
        result = result + digits.substr(i, 3);
    }
    return result + " = " + to_string(FoldHash(key));
}

void RunFold(Array& w)
{
    if (CheckArgs(w, 2))
        cout << "-> " << FoldTrace(w.data[1]) << endl;
}

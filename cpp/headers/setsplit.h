#ifndef SETSPLIT_H
#define SETSPLIT_H

#include <string>
#include "array.h"
#include "set.h"
using namespace std;

// задание 3, вариант 1: разбиение множества натуральных чисел на подмножества с равной суммой
// в groups попадают строки вида "{1, 4, 10}"
bool SplitEqualSum(Set& s, long long target, Array& groups, string& error);
// все суммы, при которых разбиение возможно: "sum 15 (2 subsets): {..} {..}"
bool SplitAllSums(Set& s, Array& lines, string& error);

void RunSetSplit(Array& w, Set& s);    // команда SETSPLIT

#endif

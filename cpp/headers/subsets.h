#ifndef SUBSETS_H
#define SUBSETS_H

#include <string>
#include "array.h"
using namespace std;

// задание 4, вариант 1: все различные подмассивы (наборы элементов массива)
bool AllDistinctSubsets(Array& items, Array& out, string& error);
void ShowSubsets(Array& items);    // команда MSUBS

#endif

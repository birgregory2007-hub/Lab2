#ifndef COMMON_H
#define COMMON_H

#include <string>
#include "array.h"
using namespace std;

// общие функции модуля database, нужные и модулю lab2
void Error(string text);
bool CheckArgs(Array& w, int count);
void ShowItems(Array& items);
void Replace(Array& items, Array& fresh);
void ShowJoined(Array& a, string sep);    // реализация в common.cpp

#endif

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "array.h"
using namespace std;

// перевод строки в целое число, false если строка не число
bool ToInt(string s, int& value);

// перевод строки в верхний регистр
string ToUpper(string s);

// ---- лаб. 2 ----
bool ParseLL(string s, long long& value);        // строгий разбор целого числа (до 18 цифр)
bool LessItem(string a, string b);               // числа - по значению, иначе по алфавиту
void SortArray(Array& a);                        // сортировка массива по LessItem
string JoinArray(Array& a, string sep);
unsigned long long Fnv1a(string s);              // хеш строки FNV-1a
string Fixed2(double x);                         // число с двумя знаками после запятой

#endif

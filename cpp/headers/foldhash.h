#ifndef FOLDHASH_H
#define FOLDHASH_H

#include <string>
#include "array.h"
using namespace std;

// хеш-функция методом свёртки: ключ режется на группы по 3 цифры, группы складываются
unsigned long long FoldHash(string key);
string FoldTrace(string key);    // "523+456+795 = 1774"
void RunFold(Array& w);          // команда HFOLD

#endif

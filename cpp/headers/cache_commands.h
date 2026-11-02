#ifndef CACHE_COMMANDS_H
#define CACHE_COMMANDS_H

#include "array.h"
using namespace std;

void RunCache(Array& w, Array& items, bool isNew, bool& changed);    // тип C
void RunCacheBatch(Array& w);                                       // команда CRUN

#endif

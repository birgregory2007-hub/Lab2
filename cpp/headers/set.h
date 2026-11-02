#ifndef SET_H
#define SET_H

#include <string>
#include "array.h"
#include "flist.h"
using namespace std;

// АТД «Множество»: хеш-таблица с цепочками. Корзины - односвязные списки FList (лаб. 1)
struct Set
{
    FList* buckets;
    int bucketCount;
    int size;
};

void SetInit(Set& s);
void SetFree(Set& s);
bool SetAdd(Set& s, string value);      // false, если элемент уже есть
bool SetDel(Set& s, string value);      // false, если элемента нет
bool SetAt(Set& s, string value);       // проверка наличия элемента
void SetToArray(Set& s, Array& out);    // элементы по возрастанию

#endif

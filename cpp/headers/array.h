#ifndef ARRAY_H
#define ARRAY_H

#include <string>
using namespace std;

struct Array
{
    string* data;
    int size;
    int capacity;
};

void ArrayInit(Array& a);
void ArrayFree(Array& a);
void ArrayClear(Array& a);   // лаб. 2: очистка массива с сохранением ёмкости 4
void ArrayPush(Array& a, string value);
bool ArrayInsert(Array& a, int index, string value);
bool ArrayGet(const Array& a, int index, string& value);
bool ArraySet(Array& a, int index, string value);
bool ArrayDelete(Array& a, int index, string& removed);

// лаб. 2: страховка от утечек, освобождает массив при выходе из функции
struct ArrayGuard
{
    Array& a;
    explicit ArrayGuard(Array& arr) : a(arr)
    {
        ArrayInit(a);
    }
    ~ArrayGuard()
    {
        ArrayFree(a);
    }
};

#endif

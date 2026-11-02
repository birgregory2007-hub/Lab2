#include "../headers/array.h"

void ArrayInit(Array& a)
{
    a.capacity = 4;
    a.size = 0;
    a.data = new string[a.capacity];
}

void ArrayFree(Array& a)
{
    delete[] a.data;
    a.data = 0;
    a.size = 0;
    a.capacity = 0;
}

void ArrayClear(Array& a)
{
    ArrayFree(a);
    ArrayInit(a);
}

static void ArrayGrow(Array& a)
{
    int newCapacity = a.capacity * 2;
    string* newData = new string[newCapacity];
    for (int i = 0; i < a.size; i++)
        newData[i] = a.data[i];
    delete[] a.data;
    a.data = newData;
    a.capacity = newCapacity;
}

void ArrayPush(Array& a, string value)
{
    if (a.size == a.capacity)
        ArrayGrow(a);
    a.data[a.size] = value;
    a.size++;
}

bool ArrayInsert(Array& a, int index, string value)
{
    if (index < 0 || index > a.size)
        return false;
    if (a.size == a.capacity)
        ArrayGrow(a);
    for (int i = a.size; i > index; i--)
        a.data[i] = a.data[i - 1];
    a.data[index] = value;
    a.size++;
    return true;
}

bool ArrayGet(const Array& a, int index, string& value)
{
    if (index < 0 || index >= a.size)
        return false;
    value = a.data[index];
    return true;
}

bool ArraySet(Array& a, int index, string value)
{
    if (index < 0 || index >= a.size)
        return false;
    a.data[index] = value;
    return true;
}

bool ArrayDelete(Array& a, int index, string& removed)
{
    if (index < 0 || index >= a.size)
        return false;
    removed = a.data[index];
    for (int i = index; i < a.size - 1; i++)
        a.data[i] = a.data[i + 1];
    a.size--;
    return true;
}

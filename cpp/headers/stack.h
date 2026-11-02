#ifndef STACK_H
#define STACK_H

#include <string>
#include "array.h"
using namespace std;

struct SNode
{
    string data;
    SNode* next;
};

struct Stack
{
    SNode* top;
    int size;
};

void StackInit(Stack& s);
void StackFree(Stack& s);
bool StackIsEmpty(Stack& s);
void StackPush(Stack& s, string value);
bool StackPop(Stack& s, string& value);
bool StackPeek(Stack& s, string& value);
void StackToArray(Stack& s, Array& out);   // от вершины ко дну

#endif

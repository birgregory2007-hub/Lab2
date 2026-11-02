#include "../headers/stack.h"

void StackInit(Stack& s)
{
    s.top = 0;
    s.size = 0;
}

void StackFree(Stack& s)
{
    string temp;
    while (StackPop(s, temp))
    {
    }
}

bool StackIsEmpty(Stack& s)
{
    return s.top == 0;
}

void StackPush(Stack& s, string value)
{
    SNode* n = new SNode;
    n->data = value;
    n->next = s.top;
    s.top = n;
    s.size++;
}

bool StackPop(Stack& s, string& value)
{
    if (s.top == 0)
        return false;
    SNode* n = s.top;
    value = n->data;
    s.top = n->next;
    delete n;
    s.size--;
    return true;
}

bool StackPeek(Stack& s, string& value)
{
    if (s.top == 0)
        return false;
    value = s.top->data;
    return true;
}

void StackToArray(Stack& s, Array& out)
{
    for (SNode* p = s.top; p != 0; p = p->next)
        ArrayPush(out, p->data);
}

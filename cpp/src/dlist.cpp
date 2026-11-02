#include <iostream>
#include "../headers/dlist.h"

void DListInit(DList& l)
{
    l.head = 0;
    l.tail = 0;
    l.size = 0;
}

void DListFree(DList& l)
{
    DNode* p = l.head;
    while (p != 0)
    {
        DNode* next = p->next;
        delete p;
        p = next;
    }
    DListInit(l);
}

static DNode* NewNode(string value)
{
    DNode* n = new DNode;
    n->data = value;
    n->prev = 0;
    n->next = 0;
    return n;
}

static DNode* FindNode(DList& l, string value)
{
    DNode* p = l.head;
    while (p != 0 && p->data != value)
        p = p->next;
    return p;
}

static void RemoveNode(DList& l, DNode* n)
{
    if (n->prev != 0)
        n->prev->next = n->next;
    else
        l.head = n->next;
    if (n->next != 0)
        n->next->prev = n->prev;
    else
        l.tail = n->prev;
    delete n;
    l.size--;
}

void DListAddHead(DList& l, string value)
{
    DNode* n = NewNode(value);
    n->next = l.head;
    if (l.head != 0)
        l.head->prev = n;
    else
        l.tail = n;
    l.head = n;
    l.size++;
}

void DListAddTail(DList& l, string value)
{
    DNode* n = NewNode(value);
    n->prev = l.tail;
    if (l.tail != 0)
        l.tail->next = n;
    else
        l.head = n;
    l.tail = n;
    l.size++;
}

bool DListAddBefore(DList& l, string target, string value)
{
    DNode* p = FindNode(l, target);
    if (p == 0)
        return false;
    DNode* n = NewNode(value);
    n->next = p;
    n->prev = p->prev;
    if (p->prev != 0)
        p->prev->next = n;
    else
        l.head = n;
    p->prev = n;
    l.size++;
    return true;
}

bool DListAddAfter(DList& l, string target, string value)
{
    DNode* p = FindNode(l, target);
    if (p == 0)
        return false;
    DNode* n = NewNode(value);
    n->prev = p;
    n->next = p->next;
    if (p->next != 0)
        p->next->prev = n;
    else
        l.tail = n;
    p->next = n;
    l.size++;
    return true;
}

bool DListDelHead(DList& l, string& removed)
{
    if (l.head == 0)
        return false;
    removed = l.head->data;
    RemoveNode(l, l.head);
    return true;
}

bool DListDelTail(DList& l, string& removed)
{
    if (l.tail == 0)
        return false;
    removed = l.tail->data;
    RemoveNode(l, l.tail);
    return true;
}

bool DListDelBefore(DList& l, string target, string& removed)
{
    DNode* p = FindNode(l, target);
    if (p == 0 || p->prev == 0)
        return false;
    removed = p->prev->data;
    RemoveNode(l, p->prev);
    return true;
}

bool DListDelAfter(DList& l, string target, string& removed)
{
    DNode* p = FindNode(l, target);
    if (p == 0 || p->next == 0)
        return false;
    removed = p->next->data;
    RemoveNode(l, p->next);
    return true;
}

bool DListDelValue(DList& l, string value)
{
    DNode* p = FindNode(l, value);
    if (p == 0)
        return false;
    RemoveNode(l, p);
    return true;
}

bool DListGet(DList& l, int index, string& value)
{
    if (index < 0 || index >= l.size)
        return false;
    DNode* p = l.head;
    for (int i = 0; i < index; i++)
        p = p->next;
    value = p->data;
    return true;
}

bool DListFind(DList& l, string value)
{
    return FindNode(l, value) != 0;
}

void DListPrint(DList& l)
{
    for (DNode* p = l.head; p != 0; p = p->next)
    {
        cout << p->data;
        if (p->next != 0)
            cout << " ";
    }
    cout << endl;
}

void DListPrintRev(DList& l)
{
    for (DNode* p = l.tail; p != 0; p = p->prev)
    {
        cout << p->data;
        if (p->prev != 0)
            cout << " ";
    }
    cout << endl;
}

void DListToArray(DList& l, Array& out)
{
    for (DNode* p = l.head; p != 0; p = p->next)
        ArrayPush(out, p->data);
}

void DListRemoveNode(DList& l, DNode* n)
{
    RemoveNode(l, n);
}

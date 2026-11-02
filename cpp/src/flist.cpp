#include <iostream>
#include "../headers/flist.h"

void FListInit(FList& l)
{
    l.head = 0;
    l.tail = 0;
    l.size = 0;
}

void FListFree(FList& l)
{
    Node* p = l.head;
    while (p != 0)
    {
        Node* next = p->next;
        delete p;
        p = next;
    }
    FListInit(l);
}

static Node* NewNode(string value)
{
    Node* n = new Node;
    n->data = value;
    n->next = 0;
    return n;
}

static Node* FindNode(FList& l, string value)
{
    Node* p = l.head;
    while (p != 0 && p->data != value)
        p = p->next;
    return p;
}

void FListAddHead(FList& l, string value)
{
    Node* n = NewNode(value);
    n->next = l.head;
    l.head = n;
    if (l.tail == 0)
        l.tail = n;
    l.size++;
}

void FListAddTail(FList& l, string value)
{
    Node* n = NewNode(value);
    if (l.tail == 0)
    {
        l.head = n;
        l.tail = n;
    }
    else
    {
        l.tail->next = n;
        l.tail = n;
    }
    l.size++;
}

bool FListAddBefore(FList& l, string target, string value)
{
    if (l.head == 0)
        return false;
    if (l.head->data == target)
    {
        FListAddHead(l, value);
        return true;
    }
    // ищем узел, следующий за которым равен target
    Node* p = l.head;
    while (p->next != 0 && p->next->data != target)
        p = p->next;
    if (p->next == 0)
        return false;
    Node* n = NewNode(value);
    n->next = p->next;
    p->next = n;
    l.size++;
    return true;
}

bool FListAddAfter(FList& l, string target, string value)
{
    Node* p = FindNode(l, target);
    if (p == 0)
        return false;
    Node* n = NewNode(value);
    n->next = p->next;
    p->next = n;
    if (p == l.tail)
        l.tail = n;
    l.size++;
    return true;
}

bool FListDelHead(FList& l, string& removed)
{
    if (l.head == 0)
        return false;
    Node* p = l.head;
    removed = p->data;
    l.head = p->next;
    if (l.head == 0)
        l.tail = 0;
    delete p;
    l.size--;
    return true;
}

bool FListDelTail(FList& l, string& removed)
{
    if (l.head == 0)
        return false;
    if (l.head == l.tail)
        return FListDelHead(l, removed);
    // назад ходить нельзя, поэтому идём до предпоследнего узла
    Node* p = l.head;
    while (p->next != l.tail)
        p = p->next;
    removed = l.tail->data;
    delete l.tail;
    p->next = 0;
    l.tail = p;
    l.size--;
    return true;
}

bool FListDelBefore(FList& l, string target, string& removed)
{
    if (l.head == 0 || l.head->data == target)
        return false;
    if (l.head->next != 0 && l.head->next->data == target)
        return FListDelHead(l, removed);
    Node* p = l.head;
    while (p->next != 0 && p->next->next != 0 && p->next->next->data != target)
        p = p->next;
    if (p->next == 0 || p->next->next == 0)
        return false;
    Node* d = p->next;
    removed = d->data;
    p->next = d->next;
    delete d;
    l.size--;
    return true;
}

bool FListDelAfter(FList& l, string target, string& removed)
{
    Node* p = FindNode(l, target);
    if (p == 0 || p->next == 0)
        return false;
    Node* d = p->next;
    removed = d->data;
    p->next = d->next;
    if (d == l.tail)
        l.tail = p;
    delete d;
    l.size--;
    return true;
}

bool FListDelValue(FList& l, string value)
{
    if (l.head == 0)
        return false;
    if (l.head->data == value)
    {
        string temp;
        return FListDelHead(l, temp);
    }
    Node* p = l.head;
    while (p->next != 0 && p->next->data != value)
        p = p->next;
    if (p->next == 0)
        return false;
    Node* d = p->next;
    p->next = d->next;
    if (d == l.tail)
        l.tail = p;
    delete d;
    l.size--;
    return true;
}

bool FListGet(FList& l, int index, string& value)
{
    if (index < 0 || index >= l.size)
        return false;
    Node* p = l.head;
    for (int i = 0; i < index; i++)
        p = p->next;
    value = p->data;
    return true;
}

bool FListFind(FList& l, string value)
{
    return FindNode(l, value) != 0;
}

void FListPrint(FList& l)
{
    for (Node* p = l.head; p != 0; p = p->next)
    {
        cout << p->data;
        if (p->next != 0)
            cout << " ";
    }
    cout << endl;
}

// вспомогательная рекурсия: сначала уходим в конец, потом печатаем
static void PrintBack(Node* n, Node* head)
{
    if (n == 0)
        return;
    PrintBack(n->next, head);
    cout << n->data;
    if (n != head)
        cout << " ";
}

void FListPrintRev(FList& l)
{
    PrintBack(l.head, l.head);
    cout << endl;
}

void FListToArray(FList& l, Array& out)
{
    for (Node* p = l.head; p != 0; p = p->next)
        ArrayPush(out, p->data);
}

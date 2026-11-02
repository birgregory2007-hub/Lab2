#ifndef DLIST_H
#define DLIST_H

#include <string>
#include "array.h"
using namespace std;

struct DNode
{
    string data;
    DNode* prev;
    DNode* next;
};

struct DList
{
    DNode* head;
    DNode* tail;
    int size;
};

void DListInit(DList& l);
void DListFree(DList& l);

void DListAddHead(DList& l, string value);
void DListAddTail(DList& l, string value);
bool DListAddBefore(DList& l, string target, string value);
bool DListAddAfter(DList& l, string target, string value);

bool DListDelHead(DList& l, string& removed);
bool DListDelTail(DList& l, string& removed);
bool DListDelBefore(DList& l, string target, string& removed);
bool DListDelAfter(DList& l, string target, string& removed);
bool DListDelValue(DList& l, string value);

bool DListGet(DList& l, int index, string& value);
bool DListFind(DList& l, string value);
void DListPrint(DList& l);
void DListPrintRev(DList& l);
void DListToArray(DList& l, Array& out);
void DListRemoveNode(DList& l, DNode* n);   // лаб. 2: удаление известного узла за O(1)

#endif

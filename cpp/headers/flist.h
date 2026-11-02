#ifndef FLIST_H
#define FLIST_H

#include <string>
#include "array.h"
using namespace std;

struct Node
{
    string data;
    Node* next;
};

struct FList
{
    Node* head;
    Node* tail;
    int size;
};

void FListInit(FList& l);
void FListFree(FList& l);

// добавление: голова, хвост, перед элементом, после элемента
void FListAddHead(FList& l, string value);
void FListAddTail(FList& l, string value);
bool FListAddBefore(FList& l, string target, string value);
bool FListAddAfter(FList& l, string target, string value);

// удаление: голова, хвост, перед элементом, после элемента, по значению
bool FListDelHead(FList& l, string& removed);
bool FListDelTail(FList& l, string& removed);
bool FListDelBefore(FList& l, string target, string& removed);
bool FListDelAfter(FList& l, string target, string& removed);
bool FListDelValue(FList& l, string value);

// чтение и поиск
bool FListGet(FList& l, int index, string& value);
bool FListFind(FList& l, string value);
void FListPrint(FList& l);
void FListPrintRev(FList& l);
void FListToArray(FList& l, Array& out);

#endif

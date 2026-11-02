#ifndef QUEUE_H
#define QUEUE_H

#include <string>
#include "array.h"
using namespace std;

struct QNode
{
    string data;
    QNode* next;
};

struct Queue
{
    QNode* head;
    QNode* tail;
    int size;
};

void QueueInit(Queue& q);
void QueueFree(Queue& q);
void QueuePush(Queue& q, string value);
bool QueuePop(Queue& q, string& value);
bool QueuePeek(Queue& q, string& value);
void QueueToArray(Queue& q, Array& out);   // от головы к хвосту

#endif

#include "../headers/queue.h"

void QueueInit(Queue& q)
{
    q.head = 0;
    q.tail = 0;
    q.size = 0;
}

void QueueFree(Queue& q)
{
    string temp;
    while (QueuePop(q, temp))
    {
    }
}

void QueuePush(Queue& q, string value)
{
    QNode* n = new QNode;
    n->data = value;
    n->next = 0;
    if (q.tail == 0)
    {
        q.head = n;
        q.tail = n;
    }
    else
    {
        q.tail->next = n;
        q.tail = n;
    }
    q.size++;
}

bool QueuePop(Queue& q, string& value)
{
    if (q.head == 0)
        return false;
    QNode* n = q.head;
    value = n->data;
    q.head = n->next;
    if (q.head == 0)
        q.tail = 0;
    delete n;
    q.size--;
    return true;
}

bool QueuePeek(Queue& q, string& value)
{
    if (q.head == 0)
        return false;
    value = q.head->data;
    return true;
}

void QueueToArray(Queue& q, Array& out)
{
    for (QNode* p = q.head; p != 0; p = p->next)
        ArrayPush(out, p->data);
}

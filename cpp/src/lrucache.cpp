#include "../headers/lrucache.h"
#include "../headers/utils.h"

static int BucketOf(LruCache& c, string key)
{
    return (int)(Fnv1a(key) % (unsigned long long)c.bucketCount);
}

static CacheEntry* FindEntry(LruCache& c, string key)
{
    for (CacheEntry* e = c.buckets[BucketOf(c, key)]; e != 0; e = e->next)
    {
        if (e->key == key)
            return e;
    }
    return 0;
}

static void UnlinkEntry(LruCache& c, CacheEntry* target)
{
    int b = BucketOf(c, target->key);
    CacheEntry* prev = 0;
    CacheEntry* cur = c.buckets[b];
    while (cur != 0 && cur != target)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur == 0)
        return;
    if (prev != 0)
        prev->next = cur->next;
    else
        c.buckets[b] = cur->next;
}

static void Touch(LruCache& c, CacheEntry* e)
{
    DListRemoveNode(c.order, e->node);
    DListAddTail(c.order, e->key);
    e->node = c.order.tail;
}

bool LruInit(LruCache& c, int capacity, string& error)
{
    if (capacity < 1 || capacity > 10000000)
    {
        error = "capacity must be in range 1..10000000";
        return false;
    }
    c.capacity = capacity;
    c.size = 0;
    DListInit(c.order);
    c.bucketCount = 2 * capacity + 1;
    c.buckets = new CacheEntry*[c.bucketCount];
    for (int i = 0; i < c.bucketCount; i++)
        c.buckets[i] = 0;
    return true;
}

void LruFree(LruCache& c)
{
    for (int i = 0; i < c.bucketCount; i++)
    {
        CacheEntry* e = c.buckets[i];
        while (e != 0)
        {
            CacheEntry* next = e->next;
            delete e;
            e = next;
        }
    }
    delete[] c.buckets;
    c.buckets = 0;
    c.bucketCount = 0;
    DListFree(c.order);
    c.size = 0;
}

void LruSet(LruCache& c, string key, string value)
{
    CacheEntry* e = FindEntry(c, key);
    if (e != 0)
    {
        e->value = value;
        Touch(c, e);
        return;
    }
    if (c.size == c.capacity)
    {
        DNode* oldest = c.order.head;
        CacheEntry* victim = FindEntry(c, oldest->data);
        UnlinkEntry(c, victim);
        DListRemoveNode(c.order, oldest);
        delete victim;
        c.size--;
    }
    e = new CacheEntry;
    e->key = key;
    e->value = value;
    DListAddTail(c.order, key);
    e->node = c.order.tail;
    int b = BucketOf(c, key);
    e->next = c.buckets[b];
    c.buckets[b] = e;
    c.size++;
}

bool LruGet(LruCache& c, string key, string& value)
{
    CacheEntry* e = FindEntry(c, key);
    if (e == 0)
        return false;
    value = e->value;
    Touch(c, e);
    return true;
}

void LruToArray(LruCache& c, Array& out)
{
    for (DNode* p = c.order.head; p != 0; p = p->next)
    {
        CacheEntry* e = FindEntry(c, p->data);
        ArrayPush(out, e->key);
        ArrayPush(out, e->value);
    }
}

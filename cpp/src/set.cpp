#include "../headers/set.h"
#include "../headers/utils.h"

static void AllocBuckets(Set& s, int count)
{
    s.buckets = new FList[count];
    for (int i = 0; i < count; i++)
        FListInit(s.buckets[i]);
    s.bucketCount = count;
}

static int BucketOf(Set& s, string value)
{
    return (int)(Fnv1a(value) % (unsigned long long)s.bucketCount);
}

static void Rehash(Set& s)
{
    FList* old = s.buckets;
    int oldCount = s.bucketCount;
    AllocBuckets(s, oldCount * 2);
    for (int i = 0; i < oldCount; i++)
    {
        for (Node* p = old[i].head; p != 0; p = p->next)
            FListAddHead(s.buckets[BucketOf(s, p->data)], p->data);
        FListFree(old[i]);
    }
    delete[] old;
}

void SetInit(Set& s)
{
    s.size = 0;
    AllocBuckets(s, 8);
}

void SetFree(Set& s)
{
    for (int i = 0; i < s.bucketCount; i++)
        FListFree(s.buckets[i]);
    delete[] s.buckets;
    s.buckets = 0;
    s.bucketCount = 0;
    s.size = 0;
}

bool SetAt(Set& s, string value)
{
    return FListFind(s.buckets[BucketOf(s, value)], value);
}

bool SetAdd(Set& s, string value)
{
    FList& bucket = s.buckets[BucketOf(s, value)];
    if (FListFind(bucket, value))
        return false;
    FListAddHead(bucket, value);
    s.size++;
    if (s.size * 4 > s.bucketCount * 3)
        Rehash(s);
    return true;
}

bool SetDel(Set& s, string value)
{
    if (!FListDelValue(s.buckets[BucketOf(s, value)], value))
        return false;
    s.size--;
    return true;
}

void SetToArray(Set& s, Array& out)
{
    for (int i = 0; i < s.bucketCount; i++)
        FListToArray(s.buckets[i], out);
    SortArray(out);
}

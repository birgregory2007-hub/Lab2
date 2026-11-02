#include "../headers/hashtable.h"
#include "../headers/utils.h"
#include "../headers/foldhash.h"

const char* TOMBSTONE = "\x02";
static const char SEP = '\x1f';

bool IsPrime(int n)
{
    if (n < 2)
        return false;
    for (int d = 2; (long long)d * d <= n; d++)
    {
        if (n % d == 0)
            return false;
    }
    return true;
}

bool HashTableInit(HashTable& t, ProbeMode mode, int capacity, string& error)
{
    if (capacity < 2 || capacity > 10000000)
    {
        error = "capacity must be in range 2..10000000";
        return false;
    }
    if (mode == PROBE_DOUBLE && !IsPrime(capacity))
    {
        error = "capacity must be a prime number for double hashing";
        return false;
    }
    t.mode = mode;
    t.capacity = capacity;
    t.size = 0;
    ArrayInit(t.slots);
    for (int i = 0; i < capacity; i++)
        ArrayPush(t.slots, "");
    return true;
}

void HashTableFree(HashTable& t)
{
    ArrayFree(t.slots);
    t.size = 0;
    t.capacity = 0;
}

int ProbeStep(HashTable& t, unsigned long long fold)
{
    if (t.mode == PROBE_LINEAR)
        return 1;
    return (int)(1 + fold % (unsigned long long)(t.capacity - 1));
}

int ProbeIndex(HashTable& t, unsigned long long fold, int attempt)
{
    unsigned long long m = (unsigned long long)t.capacity;
    unsigned long long h1 = fold % m;
    unsigned long long step = (unsigned long long)ProbeStep(t, fold);
    return (int)((h1 + (unsigned long long)attempt * step) % m);
}

string SlotKey(string slot)
{
    size_t p = slot.find(SEP);
    return slot.substr(0, p);
}

string SlotValue(string slot)
{
    size_t p = slot.find(SEP);
    return slot.substr(p + 1);
}

int HashTableSet(HashTable& t, string key, string value, int& probes)
{
    unsigned long long f = FoldHash(key);
    int firstTomb = -1;
    probes = 0;
    string entry = key + SEP + value;
    for (int i = 0; i < t.capacity; i++)
    {
        int idx = ProbeIndex(t, f, i);
        probes++;
        string s = t.slots.data[idx];
        if (s == "")
        {
            int target = idx;
            if (firstTomb >= 0)
                target = firstTomb;
            t.slots.data[target] = entry;
            t.size++;
            return 0;
        }
        if (s == TOMBSTONE)
        {
            if (firstTomb < 0)
                firstTomb = idx;
            continue;
        }
        if (SlotKey(s) == key)
        {
            t.slots.data[idx] = entry;
            return 1;
        }
    }
    if (firstTomb >= 0)
    {
        t.slots.data[firstTomb] = entry;
        t.size++;
        return 0;
    }
    return -1;
}

bool HashTableGet(HashTable& t, string key, string& value, int& probes)
{
    unsigned long long f = FoldHash(key);
    probes = 0;
    for (int i = 0; i < t.capacity; i++)
    {
        int idx = ProbeIndex(t, f, i);
        probes++;
        string s = t.slots.data[idx];
        if (s == "")
            return false;
        if (s != TOMBSTONE && SlotKey(s) == key)
        {
            value = SlotValue(s);
            return true;
        }
    }
    return false;
}

bool HashTableDel(HashTable& t, string key, string& removed, int& probes)
{
    unsigned long long f = FoldHash(key);
    probes = 0;
    for (int i = 0; i < t.capacity; i++)
    {
        int idx = ProbeIndex(t, f, i);
        probes++;
        string s = t.slots.data[idx];
        if (s == "")
            return false;
        if (s != TOMBSTONE && SlotKey(s) == key)
        {
            removed = SlotValue(s);
            t.slots.data[idx] = TOMBSTONE;
            t.size--;
            return true;
        }
    }
    return false;
}

void HashTableToArray(HashTable& t, Array& items)
{
    ArrayPush(items, t.mode == PROBE_LINEAR ? "linear" : "double");
    ArrayPush(items, to_string(t.capacity));
    for (int i = 0; i < t.capacity; i++)
    {
        string s = t.slots.data[i];
        if (s == "")
            continue;
        ArrayPush(items, to_string(i));
        if (s == TOMBSTONE)
        {
            ArrayPush(items, "~");
        }
        else
        {
            ArrayPush(items, SlotKey(s));
            ArrayPush(items, SlotValue(s));
        }
    }
}

bool HashTableFromArray(Array& items, HashTable& t, string& error)
{
    long long cap;
    if (items.size < 2 || !ParseLL(items.data[1], cap))
    {
        error = "hash table in the file is damaged";
        return false;
    }
    ProbeMode mode = PROBE_LINEAR;
    if (items.data[0] == "double")
        mode = PROBE_DOUBLE;
    if (!HashTableInit(t, mode, (int)cap, error))
        return false;
    int p = 2;
    while (p < items.size)
    {
        long long idx;
        if (!ParseLL(items.data[p], idx) || idx < 0 || idx >= t.capacity || p + 1 >= items.size)
        {
            error = "hash table in the file is damaged";
            HashTableFree(t);
            return false;
        }
        if (items.data[p + 1] == "~")
        {
            t.slots.data[idx] = TOMBSTONE;
            p += 2;
        }
        else
        {
            if (p + 2 >= items.size)
            {
                error = "hash table in the file is damaged";
                HashTableFree(t);
                return false;
            }
            t.slots.data[idx] = items.data[p + 1] + SEP + items.data[p + 2];
            t.size++;
            p += 3;
        }
    }
    return true;
}

#include <iostream>
#include "../headers/hash_commands.h"
#include "../headers/common.h"
#include "../headers/foldhash.h"
#include "../headers/hashtable.h"
#include "../headers/utils.h"

// Подзадание 6.1: команды хеш-таблицы HCREATE, HSET, HGET, HDEL, HSTAT, HHASH, PRINT

static void ShowHashTable(HashTable& t)
{
    bool printed = false;
    for (int i = 0; i < t.capacity; i++)
    {
        string s = t.slots.data[i];
        if (s == "")
            continue;
        printed = true;
        if (s == TOMBSTONE)
            cout << "-> [" << i << "] <deleted>" << endl;
        else
            cout << "-> [" << i << "] " << SlotKey(s) << " -> " << SlotValue(s) << endl;
    }
    if (!printed)
        cout << "-> (empty)" << endl;
}

void RunHash(Array& w, Array& items, bool isNew, bool& changed)
{
    string cmd = w.data[0];
    HashTable t;
    string error;
    if (cmd == "HCREATE")
    {
        if (w.size < 3 || w.size > 4)
        {
            Error("wrong number of arguments");
            return;
        }
        if (!isNew)
        {
            Error("structure already exists");
            return;
        }
        string mode = w.data[2];
        long long cap = 11;
        if (mode != "linear" && mode != "double")
        {
            Error("mode must be linear or double");
            return;
        }
        if (w.size == 4 && !ParseLL(w.data[3], cap))
        {
            Error("capacity must be an integer");
            return;
        }
        ProbeMode pm = PROBE_LINEAR;
        if (mode == "double")
            pm = PROBE_DOUBLE;
        if (!HashTableInit(t, pm, (int)cap, error))
        {
            Error(error);
            return;
        }
        Array fresh;
        ArrayInit(fresh);
        HashTableToArray(t, fresh);
        Replace(items, fresh);
        HashTableFree(t);
        changed = true;
        cout << "-> OK" << endl;
        return;
    }

    if (!HashTableFromArray(items, t, error))
    {
        Error(error);
        return;
    }
    int probes = 0;
    string value;
    if (cmd == "HSET")
    {
        if (CheckArgs(w, 4))
        {
            if (w.data[2] == "~")
                Error("key '~' is reserved");
            else if (HashTableSet(t, w.data[2], w.data[3], probes) < 0)
                Error("table is full");
            else
            {
                changed = true;
                cout << "-> OK (probes: " << probes << ")" << endl;
            }
        }
    }
    else if (cmd == "HGET")
    {
        if (CheckArgs(w, 3))
        {
            if (HashTableGet(t, w.data[2], value, probes))
                cout << "-> " << value << endl;
            else
                Error("key not found");
        }
    }
    else if (cmd == "HDEL")
    {
        if (CheckArgs(w, 3))
        {
            if (HashTableDel(t, w.data[2], value, probes))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("key not found");
        }
    }
    else if (cmd == "HSTAT")
    {
        if (CheckArgs(w, 2))
        {
            string mode = "double";
            if (t.mode == PROBE_LINEAR)
                mode = "linear";
            cout << "-> mode: " << mode << ", capacity: " << t.capacity << ", size: " << t.size
                 << ", load factor: " << Fixed2((double)t.size / t.capacity) << endl;
        }
    }
    else if (cmd == "HHASH")
    {
        if (CheckArgs(w, 3))
        {
            string key = w.data[2];
            unsigned long long f = FoldHash(key);
            string line = "fold: " + FoldTrace(key) + "; h1 = " + to_string(f) + " mod " + to_string(t.capacity) +
                          " = " + to_string(ProbeIndex(t, f, 0));
            if (t.mode == PROBE_DOUBLE)
                line = line + "; h2 = 1 + " + to_string(f) + " mod " + to_string(t.capacity - 1) + " = " +
                       to_string(ProbeStep(t, f));
            line = line + "; probe sequence:";
            for (int i = 0; i < 5 && i < t.capacity; i++)
                line = line + " " + to_string(ProbeIndex(t, f, i));
            cout << "-> " << line << endl;
        }
    }
    else if (cmd == "PRINT")
    {
        if (CheckArgs(w, 2))
            ShowHashTable(t);
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        HashTableToArray(t, fresh);
        Replace(items, fresh);
    }
    HashTableFree(t);
}

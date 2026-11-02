#include <cmath>
#include <cstdio>
#include <iostream>
#include "../headers/hashbench.h"
#include "../headers/common.h"
#include "../headers/hashtable.h"
#include "../headers/utils.h"

static string NextKey(unsigned long long& state)
{
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return to_string(100000000ULL + (state >> 33) % 900000000ULL);
}

void RunBench(Array& w)
{
    long long cap = 1009;
    if (w.size > 2)
    {
        Error("wrong number of arguments");
        return;
    }
    if (w.size == 2 && (!ParseLL(w.data[1], cap) || cap < 11 || cap > 1000000 || !IsPrime((int)cap)))
    {
        Error("capacity must be a prime number in range 11..1000000");
        return;
    }
    cout << "-> mode    load     n  insert     hit    miss  | theory hit   miss" << endl;
    double loads[3] = {0.5, 0.7, 0.9};
    for (int m = 0; m < 2; m++)
    {
        for (int li = 0; li < 3; li++)
        {
            HashTable t;
            string error;
            HashTableInit(t, m == 0 ? PROBE_LINEAR : PROBE_DOUBLE, (int)cap, error);
            unsigned long long state = 12345ULL;
            int n = (int)(loads[li] * (double)cap);
            Array keys;
            ArrayGuard guard(keys);
            long long insertProbes = 0;
            int probes = 0;
            while (keys.size < n)
            {
                string key = NextKey(state);
                if (HashTableSet(t, key, "v", probes) == 0)
                {
                    insertProbes += probes;
                    ArrayPush(keys, key);
                }
            }
            long long hitProbes = 0;
            string value;
            for (int i = 0; i < keys.size; i++)
            {
                HashTableGet(t, keys.data[i], value, probes);
                hitProbes += probes;
            }
            long long missProbes = 0;
            int missCount = 0;
            for (int i = 0; i < 2000; i++)
            {
                string key = NextKey(state);
                if (!HashTableGet(t, key, value, probes))
                {
                    missProbes += probes;
                    missCount++;
                }
            }
            double a = (double)n / (double)cap;
            double thHit;
            double thMiss;
            if (m == 0)
            {
                thHit = 0.5 * (1.0 + 1.0 / (1.0 - a));
                thMiss = 0.5 * (1.0 + 1.0 / ((1.0 - a) * (1.0 - a)));
            }
            else
            {
                thHit = (1.0 / a) * log(1.0 / (1.0 - a));
                thMiss = 1.0 / (1.0 - a);
            }
            char buf[200];
            snprintf(buf, sizeof(buf), "%-6s  %.2f  %4d  %6.2f  %6.2f  %6.2f  |      %6.2f %6.2f",
                     m == 0 ? "linear" : "double", loads[li], n,
                     (double)insertProbes / n, (double)hitProbes / n, (double)missProbes / missCount, thHit, thMiss);
            cout << "-> " << buf << endl;
            HashTableFree(t);
        }
    }
}

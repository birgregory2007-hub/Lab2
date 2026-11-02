#include <iostream>
#include <vector>
#include "../headers/subsets.h"
#include "../headers/common.h"
#include "../headers/set.h"
#include "../headers/utils.h"

bool AllDistinctSubsets(Array& items, Array& out, string& error)
{
    int n = items.size;
    if (n > 20)
    {
        error = "array is too large (max 20 elements)";
        return false;
    }
    Set seen;
    SetInit(seen);
    for (int k = 0; k <= n; k++)
    {
        vector<int> idx(k);
        for (int j = 0; j < k; j++)
            idx[j] = j;
        while (true)
        {
            string key = "";
            string text = "{";
            for (int j = 0; j < k; j++)
            {
                key = key + items.data[idx[j]] + "\x1f";
                if (j > 0)
                    text = text + ", ";
                text = text + items.data[idx[j]];
            }
            if (SetAdd(seen, key))
                ArrayPush(out, text + "}");
            int p = k - 1;
            while (p >= 0 && idx[p] == n - k + p)
                p--;
            if (p < 0)
                break;
            idx[p]++;
            for (int j = p + 1; j < k; j++)
                idx[j] = idx[j - 1] + 1;
        }
    }
    SetFree(seen);
    return true;
}

void ShowSubsets(Array& items)
{
    Array res;
    ArrayGuard guard(res);
    string error;
    if (AllDistinctSubsets(items, res, error))
        cout << "-> [" << JoinArray(res, ", ") << "]" << endl;
    else
        Error(error);
}

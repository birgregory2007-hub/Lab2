#include <algorithm>
#include <iostream>
#include <vector>
#include "../headers/setsplit.h"
#include "../headers/common.h"
#include "../headers/utils.h"

static bool Fill(vector<long long>& a, int i, long long target, vector<long long>& sums, vector<int>& assign)
{
    if (i == (int)a.size())
        return true;
    for (int b = 0; b < (int)sums.size(); b++)
    {
        if (sums[b] + a[i] > target)
            continue;
        bool seen = false;
        for (int p = 0; p < b; p++)
        {
            if (sums[p] == sums[b])
            {
                seen = true;
                break;
            }
        }
        if (seen)
            continue;
        sums[b] += a[i];
        assign[i] = b;
        if (Fill(a, i + 1, target, sums, assign))
            return true;
        sums[b] -= a[i];
    }
    return false;
}

static bool LoadNumbers(Set& s, vector<long long>& nums, string& error)
{
    Array sorted;
    ArrayGuard guard(sorted);
    SetToArray(s, sorted);
    for (int i = 0; i < sorted.size; i++)
    {
        long long v;
        if (!ParseLL(sorted.data[i], v) || v <= 0)
        {
            error = "set must contain only natural numbers";
            return false;
        }
        nums.push_back(v);
    }
    if (nums.size() == 0)
    {
        error = "set is empty";
        return false;
    }
    sort(nums.begin(), nums.end(), greater<long long>());
    return true;
}

static bool SplitSorted(vector<long long>& desc, long long target, Array& groups)
{
    long long total = 0;
    for (int i = 0; i < (int)desc.size(); i++)
        total += desc[i];
    if (target <= 0 || total % target != 0 || desc[0] > target)
        return false;
    int k = (int)(total / target);
    vector<long long> sums(k, 0);
    vector<int> assign(desc.size(), 0);
    if (!Fill(desc, 0, target, sums, assign))
        return false;
    vector<vector<long long> > parts(k);
    for (int i = 0; i < (int)desc.size(); i++)
        parts[assign[i]].push_back(desc[i]);
    for (int b = 0; b < k; b++)
        sort(parts[b].begin(), parts[b].end());
    sort(parts.begin(), parts.end());
    for (int b = 0; b < k; b++)
    {
        string line = "{";
        for (int j = 0; j < (int)parts[b].size(); j++)
        {
            if (j > 0)
                line = line + ", ";
            line = line + to_string(parts[b][j]);
        }
        ArrayPush(groups, line + "}");
    }
    return true;
}

bool SplitEqualSum(Set& s, long long target, Array& groups, string& error)
{
    vector<long long> nums;
    if (!LoadNumbers(s, nums, error))
        return false;
    if (!SplitSorted(nums, target, groups))
    {
        error = "set cannot be partitioned into subsets with sum " + to_string(target);
        return false;
    }
    return true;
}

bool SplitAllSums(Set& s, Array& lines, string& error)
{
    vector<long long> nums;
    if (!LoadNumbers(s, nums, error))
        return false;
    long long total = 0;
    for (int i = 0; i < (int)nums.size(); i++)
        total += nums[i];
    vector<long long> divisors;
    for (long long d = 1; d * d <= total; d++)
    {
        if (total % d != 0)
            continue;
        divisors.push_back(d);
        if (d != total / d)
            divisors.push_back(total / d);
    }
    sort(divisors.begin(), divisors.end());
    for (int i = 0; i < (int)divisors.size(); i++)
    {
        if (divisors[i] < nums[0])
            continue;
        Array groups;
        ArrayGuard guard(groups);
        if (SplitSorted(nums, divisors[i], groups))
        {
            string word = " subsets): ";
            if (groups.size == 1)
                word = " subset): ";
            ArrayPush(lines, "sum " + to_string(divisors[i]) + " (" + to_string(groups.size) + word + JoinArray(groups, " "));
        }
    }
    return true;
}

void RunSetSplit(Array& w, Set& s)
{
    Array result;
    ArrayGuard guard(result);
    string error;
    long long target;
    if (w.size != 2 && w.size != 3)
        Error("wrong number of arguments");
    else if (w.size == 3)
    {
        if (!ParseLL(w.data[2], target) || target <= 0)
            Error("sum must be a natural number");
        else if (SplitEqualSum(s, target, result, error))
            cout << "-> " << JoinArray(result, " ") << endl;
        else
            Error(error);
    }
    else
    {
        if (!SplitAllSums(s, result, error))
            Error(error);
        else
        {
            for (int i = 0; i < result.size; i++)
                cout << "-> " << result.data[i] << endl;
        }
    }
}

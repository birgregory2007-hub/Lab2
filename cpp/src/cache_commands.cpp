#include <iostream>
#include "../headers/cache_commands.h"
#include "../headers/common.h"
#include "../headers/lrucache.h"
#include "../headers/utils.h"

static bool LoadCache(Array& items, LruCache& c, string& error)
{
    long long cap;
    if (items.size < 1 || !ParseLL(items.data[0], cap))
    {
        error = "cache in the file is damaged";
        return false;
    }
    if (!LruInit(c, (int)cap, error))
        return false;
    for (int i = 1; i + 1 < items.size; i += 2)
        LruSet(c, items.data[i], items.data[i + 1]);
    return true;
}

void RunCache(Array& w, Array& items, bool isNew, bool& changed)
{
    string cmd = w.data[0];
    LruCache c;
    string error;
    if (cmd == "CCREATE")
    {
        long long cap;
        if (!CheckArgs(w, 3))
            return;
        if (!isNew)
        {
            Error("structure already exists");
            return;
        }
        if (!ParseLL(w.data[2], cap))
        {
            Error("capacity must be an integer");
            return;
        }
        if (!LruInit(c, (int)cap, error))
        {
            Error(error);
            return;
        }
        LruFree(c);
        Array fresh;
        ArrayInit(fresh);
        ArrayPush(fresh, to_string(cap));
        Replace(items, fresh);
        changed = true;
        cout << "-> OK" << endl;
        return;
    }

    if (!LoadCache(items, c, error))
    {
        Error(error);
        return;
    }
    string value;
    if (cmd == "CSET")
    {
        if (CheckArgs(w, 4))
        {
            LruSet(c, w.data[2], w.data[3]);
            changed = true;
            cout << "-> OK" << endl;
        }
    }
    else if (cmd == "CGET")
    {
        if (CheckArgs(w, 3))
        {
            if (LruGet(c, w.data[2], value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                cout << "-> -1" << endl;
        }
    }
    else if (cmd == "PRINT")
    {
        if (CheckArgs(w, 2))
        {
            Array pairs;
            ArrayGuard guard(pairs);
            LruToArray(c, pairs);
            string line = "";
            for (int i = 0; i + 1 < pairs.size; i += 2)
            {
                if (i > 0)
                    line = line + ", ";
                line = line + pairs.data[i] + " -> " + pairs.data[i + 1];
            }
            if (line == "")
                line = "(empty)";
            cout << "-> " << line << endl;
        }
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        ArrayPush(fresh, items.data[0]);
        LruToArray(c, fresh);
        Replace(items, fresh);
    }
    LruFree(c);
}

void RunCacheBatch(Array& w)
{
    long long cap;
    string error;
    LruCache c;
    if (w.size < 3)
    {
        Error("wrong number of arguments");
        return;
    }
    if (!ParseLL(w.data[1], cap))
    {
        Error("capacity must be an integer");
        return;
    }
    if (!LruInit(c, (int)cap, error))
    {
        Error(error);
        return;
    }
    Array res;
    ArrayGuard guard(res);
    int i = 2;
    bool ok = true;
    while (i < w.size && ok)
    {
        string tok = ToUpper(w.data[i]);
        string value;
        if (tok == "SET" && i + 2 < w.size)
        {
            LruSet(c, w.data[i + 1], w.data[i + 2]);
            i += 3;
        }
        else if (tok == "GET" && i + 1 < w.size)
        {
            if (LruGet(c, w.data[i + 1], value))
                ArrayPush(res, value);
            else
                ArrayPush(res, "-1");
            i += 2;
        }
        else
        {
            Error("invalid query near '" + w.data[i] + "'");
            ok = false;
        }
    }
    if (ok)
        ShowJoined(res, " ");
    LruFree(c);
}

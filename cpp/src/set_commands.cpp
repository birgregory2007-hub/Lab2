#include <iostream>
#include "../headers/set_commands.h"
#include "../headers/common.h"
#include "../headers/set.h"
#include "../headers/setsplit.h"
#include "../headers/utils.h"

void RunSet(Array& w, Array& items, bool& changed)
{
    string cmd = w.data[0];
    Set s;
    SetInit(s);
    for (int i = 0; i < items.size; i++)
        SetAdd(s, items.data[i]);

    if (cmd == "SETADD")
    {
        if (CheckArgs(w, 3))
        {
            bool added = SetAdd(s, w.data[2]);
            changed = added;
            cout << "-> " << (added ? "TRUE" : "FALSE") << endl;
        }
    }
    else if (cmd == "SETDEL")
    {
        if (CheckArgs(w, 3))
        {
            bool removed = SetDel(s, w.data[2]);
            changed = removed;
            cout << "-> " << (removed ? "TRUE" : "FALSE") << endl;
        }
    }
    else if (cmd == "SET_AT")
    {
        if (CheckArgs(w, 3))
            cout << "-> " << (SetAt(s, w.data[2]) ? "TRUE" : "FALSE") << endl;
    }
    else if (cmd == "SETSIZE")
    {
        if (CheckArgs(w, 2))
            cout << "-> " << s.size << endl;
    }
    else if (cmd == "SETSPLIT")
    {
        RunSetSplit(w, s);
    }
    else if (cmd == "PRINT")
    {
        if (CheckArgs(w, 2))
        {
            Array sorted;
            ArrayGuard guard(sorted);
            SetToArray(s, sorted);
            ShowItems(sorted);
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
        SetToArray(s, fresh);
        Replace(items, fresh);
    }
    SetFree(s);
}

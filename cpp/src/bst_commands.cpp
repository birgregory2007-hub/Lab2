#include <iostream>
#include "../headers/bst_commands.h"
#include "../headers/common.h"
#include "../headers/tree.h"
#include "../headers/utils.h"

void RunBst(Array& w, Array& items, bool& changed)
{
    string cmd = w.data[0];
    Tree t;
    TreeInit(t);
    if (!TreeBuild(t, items, 0))
    {
        Error("tree in the file is damaged");
        return;
    }

    int value;
    if (cmd == "BINSERT")
    {
        if (CheckArgs(w, 3))
        {
            if (!ToInt(w.data[2], value))
                Error("value must be an integer");
            else if (!TreeInsertBST(t, value))
                Error("key already exists");
            else
            {
                changed = true;
                cout << "-> " << value << endl;
            }
        }
    }
    else if (cmd == "BBUILD")
    {
        if (w.size < 3)
            Error("wrong number of arguments");
        else
        {
            bool good = true;
            for (int i = 2; i < w.size; i++)
            {
                if (!ToInt(w.data[i], value))
                    good = false;
            }
            if (!good)
                Error("keys must be integers");
            else
            {
                TreeFree(t);
                for (int i = 2; i < w.size; i++)
                {
                    ToInt(w.data[i], value);
                    TreeInsertBST(t, value);
                }
                changed = true;
                cout << "-> OK" << endl;
            }
        }
    }
    else if (cmd == "BRIGHT")
    {
        if (CheckArgs(w, 2))
        {
            Array edge;
            ArrayGuard guard(edge);
            TreeRightEdge(t, edge);
            ShowJoined(edge, ", ");
        }
    }
    else if (cmd == "BINORDER")
    {
        if (CheckArgs(w, 2))
        {
            Array order;
            ArrayGuard guard(order);
            TreeInOrder(t, order);
            ShowItems(order);
        }
    }
    else if (cmd == "PRINT")
    {
        if (CheckArgs(w, 2))
            TreePrint(t);
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        TreeToArray(t, fresh);
        Replace(items, fresh);
    }
    TreeFree(t);
}

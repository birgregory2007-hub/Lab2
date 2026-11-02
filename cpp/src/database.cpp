#include <iostream>
#include <fstream>
#include "../headers/database.h"
#include "../headers/utils.h"
#include "../headers/array.h"
#include "../headers/flist.h"
#include "../headers/dlist.h"
#include "../headers/stack.h"
#include "../headers/queue.h"
#include "../headers/tree.h"
#include "../headers/common.h"
#include "../headers/standalone.h"
#include "../headers/set_commands.h"
#include "../headers/bst_commands.h"
#include "../headers/hash_commands.h"
#include "../headers/cache_commands.h"
#include "../headers/subsets.h"

void Error(string text)
{
    cout << "-> ERROR: " << text << endl;
}

bool CheckArgs(Array& w, int count)
{
    if (w.size != count)
    {
        Error("wrong number of arguments");
        return false;
    }
    return true;
}

static bool GetIndex(string s, int& index)
{
    if (!ToInt(s, index))
    {
        Error("index must be an integer");
        return false;
    }
    return true;
}

void ShowItems(Array& items)
{
    if (items.size == 0)
    {
        cout << "-> (empty)" << endl;
        return;
    }
    cout << "->";
    for (int i = 0; i < items.size; i++)
        cout << " " << items.data[i];
    cout << endl;
}

static void SplitWords(string s, Array& words)
{
    string word = "";
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (s[i] == ' ' || s[i] == '\t')
        {
            if (word != "")
                ArrayPush(words, word);
            word = "";
        }
        else
        {
            word = word + s[i];
        }
    }
    if (word != "")
        ArrayPush(words, word);
}

static void LoadFile(string fileName, Array& lines)
{
    ifstream in(fileName.c_str());
    if (!in.is_open())
        return;
    string line;
    while (getline(in, line))
    {
        if (line.size() > 0 && line[line.size() - 1] == '\r')
            line = line.substr(0, line.size() - 1);
        if (line != "")
            ArrayPush(lines, line);
    }
}

static void SaveFile(string fileName, Array& lines)
{
    ofstream out(fileName.c_str());
    for (int i = 0; i < lines.size; i++)
        out << lines.data[i] << endl;
}

static int FindLine(Array& lines, string name)
{
    for (int i = 0; i < lines.size; i++)
    {
        Array w;
        ArrayInit(w);
        SplitWords(lines.data[i], w);
        bool found = (w.size >= 2 && w.data[1] == name);
        ArrayFree(w);
        if (found)
            return i;
    }
    return -1;
}

static void ParseLine(string line, string& type, string& name, Array& items)
{
    Array w;
    ArrayInit(w);
    SplitWords(line, w);
    type = w.data[0];
    name = w.data[1];
    for (int i = 2; i < w.size; i++)
        ArrayPush(items, w.data[i]);
    ArrayFree(w);
}

static string BuildLine(string type, string name, Array& items)
{
    string line = type + " " + name;
    for (int i = 0; i < items.size; i++)
        line = line + " " + items.data[i];
    return line;
}

void Replace(Array& items, Array& fresh)
{
    ArrayFree(items);
    items = fresh;
}

static void RunArray(Array& w, Array& items, bool isNew, bool& changed)
{
    string cmd = w.data[0];
    string value;
    int index;
    if (cmd == "MCREATE")
    {
        if (!CheckArgs(w, 2))
            return;
        if (!isNew)
        {
            Error("structure already exists");
            return;
        }
        changed = true;
        cout << "-> OK" << endl;
    }
    else if (cmd == "MPUSH")
    {
        if (!CheckArgs(w, 3))
            return;
        ArrayPush(items, w.data[2]);
        changed = true;
        cout << "-> " << w.data[2] << endl;
    }
    else if (cmd == "MINSERT")
    {
        if (!CheckArgs(w, 4) || !GetIndex(w.data[2], index))
            return;
        if (!ArrayInsert(items, index, w.data[3]))
        {
            Error("index out of range");
            return;
        }
        changed = true;
        cout << "-> " << w.data[3] << endl;
    }
    else if (cmd == "MGET")
    {
        if (!CheckArgs(w, 3) || !GetIndex(w.data[2], index))
            return;
        if (!ArrayGet(items, index, value))
        {
            Error("index out of range");
            return;
        }
        cout << "-> " << value << endl;
    }
    else if (cmd == "MSET")
    {
        if (!CheckArgs(w, 4) || !GetIndex(w.data[2], index))
            return;
        if (!ArraySet(items, index, w.data[3]))
        {
            Error("index out of range");
            return;
        }
        changed = true;
        cout << "-> OK" << endl;
    }
    else if (cmd == "MDEL")
    {
        if (!CheckArgs(w, 3) || !GetIndex(w.data[2], index))
            return;
        if (!ArrayDelete(items, index, value))
        {
            Error("index out of range");
            return;
        }
        changed = true;
        cout << "-> " << value << endl;
    }
    else if (cmd == "MLEN")
    {
        if (!CheckArgs(w, 2))
            return;
        cout << "-> " << items.size << endl;
    }
    else if (cmd == "MSUBS")
    {
        if (!CheckArgs(w, 2))
            return;
        ShowSubsets(items);
    }
    else if (cmd == "PRINT")
    {
        if (!CheckArgs(w, 2))
            return;
        ShowItems(items);
    }
    else
    {
        Error("unknown command " + cmd);
    }
}

static void ShowFList(FList& l)
{
    cout << "-> ";
    if (l.size == 0)
        cout << "(empty)" << endl;
    else
        FListPrint(l);
}

static void RunFList(Array& w, Array& items, bool& changed)
{
    string cmd = w.data[0];
    string c;
    if (cmd == "PRINT")
        c = cmd;
    else
        c = cmd.substr(1);

    FList l;
    FListInit(l);
    for (int i = 0; i < items.size; i++)
        FListAddTail(l, items.data[i]);

    string value;
    int index;
    if (c == "PUSH" || c == "PUSHTAIL")
    {
        if (CheckArgs(w, 3))
        {
            FListAddTail(l, w.data[2]);
            changed = true;
            cout << "-> " << w.data[2] << endl;
        }
    }
    else if (c == "PUSHHEAD")
    {
        if (CheckArgs(w, 3))
        {
            FListAddHead(l, w.data[2]);
            changed = true;
            cout << "-> " << w.data[2] << endl;
        }
    }
    else if (c == "PUSHBEFORE")
    {
        if (CheckArgs(w, 4))
        {
            if (FListAddBefore(l, w.data[2], w.data[3]))
            {
                changed = true;
                cout << "-> " << w.data[3] << endl;
            }
            else
                Error("target not found");
        }
    }
    else if (c == "PUSHAFTER")
    {
        if (CheckArgs(w, 4))
        {
            if (FListAddAfter(l, w.data[2], w.data[3]))
            {
                changed = true;
                cout << "-> " << w.data[3] << endl;
            }
            else
                Error("target not found");
        }
    }
    else if (c == "DELHEAD")
    {
        if (CheckArgs(w, 2))
        {
            if (FListDelHead(l, value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("list is empty");
        }
    }
    else if (c == "DELTAIL")
    {
        if (CheckArgs(w, 2))
        {
            if (FListDelTail(l, value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("list is empty");
        }
    }
    else if (c == "DELBEFORE")
    {
        if (CheckArgs(w, 3))
        {
            if (FListDelBefore(l, w.data[2], value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("no element before target");
        }
    }
    else if (c == "DELAFTER")
    {
        if (CheckArgs(w, 3))
        {
            if (FListDelAfter(l, w.data[2], value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("no element after target");
        }
    }
    else if (c == "DELVAL")
    {
        if (CheckArgs(w, 3))
        {
            if (FListDelValue(l, w.data[2]))
            {
                changed = true;
                cout << "-> " << w.data[2] << endl;
            }
            else
                Error("value not found");
        }
    }
    else if (c == "GET")
    {
        if (CheckArgs(w, 3) && GetIndex(w.data[2], index))
        {
            if (FListGet(l, index, value))
                cout << "-> " << value << endl;
            else
                Error("index out of range");
        }
    }
    else if (c == "FIND")
    {
        if (CheckArgs(w, 3))
        {
            if (FListFind(l, w.data[2]))
                cout << "-> TRUE" << endl;
            else
                cout << "-> FALSE" << endl;
        }
    }
    else if (c == "REV")
    {
        if (CheckArgs(w, 2))
        {
            cout << "-> ";
            if (l.size == 0)
                cout << "(empty)" << endl;
            else
                FListPrintRev(l);
        }
    }
    else if (c == "LEN")
    {
        if (CheckArgs(w, 2))
            cout << "-> " << l.size << endl;
    }
    else if (c == "PRINT")
    {
        if (CheckArgs(w, 2))
            ShowFList(l);
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        FListToArray(l, fresh);
        Replace(items, fresh);
    }
    FListFree(l);
}

static void ShowDList(DList& l)
{
    cout << "-> ";
    if (l.size == 0)
        cout << "(empty)" << endl;
    else
        DListPrint(l);
}

static void RunDList(Array& w, Array& items, bool& changed)
{
    string cmd = w.data[0];
    string c;
    if (cmd == "PRINT")
        c = cmd;
    else
        c = cmd.substr(1);

    DList l;
    DListInit(l);
    for (int i = 0; i < items.size; i++)
        DListAddTail(l, items.data[i]);

    string value;
    int index;
    if (c == "PUSH" || c == "PUSHTAIL")
    {
        if (CheckArgs(w, 3))
        {
            DListAddTail(l, w.data[2]);
            changed = true;
            cout << "-> " << w.data[2] << endl;
        }
    }
    else if (c == "PUSHHEAD")
    {
        if (CheckArgs(w, 3))
        {
            DListAddHead(l, w.data[2]);
            changed = true;
            cout << "-> " << w.data[2] << endl;
        }
    }
    else if (c == "PUSHBEFORE")
    {
        if (CheckArgs(w, 4))
        {
            if (DListAddBefore(l, w.data[2], w.data[3]))
            {
                changed = true;
                cout << "-> " << w.data[3] << endl;
            }
            else
                Error("target not found");
        }
    }
    else if (c == "PUSHAFTER")
    {
        if (CheckArgs(w, 4))
        {
            if (DListAddAfter(l, w.data[2], w.data[3]))
            {
                changed = true;
                cout << "-> " << w.data[3] << endl;
            }
            else
                Error("target not found");
        }
    }
    else if (c == "DELHEAD")
    {
        if (CheckArgs(w, 2))
        {
            if (DListDelHead(l, value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("list is empty");
        }
    }
    else if (c == "DELTAIL")
    {
        if (CheckArgs(w, 2))
        {
            if (DListDelTail(l, value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("list is empty");
        }
    }
    else if (c == "DELBEFORE")
    {
        if (CheckArgs(w, 3))
        {
            if (DListDelBefore(l, w.data[2], value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("no element before target");
        }
    }
    else if (c == "DELAFTER")
    {
        if (CheckArgs(w, 3))
        {
            if (DListDelAfter(l, w.data[2], value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("no element after target");
        }
    }
    else if (c == "DELVAL")
    {
        if (CheckArgs(w, 3))
        {
            if (DListDelValue(l, w.data[2]))
            {
                changed = true;
                cout << "-> " << w.data[2] << endl;
            }
            else
                Error("value not found");
        }
    }
    else if (c == "GET")
    {
        if (CheckArgs(w, 3) && GetIndex(w.data[2], index))
        {
            if (DListGet(l, index, value))
                cout << "-> " << value << endl;
            else
                Error("index out of range");
        }
    }
    else if (c == "FIND")
    {
        if (CheckArgs(w, 3))
        {
            if (DListFind(l, w.data[2]))
                cout << "-> TRUE" << endl;
            else
                cout << "-> FALSE" << endl;
        }
    }
    else if (c == "REV")
    {
        if (CheckArgs(w, 2))
        {
            cout << "-> ";
            if (l.size == 0)
                cout << "(empty)" << endl;
            else
                DListPrintRev(l);
        }
    }
    else if (c == "LEN")
    {
        if (CheckArgs(w, 2))
            cout << "-> " << l.size << endl;
    }
    else if (c == "PRINT")
    {
        if (CheckArgs(w, 2))
            ShowDList(l);
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        DListToArray(l, fresh);
        Replace(items, fresh);
    }
    DListFree(l);
}

static void RunStack(Array& w, Array& items, bool& changed)
{
    string cmd = w.data[0];
    Stack s;
    StackInit(s);
    for (int i = items.size - 1; i >= 0; i--)
        StackPush(s, items.data[i]);

    string value;
    if (cmd == "SPUSH")
    {
        if (CheckArgs(w, 3))
        {
            StackPush(s, w.data[2]);
            changed = true;
            cout << "-> " << w.data[2] << endl;
        }
    }
    else if (cmd == "SPOP")
    {
        if (CheckArgs(w, 2))
        {
            if (StackPop(s, value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("stack is empty");
        }
    }
    else if (cmd == "SPEEK")
    {
        if (CheckArgs(w, 2))
        {
            if (StackPeek(s, value))
                cout << "-> " << value << endl;
            else
                Error("stack is empty");
        }
    }
    else if (cmd == "SLEN")
    {
        if (CheckArgs(w, 2))
            cout << "-> " << s.size << endl;
    }
    else if (cmd == "PRINT")
    {
        if (CheckArgs(w, 2))
            ShowItems(items);
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        StackToArray(s, fresh);
        Replace(items, fresh);
    }
    StackFree(s);
}

static void RunQueue(Array& w, Array& items, bool& changed)
{
    string cmd = w.data[0];
    Queue q;
    QueueInit(q);
    for (int i = 0; i < items.size; i++)
        QueuePush(q, items.data[i]);

    string value;
    if (cmd == "QPUSH")
    {
        if (CheckArgs(w, 3))
        {
            QueuePush(q, w.data[2]);
            changed = true;
            cout << "-> " << w.data[2] << endl;
        }
    }
    else if (cmd == "QPOP")
    {
        if (CheckArgs(w, 2))
        {
            if (QueuePop(q, value))
            {
                changed = true;
                cout << "-> " << value << endl;
            }
            else
                Error("queue is empty");
        }
    }
    else if (cmd == "QPEEK")
    {
        if (CheckArgs(w, 2))
        {
            if (QueuePeek(q, value))
                cout << "-> " << value << endl;
            else
                Error("queue is empty");
        }
    }
    else if (cmd == "QLEN")
    {
        if (CheckArgs(w, 2))
            cout << "-> " << q.size << endl;
    }
    else if (cmd == "PRINT")
    {
        if (CheckArgs(w, 2))
            ShowItems(items);
    }
    else
    {
        Error("unknown command " + cmd);
    }

    if (changed)
    {
        Array fresh;
        ArrayInit(fresh);
        QueueToArray(q, fresh);
        Replace(items, fresh);
    }
    QueueFree(q);
}

static void ShowTraversal(Tree& t, int kind)
{
    Array out;
    ArrayInit(out);
    if (kind == 0)
        TreeInOrder(t, out);
    else if (kind == 1)
        TreePreOrder(t, out);
    else if (kind == 2)
        TreePostOrder(t, out);
    else
        TreeLevelOrder(t, out);
    ShowItems(out);
    ArrayFree(out);
}

static void RunTree(Array& w, Array& items, bool& changed)
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
    int index;
    if (cmd == "TINSERT" || cmd == "TPUSH")
    {
        if (CheckArgs(w, 3))
        {
            if (!ToInt(w.data[2], value))
                Error("value must be an integer");
            else
            {
                TreeInsert(t, value);
                changed = true;
                cout << "-> " << value << endl;
            }
        }
    }
    else if (cmd == "TBUILD")
    {
        if (w.size < 3)
            Error("wrong number of arguments");
        else if (TreeBuild(t, w, 2))
        {
            changed = true;
            cout << "-> OK" << endl;
        }
        else
            Error("wrong tree description");
    }
    else if (cmd == "TGET")
    {
        if (CheckArgs(w, 3) && GetIndex(w.data[2], index))
        {
            Array level;
            ArrayInit(level);
            TreeLevelOrder(t, level);
            string result;
            if (ArrayGet(level, index, result))
                cout << "-> " << result << endl;
            else
                Error("index out of range");
            ArrayFree(level);
        }
    }
    else if (cmd == "TFIND")
    {
        if (CheckArgs(w, 3))
        {
            if (!ToInt(w.data[2], value))
                Error("value must be an integer");
            else
            {
                int level;
                if (TreeFind(t, value, level))
                    cout << "-> level " << level << endl;
                else
                    Error("value not found");
            }
        }
    }
    else if (cmd == "ISMEMBER")
    {
        if (CheckArgs(w, 3))
        {
            int level;
            if (!ToInt(w.data[2], value))
                Error("value must be an integer");
            else if (TreeFind(t, value, level))
                cout << "-> TRUE" << endl;
            else
                cout << "-> FALSE" << endl;
        }
    }
    else if (cmd == "TISCOMPLETE")
    {
        if (CheckArgs(w, 2))
        {
            if (TreeIsComplete(t))
                cout << "-> TRUE" << endl;
            else
                cout << "-> FALSE" << endl;
        }
    }
    else if (cmd == "TINORDER" || cmd == "TPREORDER" || cmd == "TPOSTORDER" || cmd == "TBFS")
    {
        if (CheckArgs(w, 2))
        {
            if (cmd == "TINORDER")
                ShowTraversal(t, 0);
            else if (cmd == "TPREORDER")
                ShowTraversal(t, 1);
            else if (cmd == "TPOSTORDER")
                ShowTraversal(t, 2);
            else
                ShowTraversal(t, 3);
        }
    }
    else if (cmd == "TSIZE")
    {
        if (CheckArgs(w, 2))
            cout << "-> " << t.size << endl;
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

static bool IsCreating(string cmd)
{
    return cmd == "MCREATE" || cmd == "MPUSH" || cmd == "MINSERT" ||
           cmd == "FPUSH" || cmd == "FPUSHHEAD" || cmd == "FPUSHTAIL" ||
           cmd == "LPUSH" || cmd == "LPUSHHEAD" || cmd == "LPUSHTAIL" ||
           cmd == "SPUSH" || cmd == "QPUSH" || cmd == "TINSERT" || cmd == "TPUSH" || cmd == "TBUILD" ||
           cmd == "SETADD" || cmd == "BINSERT" || cmd == "BBUILD" || cmd == "HCREATE" || cmd == "CCREATE";
}

void ExecuteQuery(string fileName, string query)
{
    Array words;
    ArrayInit(words);
    SplitWords(query, words);
    if (words.size == 0)
    {
        Error("empty query");
        ArrayFree(words);
        return;
    }
    words.data[0] = ToUpper(words.data[0]);
    if (RunStandalone(words, query))    // EVAL, CRUN, HFOLD, HBENCH работают без файла
    {
        ArrayFree(words);
        return;
    }
    if (words.size == 1)
    {
        Error("structure name is missing");
        ArrayFree(words);
        return;
    }
    string cmd = words.data[0];
    string name = words.data[1];

    Array lines;
    ArrayInit(lines);
    LoadFile(fileName, lines);
    int lineNumber = FindLine(lines, name);

    string lineType = "";
    string lineName;
    Array items;
    ArrayInit(items);
    if (lineNumber >= 0)
        ParseLine(lines.data[lineNumber], lineType, lineName, items);

    char type;
    if (cmd == "PRINT")
        type = (lineNumber >= 0) ? lineType[0] : '?';
    else if (cmd == "ISMEMBER")
        type = 'T';
    else if (cmd.compare(0, 3, "SET") == 0)
        type = 'E';    // множество (команды SETADD, SETDEL, SET_AT), буква S занята стеком
    else
        type = cmd[0];

    bool isNew = false;
    bool changed = false;
    bool good = true;
    if (type != 'M' && type != 'F' && type != 'L' && type != 'S' && type != 'Q' && type != 'T' &&
        type != 'E' && type != 'B' && type != 'H' && type != 'C' && cmd != "PRINT")
    {
        Error("unknown command " + cmd);
        good = false;
    }
    else if (lineNumber >= 0 && lineType[0] != type)
    {
        Error("structure " + name + " has another type");
        good = false;
    }
    else if (lineNumber < 0)
    {
        if (IsCreating(cmd))
            isNew = true;
        else
        {
            Error("structure " + name + " not found");
            good = false;
        }
    }

    if (good)
    {
        if (type == 'M')
            RunArray(words, items, isNew, changed);
        else if (type == 'F')
            RunFList(words, items, changed);
        else if (type == 'L')
            RunDList(words, items, changed);
        else if (type == 'S')
            RunStack(words, items, changed);
        else if (type == 'Q')
            RunQueue(words, items, changed);
        else if (type == 'T')
            RunTree(words, items, changed);
        else if (type == 'E')
            RunSet(words, items, changed);
        else if (type == 'B')
            RunBst(words, items, changed);
        else if (type == 'H')
            RunHash(words, items, isNew, changed);
        else
            RunCache(words, items, isNew, changed);
    }

    if (good && changed)
    {
        string line = BuildLine(string(1, type), name, items);
        if (lineNumber >= 0)
            lines.data[lineNumber] = line;
        else
            ArrayPush(lines, line);
        SaveFile(fileName, lines);
    }

    ArrayFree(items);
    ArrayFree(lines);
    ArrayFree(words);
}

#include "../headers/standalone.h"
#include "../headers/expression.h"
#include "../headers/foldhash.h"
#include "../headers/hashbench.h"
#include "../headers/cache_commands.h"

bool RunStandalone(Array& w, string query)
{
    string cmd = w.data[0];
    if (cmd == "EVAL")
    {
        int p = (int)query.find_first_not_of(" \t");
        p = (int)query.find_first_of(" \t", p);
        if (p < 0)
            RunEval("");
        else
            RunEval(query.substr(p));
        return true;
    }
    if (cmd == "CRUN")
    {
        RunCacheBatch(w);
        return true;
    }
    if (cmd == "HFOLD")
    {
        RunFold(w);
        return true;
    }
    if (cmd == "HBENCH")
    {
        RunBench(w);
        return true;
    }
    return false;
}

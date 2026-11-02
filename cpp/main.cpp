#include <iostream>
#include <string>
#include "headers/database.h"
using namespace std;

int main(int argc, char* argv[])
{
    string fileName = "";
    string query = "";
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc)
        {
            fileName = argv[i + 1];
            i++;
        }
        else if (arg == "--query" && i + 1 < argc)
        {
            query = argv[i + 1];
            i++;
        }
    }
    if (fileName == "" || query == "")
    {
        cout << "usage: ./dbms --file file.data --query 'SPUSH mystack item'" << endl;
        return 1;
    }
    ExecuteQuery(fileName, query);
    return 0;
}

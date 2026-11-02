#ifndef DATABASE_H
#define DATABASE_H

#include <string>
using namespace std;

// выполняет один запрос к файлу с данными
void ExecuteQuery(string fileName, string query);

#endif

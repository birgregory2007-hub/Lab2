#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include "array.h"
using namespace std;

// Подзадание 6.1 (вариант 1): хеш-таблица с открытой адресацией, линейное апробирование и двойное хеширование. Слоты хранятся в массиве Array (лаб. 1):
//   ""            - пустой слот
//   "\x02"        - удалённый слот (надгробие)
//   key\x1fvalue  - занятый слот
enum ProbeMode
{
    PROBE_LINEAR = 0,
    PROBE_DOUBLE = 1
};

struct HashTable
{
    Array slots;
    int capacity;
    int size;
    ProbeMode mode;
};

extern const char* TOMBSTONE;

bool IsPrime(int n);

bool HashTableInit(HashTable& t, ProbeMode mode, int capacity, string& error);
void HashTableFree(HashTable& t);
int HashTableSet(HashTable& t, string key, string value, int& probes);   // 0 - добавлен, 1 - обновлён, -1 - нет места
bool HashTableGet(HashTable& t, string key, string& value, int& probes);
bool HashTableDel(HashTable& t, string key, string& removed, int& probes);

int ProbeIndex(HashTable& t, unsigned long long fold, int attempt);
int ProbeStep(HashTable& t, unsigned long long fold);

string SlotKey(string slot);
string SlotValue(string slot);

// сохранение и загрузка точного состояния таблицы (включая надгробия)
void HashTableToArray(HashTable& t, Array& items);
bool HashTableFromArray(Array& items, HashTable& t, string& error);

#endif

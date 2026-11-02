#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <string>
#include "array.h"
#include "dlist.h"
using namespace std;

// LRU-кэш: двусвязный список DList (лаб. 1) хранит порядок использования
// (голова - давно не использовался, хвост - использовался последним),
// хеш-таблица с цепочками даёт доступ к записи по ключу за O(1)
struct CacheEntry
{
    string key;
    string value;
    DNode* node;         // узел списка использования
    CacheEntry* next;    // следующая запись в цепочке корзины
};

struct LruCache
{
    int capacity;
    int size;
    DList order;
    CacheEntry** buckets;
    int bucketCount;
};

bool LruInit(LruCache& c, int capacity, string& error);
void LruFree(LruCache& c);
void LruSet(LruCache& c, string key, string value);
bool LruGet(LruCache& c, string key, string& value);   // false - ключа нет
void LruToArray(LruCache& c, Array& out);              // пары "ключ значение" от давних к свежим

#endif

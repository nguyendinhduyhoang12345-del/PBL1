#ifndef HASH_SERVICE_H
#define HASH_SERVICE_H

#include "../models/models.h"

HashTable *createHashTable();
void freeHashTable(HashTable *ht);
void hashTableInsert(HashTable *ht, int key, MenuItem item);
MenuItem *hashTableSearch(HashTable *ht, int key);

#endif
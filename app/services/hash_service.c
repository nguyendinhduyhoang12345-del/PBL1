#include <stdlib.h>
#include "hash_service.h"

static int hash(int key, int size) {
    return key % size;
}

HashTable *createHashTable() {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size = HASH_TABLE_SIZE;
    ht->table = (HashNode **)calloc(ht->size, sizeof(HashNode*));
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    return ht;
}

void freeHashTable(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

void hashTableInsert(HashTable *ht, int key, MenuItem item) {
    if (!ht) return;
    int index = hash(key, ht->size);
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (!node) return;
    node->key = key;
    node->item = item;
    node->next = ht->table[index];
    ht->table[index] = node;
}

MenuItem *hashTableSearch(HashTable *ht, int key) {
    if (!ht) return NULL;
    int index = hash(key, ht->size);
    HashNode *node = (HashNode *)ht->table[index];
    while (node) {
        if (node->key == key) {
            return &node->item;
        }
        node = node->next;
    }
    return NULL;
}
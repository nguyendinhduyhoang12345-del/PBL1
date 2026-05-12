#ifndef MENU_HASH_SERVICE_H
#define MENU_HASH_SERVICE_H

#include "../models/models.h"

// Hàm load menu từ file vào HashTable
HashTable* loadMenuToHashTable(const char* filename);

// Hàm lấy MenuItem từ HashTable theo id
MenuItem* getMenuItemFromHash(HashTable* ht, int id);

#endif
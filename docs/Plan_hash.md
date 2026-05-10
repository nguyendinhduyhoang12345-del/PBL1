# Plan_hash.md

## Mục đích
File này hướng dẫn cách code từng hàm của Hash Table cho OrderItem trong dự án.

## 1. Thiết kế struct
File: `app/services/hash_table_service.h`

```c
#ifndef HASH_TABLE_SERVICE_H
#define HASH_TABLE_SERVICE_H

#include "../models/models.h"

#define HASH_TABLE_SIZE 31

typedef struct HashNode {
    int itemId;
    OrderItem item;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** table;
    int size;
    int count;
} HashTable;

HashTable* hashTableCreate(int size);
int hashFunction(int key, int tableSize);
int hashTableInsert(HashTable* ht, OrderItem* item);
OrderItem* hashTableGet(HashTable* ht, int itemId);
int hashTableDelete(HashTable* ht, int itemId);
int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty);
OrderItem* hashTableToArray(HashTable* ht, int* count);
void hashTableFree(HashTable* ht);

#endif
```

## 2. Cách code từng hàm

### 2.1 `HashTable* hashTableCreate(int size)`
File: `app/services/hash_table_service.c`

- Mục đích: khởi tạo bảng hash động.
- Bước:
  1. Cấp phát `HashTable* ht = malloc(sizeof(HashTable));`
  2. Gán `ht->size = size; ht->count = 0;`
  3. Cấp phát `ht->table = malloc(size * sizeof(HashNode*));`
  4. Khởi tạo mỗi bucket = NULL bằng vòng lặp.
  5. Trả `ht`.

```c
HashTable* hashTableCreate(int size) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size = size;
    ht->count = 0;
    ht->table = (HashNode**)malloc(size * sizeof(HashNode*));
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}
```

### 2.2 `int hashFunction(int key, int tableSize)`
- Mục đích: tính bucket index.
- Bước:
  1. Lấy `int hash = key % tableSize;`
  2. Nếu hash âm, cộng tableSize.
  3. Trả hash.

```c
int hashFunction(int key, int tableSize) {
    int hash = key % tableSize;
    if (hash < 0) hash += tableSize;
    return hash;
}
```

### 2.3 `int hashTableInsert(HashTable* ht, OrderItem* item)`
- Mục đích: thêm item, nếu trùng ID thì cộng `quantity`.
- Bước:
  1. Tính bucket: `int idx = hashFunction(item->id, ht->size);`
  2. Duyệt chain ở bucket đó.
  3. Nếu tồn tại `node->item.id == item->id`:
     - `node->item.quantity += item->quantity;`
     - `node->item.totalPrice = node->item.quantity * node->item.price;`
     - Cập nhật `note`/`option` nếu cần.
     - Trả 0.
  4. Nếu không tồn tại node trùng:
     - Cấp phát `HashNode* newNode = malloc(sizeof(HashNode));`
     - Copy `newNode->item = *item;`
     - `newNode->itemId = item->id;`
     - Chèn vào đầu chain hoặc cuối chain.
     - `ht->count++`.
     - Trả 0.
  5. Nếu cấp phát thất bại, trả -1.

```c
int hashTableInsert(HashTable* ht, OrderItem* item) {
    if (!ht || !item) return -1;
    int idx = hashFunction(item->id, ht->size);
    HashNode* node = ht->table[idx];

    while (node) {
        if (node->itemId == item->id) {
            node->item.quantity += item->quantity;
            node->item.totalPrice = node->item.quantity * node->item.price;
            return 0;
        }
        node = node->next;
    }

    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    if (!newNode) return -1;
    newNode->itemId = item->id;
    newNode->item = *item;
    newNode->next = ht->table[idx];
    ht->table[idx] = newNode;
    ht->count++;
    return 0;
}
```

### 2.4 `OrderItem* hashTableGet(HashTable* ht, int itemId)`
- Mục đích: tìm item theo ID và trả pointer tới giá trị.
- Bước:
  1. Tính bucket.
  2. Duyệt chain.
  3. Nếu match, trả `&node->item`.
  4. Nếu không tìm, trả NULL.

```c
OrderItem* hashTableGet(HashTable* ht, int itemId) {
    if (!ht) return NULL;
    int idx = hashFunction(itemId, ht->size);
    HashNode* node = ht->table[idx];
    while (node) {
        if (node->itemId == itemId) return &node->item;
        node = node->next;
    }
    return NULL;
}
```

### 2.5 `int hashTableDelete(HashTable* ht, int itemId)`
- Mục đích: xóa item theo ID.
- Bước:
  1. Tính bucket.
  2. Duyệt chain với `prev` và `curr`.
  3. Nếu tìm thấy, unlink node khỏi chain.
  4. `free(curr)`, `ht->count--`, trả 0.
  5. Nếu không tìm, trả -1.

```c
int hashTableDelete(HashTable* ht, int itemId) {
    if (!ht) return -1;
    int idx = hashFunction(itemId, ht->size);
    HashNode* node = ht->table[idx];
    HashNode* prev = NULL;

    while (node) {
        if (node->itemId == itemId) {
            if (prev) prev->next = node->next;
            else ht->table[idx] = node->next;
            free(node);
            ht->count--;
            return 0;
        }
        prev = node;
        node = node->next;
    }
    return -1;
}
```

### 2.6 `int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty)`
- Mục đích: cập nhật số lượng & totalPrice.
- Bước:
  1. Lấy item qua `hashTableGet`.
  2. Nếu không tồn tại, trả -1.
  3. Nếu `newQty <= 0`, gọi `hashTableDelete`.
  4. Ngược lại, cập nhật `quantity` và `totalPrice`.
  5. Trả 0.

```c
int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty) {
    OrderItem* item = hashTableGet(ht, itemId);
    if (!item) return -1;
    if (newQty <= 0) {
        return hashTableDelete(ht, itemId);
    }
    item->quantity = newQty;
    item->totalPrice = item->price * newQty;
    return 0;
}
```

### 2.7 `OrderItem* hashTableToArray(HashTable* ht, int* count)`
- Mục đích: chuyển toàn bộ item thành array để hiển thị / lưu trữ.
- Bước:
  1. Nếu `ht->count == 0`, gán `*count = 0`, trả NULL hoặc malloc 0.
  2. Cấp phát `OrderItem* arr = malloc(ht->count * sizeof(OrderItem));`
  3. Duyệt tất cả buckets, copy mỗi `node->item` vào array.
  4. Gán `*count = ht->count`; trả `arr`.

```c
OrderItem* hashTableToArray(HashTable* ht, int* count) {
    if (!ht || !count) return NULL;
    *count = ht->count;
    if (ht->count == 0) return NULL;

    OrderItem* array = (OrderItem*)malloc(ht->count * sizeof(OrderItem));
    if (!array) {
        *count = 0;
        return NULL;
    }

    int idx = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode* node = ht->table[i];
        while (node) {
            array[idx++] = node->item;
            node = node->next;
        }
    }
    return array;
}
```

### 2.8 `void hashTableFree(HashTable* ht)`
- Mục đích: giải phóng toàn bộ memory bảng hash.
- Bước:
  1. Duyệt từng bucket.
  2. Với mỗi node trong bucket, `free(node)`.
  3. `free(ht->table)`.
  4. `free(ht)`.

```c
void hashTableFree(HashTable* ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode* node = ht->table[i];
        while (node) {
            HashNode* next = node->next;
            free(node);
            node = next;
        }
    }
    free(ht->table);
    free(ht);
}
```

## 3. Gợi ý khi tích hợp
- Khởi tạo `currentBill->itemsHash = hashTableCreate(HASH_TABLE_SIZE);`
- Khi xóa item trong cart, dùng `hashTableDelete`.
- Khi thay đổi số lượng, dùng `hashTableUpdateQuantity`.
- Khi hiển thị cart, dùng `hashTableToArray` và `free` array ngay sau khi dùng.
- Cập nhật `currentBill->itemCount = currentBill->itemsHash->count` mỗi lần thay đổi.

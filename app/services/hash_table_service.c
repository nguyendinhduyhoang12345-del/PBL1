#include "hash_table_service.h"
#include <stdio.h>
#include <stdlib.h>

HashTable* hashTableCreate(int size) {
    // Tạo một bảng băm mới với kích thước đã cho
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable)); 
    if (ht == NULL) return NULL;

    ht->size = size;
    ht->count = 0;
    ht->table = (HashNode**)malloc(size * sizeof(HashNode*));
    
    if (ht->table == NULL) {
        free(ht);
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        ht->table[i] = NULL; // Khởi tạo tất cả các bucket là NULL
    }
    return ht;
}

int hashFunction(int key, int tableSize) {
    int hash = key % tableSize;
    if (hash < 0) hash += tableSize;
    return hash;
}

int hashTableInsert(HashTable* ht, OrderItem* item) {
    if (!ht || !item) return -1;
    int idx = hashFunction(item->id, ht->size);
    HashNode* node = ht->table[idx]; // node là con trỏ đến đầu của bucket tại idx

    while (node) { // Kiểm tra nếu món đã tồn tại trong bucket
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
    newNode->next = ht->table[idx]; // Thêm node mới vào đầu bucket
    ht->table[idx] = newNode;
    ht->count++;
    return 0;
}

OrderItem* hashTableGet(HashTable *ht, int itemId) {
    if (!ht) return NULL;
    int idx = hashFunction(itemId, ht->size);
    HashNode* node = ht->table[idx];

    while (node) {
        if (node->itemId == itemId) {
            return &node->item; // Trả về con trỏ đến OrderItem nếu tìm thấy
        }
        node = node->next;
    }
    return NULL; // Không tìm thấy món
}

int hashTableDelete(HashTable* ht, int itemId) {
    if (!ht) return -1;
    int idx = hashFunction(itemId, ht->size);
    HashNode* node = ht->table[idx];
    HashNode* prev = NULL; // Con trỏ để theo dõi node trước đó trong bucket

    while (node) {
        if (node->itemId == itemId) {
            if (prev != NULL) prev->next = node->next; // Nếu node cần xóa không phải là node đầu của bucket
            else ht->table[idx] = node->next; // Nếu node cần xóa là node đầu của bucket
            free(node);
            ht->count--;
            return 0; // Xóa thành công
        }
        prev = node;
        node = node->next;
    }
    return -1; // Không tìm thấy món để xóa
}

int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty) {
    OrderItem* item = hashTableGet(ht, itemId);
    if (!item) return -1;
    if (newQty <= 0) {
        return hashTableDelete(ht, itemId); // Nếu số lượng mới <= 0, xóa món khỏi bảng băm
    }
    item->quantity = newQty;
    item->totalPrice = newQty * item->price; // Cập nhật lại tổng giá
    return 0;
}

OrderItem* hashTableToArray(HashTable* ht, int* count) {
    if (!ht || !count) return NULL;
    *count = ht->count;
    if (ht->count == 0) return NULL;

    // Cấp phát mảng để chứa tất cả OrderItem từ bảng băm
    OrderItem* array = (OrderItem*)malloc(ht->count * sizeof(OrderItem));
    if (!array) {
        *count = 0;
        return NULL;
    }

    int idx = 0;
    for (int i = 0; i < ht->size; i++) {
        HashNode* node = ht->table[i];
        while (node) {
            array[idx++] = node->item; // Sao chép OrderItem vào mảng
            node = node->next;
        }
    }
    return array;
}

void hashTableFree(HashTable* ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode* node = ht->table[i];
        while (node) {
            HashNode* temp = node;
            node = node->next;
            free(temp); // Giải phóng từng node trong bucket
        }
    }
    free(ht->table); // Giải phóng mảng bucket
    free(ht); // Giải phóng cấu trúc bảng băm
}
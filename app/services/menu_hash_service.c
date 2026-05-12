#include "menu_hash_service.h"
#include "hash_table_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hàm load menu từ file vào HashTable
HashTable* loadMenuToHashTable(const char* filename) {
    HashTable* ht = hashTableCreate(HASH_TABLE_SIZE);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("[!] Loi: Khong the mo file menu %s\n", filename);
        return ht;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        MenuItem item;
        if (sscanf(line, "%d|%[^|]|%lf|%d", &item.id, item.name, &item.price, &item.hasOptions) == 4) {
            // Giả sử stock ban đầu là 100 hoặc từ file khác, nhưng giờ hardcode
            item.stock = 100; // Có thể load từ file khác nếu cần
            // Insert vào hash table với key là item.id
            // Nhưng hashTableInsert nhận OrderItem, nên cần tạo OrderItem tạm
            OrderItem temp;
            temp.id = item.id;
            strcpy(temp.name, item.name);
            temp.price = item.price;
            temp.quantity = item.stock; // Dùng quantity làm stock tạm
            temp.option = item.hasOptions;
            strcpy(temp.note, "");
            temp.totalPrice = 0;
            hashTableInsert(ht, &temp);
        }
    }
    fclose(file);
    return ht;
}

// Hàm lấy MenuItem từ HashTable theo id
MenuItem* getMenuItemFromHash(HashTable* ht, int id) {
    OrderItem* orderItem = hashTableGet(ht, id);
    if (orderItem) {
        // Tạo MenuItem từ OrderItem
        static MenuItem item; // Static để trả về pointer
        item.id = orderItem->id;
        strcpy(item.name, orderItem->name);
        item.price = orderItem->price;
        item.stock = orderItem->quantity; // Stock lưu trong quantity
        item.hasOptions = orderItem->option;
        return &item;
    }
    return NULL;
}
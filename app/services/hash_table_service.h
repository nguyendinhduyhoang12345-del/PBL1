#ifndef HASH_TABLE_SERVICE_H
#define HASH_TABLE_SERVICE_H

#include "../models/models.h"

HashTable* hashTableCreate(int size); // Tạo bảng băm mới

int hashFunction(int key, int tableSize);

int hashTableInsert(HashTable* ht, OrderItem* item); // Thêm một món vào bảng băm

OrderItem* hashTableGet(HashTable *ht, int itemId); // Lấy thông tin món dựa trên ID

int hashTableDelete(HashTable* ht, int itemId); // Xóa một món khỏi bảng băm

int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty); // Cập nhật số lượng món trong bảng băm

OrderItem* hashTableToArray(HashTable* ht, int* count); // Chuyển bảng băm thành mảng để dễ dàng hiển thị

void hashTableFree(HashTable* ht); // Giải phóng bộ nhớ của bảng băm

#endif
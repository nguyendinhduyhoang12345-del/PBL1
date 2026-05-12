#ifndef DOUBLY_LINKED_LIST_SERVICE_H
#define DOUBLY_LINKED_LIST_SERVICE_H

#include "../models/models.h"


DoublyLinkedList* dllCreate();

// Thêm một OrderItem vào cuối danh sách liên kết đôi
int dllAppend(DoublyLinkedList* dll, OrderItem* item); 

// Lấy số lượng node trong danh sách
int dllGetCount(DoublyLinkedList* dll);

// Xóa một OrderItem tại vị trí cụ thể trong danh sách liên kết đôi
int dllRemoveAt(DoublyLinkedList* dll, int position);

// Cập nhật số lượng của một OrderItem tại vị trí cụ thể trong danh sách liên kết đôi
int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty);


DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position);


void dllUpdatePositions(DoublyLinkedList* dll);


void dllFree(DoublyLinkedList* dll);

#endif
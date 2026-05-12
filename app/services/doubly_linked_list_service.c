#include "doubly_linked_list_service.h"
#include <stdio.h>
#include <stdlib.h>

DoublyLinkedList* dllCreate() {
    DoublyLinkedList* dll = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (dll == NULL) return NULL;
    dll->head = NULL;
    dll->tail = NULL;
    dll->count = 0;
    return dll;
}

int dllAppend(DoublyLinkedList* dll, OrderItem* item) {
    if (!dll || !item) return -1;
    DLLNode* node = (DLLNode*)malloc(sizeof(DLLNode));
    if (!node) return -1;
    node->item = *item;
    node->next = NULL;
    node->prev = dll->tail;
    node->position = dll->count + 1;

    
    if (dll->tail) {  // nếu đuôi đã tồn tại, nối node mới vào cuối
        dll->tail->next = node;
    } 
    else { 
        dll->head = node;
    }
    dll->tail = node;
    dll->count++;
    return 0;
}

int dllGetCount(DoublyLinkedList* dll) {
    return dll ? dll->count : 0;
}

DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position) {
    if (!dll || position < 1 || position > dll->count) return NULL;
    DLLNode* node;
    if (position <= dll->count / 2) {
        node = dll->head;
        for (int i = 1; i < position; i++) {
            node = node->next;
        }
    } 
    else {
        node = dll->tail;
        for (int i = dll->count; i > position; i--) {
            node = node->prev;
        }
    }
    return node;
}

int dllRemoveAt(DoublyLinkedList* dll, int position) {
    if (!dll) return -1;
    DLLNode * node = dllGetNodeAt(dll, position);
    if (!node) return -1;

    if (node->prev) node->prev->next = node->next;
    else dll->head = node->next;

    if (node->next) node->next->prev = node->prev;
    else dll->tail = node->prev;

    free(node);
    dll->count--;
    dllUpdatePositions(dll);
    return 0;
}

int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty) {
    if (!dll) return -1;
    DLLNode* node = dllGetNodeAt(dll, position);
    if (!node) return -1;
    node->item.quantity = newQty;
    node->item.totalPrice = node->item.price * newQty;
    return 0;
}

void dllUpdatePositions(DoublyLinkedList* dll) {
    if (!dll) return;
    DLLNode* node = dll->head;
    int pos = 1;
    while (node) {
        node->position = pos++;
        node = node->next;
    }
}

void dllFree(DoublyLinkedList* dll) {
    if (!dll) return;
    DLLNode* node = dll->head;
    while (node) {
        DLLNode* next = node->next;
        free(node);
        node = next;
    }
    free(dll);
}
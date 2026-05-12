# Plan_DLL.md

## Mục đích
File này hướng dẫn cách code những hàm cần thiết nhất của Doubly Linked List cho cart editing: thêm món, xóa món, và chỉnh số lượng.

## 1. Thiết kế struct
File: `app/services/doubly_linked_list_service.h`

```c
#ifndef DOUBLY_LINKED_LIST_SERVICE_H
#define DOUBLY_LINKED_LIST_SERVICE_H

#include "../models/models.h"

typedef struct DLLNode {
    OrderItem item;
    int position;
    struct DLLNode* next;
    struct DLLNode* prev;
} DLLNode;

typedef struct {
    DLLNode* head;
    DLLNode* tail;
    int count;
} DoublyLinkedList;

DoublyLinkedList* dllCreate();
int dllAppend(DoublyLinkedList* dll, OrderItem* item);
int dllRemoveAt(DoublyLinkedList* dll, int position);
int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty);
DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position);
void dllUpdatePositions(DoublyLinkedList* dll);
void dllFree(DoublyLinkedList* dll);

#endif
```

## 2. Cách code từng hàm

### 2.1 `DoublyLinkedList* dllCreate()`
File: `app/services/doubly_linked_list_service.c`

- Mục đích: khởi tạo danh sách liên kết đôi rỗng.
- Cách làm:
  1. Cấp phát `DoublyLinkedList* dll = malloc(sizeof(DoublyLinkedList));`
  2. Gán `dll->head = NULL; dll->tail = NULL; dll->count = 0;`
  3. Trả `dll`.

```c
DoublyLinkedList* dllCreate() {
    DoublyLinkedList* dll = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (!dll) return NULL;
    dll->head = NULL;
    dll->tail = NULL;
    dll->count = 0;
    return dll;
}
```

### 2.2 `int dllAppend(DoublyLinkedList* dll, OrderItem* item)`
- Mục đích: thêm item mới vào cuối giỏ hàng.
- Cách làm:
  1. Cấp phát `DLLNode* node = malloc(sizeof(DLLNode));`
  2. Copy dữ liệu item vào `node->item`.
  3. Gán `node->next = NULL; node->prev = dll->tail;`
  4. Nếu danh sách rỗng thì `dll->head = node;`.
  5. Nếu không, `dll->tail->next = node;`.
  6. Cập nhật `dll->tail = node; dll->count++; node->position = dll->count;`

```c
int dllAppend(DoublyLinkedList* dll, OrderItem* item) {
    if (!dll || !item) return -1;
    DLLNode* node = (DLLNode*)malloc(sizeof(DLLNode));
    if (!node) return -1;
    node->item = *item;
    node->next = NULL;
    node->prev = dll->tail;
    node->position = dll->count + 1;

    if (dll->tail) {
        dll->tail->next = node;
    } else {
        dll->head = node;
    }
    dll->tail = node;
    dll->count++;
    return 0;
}
```

### 2.3 `DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position)`
- Mục đích: tìm node bằng STT trong giỏ.
- Cách làm:
  1. Nếu `position < 1 || position > dll->count`, trả NULL.
  2. Nếu position nằm nửa đầu, duyệt từ `head`.
  3. Nếu position nằm nửa sau, duyệt từ `tail`.
  4. Trả node.

```c
DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position) {
    if (!dll || position < 1 || position > dll->count) return NULL;
    DLLNode* node;
    if (position <= dll->count / 2) {
        node = dll->head;
        for (int i = 1; i < position; i++) {
            node = node->next;
        }
    } else {
        node = dll->tail;
        for (int i = dll->count; i > position; i--) {
            node = node->prev;
        }
    }
    return node;
}
```

### 2.4 `int dllRemoveAt(DoublyLinkedList* dll, int position)`
- Mục đích: xóa món khỏi giỏ theo STT.
- Cách làm:
  1. Tìm node bằng `dllGetNodeAt(dll, position)`.
  2. Nếu node không tồn tại, trả -1.
  3. Nếu `node->prev`, cập nhật `node->prev->next = node->next`.
  4. Nếu `node->next`, cập nhật `node->next->prev = node->prev`.
  5. Nếu node là đầu, cập nhật `dll->head = node->next`.
  6. Nếu node là cuối, cập nhật `dll->tail = node->prev`.
  7. Free node, giảm `dll->count`, gọi `dllUpdatePositions(dll)`.

```c
int dllRemoveAt(DoublyLinkedList* dll, int position) {
    if (!dll) return -1;
    DLLNode* node = dllGetNodeAt(dll, position);
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
```

### 2.5 `int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty)`
- Mục đích: cập nhật số lượng và giá trị tổng tiền của món trong giỏ.
- Cách làm:
  1. Tìm node bằng `dllGetNodeAt`.
  2. Nếu node không tồn tại, trả -1.
  3. Gán `node->item.quantity = newQty`.
  4. Gán `node->item.totalPrice = newQty * node->item.price`.

```c
int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty) {
    if (!dll) return -1;
    DLLNode* node = dllGetNodeAt(dll, position);
    if (!node) return -1;
    node->item.quantity = newQty;
    node->item.totalPrice = node->item.price * newQty;
    return 0;
}
```

### 2.6 `void dllUpdatePositions(DoublyLinkedList* dll)`
- Mục đích: đánh lại STT sau khi xóa món.
- Cách làm:
  1. Duyệt từ `dll->head`.
  2. Gán `node->position = 1, 2, ...`.

```c
void dllUpdatePositions(DoublyLinkedList* dll) {
    if (!dll) return;
    DLLNode* node = dll->head;
    int pos = 1;
    while (node) {
        node->position = pos++;
        node = node->next;
    }
}
```

### 2.7 `void dllFree(DoublyLinkedList* dll)`
- Mục đích: giải phóng bộ nhớ khi không còn dùng giỏ hàng.
- Cách làm:
  1. Duyệt từ đầu, free từng node.
  2. Free `dll`.

```c
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
```

## 3. Gợi ý khi tích hợp
- Khởi tạo `currentBill->cartDisplay = dllCreate();`
- Khi thêm món mới, gọi `dllAppend(currentBill->cartDisplay, &item);`
- Khi xóa món từ giỏ, gọi `dllRemoveAt(currentBill->cartDisplay, position);`
- Khi chỉnh số lượng, gọi `dllUpdateQuantity(currentBill->cartDisplay, position, newQty);`
- Sau mỗi thay đổi, đồng bộ `currentBill->itemCount = currentBill->cartDisplay->count;`

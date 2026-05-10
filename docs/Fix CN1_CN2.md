# Fix CN1 & CN2

## Mục tiêu chung

Hiện tại ý tưởng muốn tách:
1. Phần đặt món: dùng `Hash Table` để load menu và tìm nhanh theo `itemId`.
2. Phần giỏ hàng: dùng `Doubly Linked List` để quản lý `OrderItem`.
3. `addToCart()` chỉ dùng cho phần giỏ hàng, không chứa luôn logic tìm món.

---

## 1. Sửa `models` và cấu trúc dữ liệu

### 1.1. `app/models/models.h`

- Giữ nguyên `MenuItem`, `OrderItem`, `Customer`, `Bill`.
- Thêm định nghĩa `HashNode` và `HashTable` để lưu menu hash.
- Thêm định nghĩa `CartNode` và `Cart` cho Doubly Linked List.
- Sửa `Bill` để chứa `Cart cart;`.

Ví dụ:

```c
#define HASH_TABLE_SIZE 101

typedef struct HashNode {
    int key;
    MenuItem item;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode **table;
    int size;
} HashTable;

typedef struct CartNode {
    OrderItem item;
    struct CartNode *prev;
    struct CartNode *next;
} CartNode;

typedef struct {
    CartNode *head;
    CartNode *tail;
    int itemCount;
} Cart;

typedef struct {
    int id;
    Customer customer;
    Cart cart;
    double total;
    double discount;
    double finalPrice;
    char dateTime[50];
} Bill;
```

- Nếu dùng biến `menuHashTable` toàn cục, khai báo `extern HashTable *menuHashTable;`.

---

## 2. Tạo `hash_service` cho menu lookup

### 2.1. `app/services/hash_service.h`

Tạo interface:

```c
#ifndef HASH_SERVICE_H
#define HASH_SERVICE_H

#include "../models/models.h"

HashTable *createHashTable();
void freeHashTable(HashTable *ht);
void hashTableInsert(HashTable *ht, int key, MenuItem item);
MenuItem *hashTableSearch(HashTable *ht, int key);

#endif
```

### 2.2. `app/services/hash_service.c`

Triển khai hash table:

```c
#include <stdlib.h>
#include "hash_service.h"

static int hash(int key, int size) {
    return key % size;
}

HashTable *createHashTable() {
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size = HASH_TABLE_SIZE;
    ht->table = calloc(HASH_TABLE_SIZE, sizeof(HashNode *));
    return ht;
}

void freeHashTable(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            HashNode *next = node->next;
            free(node);
            node = next;
        }
    }
    free(ht->table);
    free(ht);
}

void hashTableInsert(HashTable *ht, int key, MenuItem item) {
    if (!ht) return;
    int idx = hash(key, ht->size);
    HashNode *node = malloc(sizeof(HashNode));
    node->key = key;
    node->item = item;
    node->next = ht->table[idx];
    ht->table[idx] = node;
}

MenuItem *hashTableSearch(HashTable *ht, int key) {
    if (!ht) return NULL;
    int idx = hash(key, ht->size);
    HashNode *node = ht->table[idx];
    while (node) {
        if (node->key == key) return &node->item;
        node = node->next;
    }
    return NULL;
}
```

---

## 3. Tạo `cart_service` cho giỏ hàng Doubly Linked List

### 3.1. `app/services/cart_service.h`

Tạo interface:

```c
#ifndef CART_SERVICE_H
#define CART_SERVICE_H

#include "../models/models.h"

void cartInit(Cart *cart);
void cartAddItem(Cart *cart, OrderItem item);
CartNode *cartGetNodeByIndex(Cart *cart, int index);
int cartRemoveItemByIndex(Cart *cart, int index);
int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity);

#endif
```

### 3.2. `app/services/cart_service.c`

Triển khai:

```c
#include <stdlib.h>
#include "cart_service.h"

void cartInit(Cart *cart) {
    cart->head = NULL;
    cart->tail = NULL;
    cart->itemCount = 0;
}

void cartAddItem(Cart *cart, OrderItem item) {
    CartNode *node = malloc(sizeof(CartNode));
    node->item = item;
    node->next = NULL;
    node->prev = cart->tail;
    if (cart->tail) cart->tail->next = node;
    cart->tail = node;
    if (!cart->head) cart->head = node;
    cart->itemCount++;
}

CartNode *cartGetNodeByIndex(Cart *cart, int index) {
    if (!cart || index < 0 || index >= cart->itemCount) return NULL;
    CartNode *node = cart->head;
    for (int i = 0; i < index; i++) node = node->next;
    return node;
}

int cartRemoveItemByIndex(Cart *cart, int index) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (!node) return 0;
    if (node->prev) node->prev->next = node->next;
    else cart->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else cart->tail = node->prev;
    free(node);
    cart->itemCount--;
    return 1;
}

int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (!node) return 0;
    node->item.quantity = newQuantity;
    node->item.totalPrice = node->item.price * newQuantity;
    return 1;
}
```

---

## 4. Sửa `file_service.c` để tạo hash và load menu

### 4.1. Khởi tạo hash table trong `loadMenuFromFile()`

- Bao gồm header `#include "../services/hash_service.h"`.
- Tạo `menuHashTable = createHashTable(101);` trước khi đọc file.
- Gọi `hashTableInsert(menuHashTable, &menu[i]);` sau khi nạp mỗi `MenuItem`.

Ví dụ:

```c
extern HashTable *menuHashTable;

void loadMenuFromFile(const char* filename) {
    menuHashTable = createHashTable();
    ...
    while (fgets(line, sizeof(line), file)) {
        ...
        hashTableInsert(menuHashTable, menu[i].id, menu[i]);
        i++;
    }
}
```

### 4.2. Không dùng `id - 1` trực tiếp

- Trong phần load menu, menu[] vẫn dùng để lưu danh sách.
- Những thao tác lookup sau này phải dùng `hashTableSearch(menuHashTable, idNhap)` để truy xuất `MenuItem` chính xác.

---

## 5. Sửa `order_controller.c` theo đúng yêu cầu

### 5.1. Chỉ dùng `Hash Table` cho phần đặt món

Tạo hàm `placeOrder()` hoặc dùng `handleOrderMenu()` để:

1. Nhập `idNhap`.
2. Tìm `MenuItem *menuItem = hashTableSearch(menuHashTable, idNhap);`.
3. Kiểm tra tồn kho / số lượng / giới hạn món chính.
4. Tạo `OrderItem` từ `MenuItem`.
5. Gọi `addToCart(currentBill, newItem);`.

Ví dụ:

```c
void handleOrderMenu(Bill *currentBill) {
    while (1) {
        int idNhap = getValidInt("\n>> Nhap Ma Mon (0 de thoat): ");
        if (idNhap == 0) break;

        MenuItem *menuItem = hashTableSearch(menuHashTable, idNhap);
        if (!menuItem) {
            printf("  !! Loi: Ma mon ko ton tai!\n");
            continue;
        }

        int countMain = 0;
        CartNode *tmp = currentBill->cart.head;
        while (tmp) {
            if (tmp->item.id <= 12) countMain += tmp->item.quantity;
            tmp = tmp->next;
        }
        if (idNhap <= 12 && countMain >= 5) {
            printf("  !! Thong bao: Ban da dat du 5 mon chinh.\n");
            continue;
        }

        int slNhap = getValidInt(">> Nhap so luong: ");
        if (slNhap <= 0 || slNhap > menuItem->stock) {
            printf("  !! Loi: Kho ko du hoac SL ko hop le (Con: %d)\n", menuItem->stock);
            continue;
        }

        int opt = 0;
        double giaThucTe = menuItem->price;
        if (menuItem->hasOptions) {
            while (1) {
                printf("  1. Cot let | 2. Suon cay (+10k)\n");
                opt = getValidInt("  >> Chon loai: ");
                if (opt == 1 || opt == 2) {
                    if (opt == 2) giaThucTe += 10000;
                    break;
                }
                printf("  !! Loi: Chi chon 1 hoac 2.\n");
            }
        }

        char ghiChu[100] = "";
        printf("  >> Ghi chu (Bam Enter bo qua): ");
        fgets(ghiChu, sizeof(ghiChu), stdin);
        ghiChu[strcspn(ghiChu, "\n")] = 0;

        OrderItem newItem;
        newItem.id = menuItem->id;
        strcpy(newItem.name, menuItem->name);
        newItem.price = giaThucTe;
        newItem.quantity = slNhap;
        newItem.option = opt;
        strcpy(newItem.note, ghiChu);
        newItem.totalPrice = giaThucTe * slNhap;

        addToCart(currentBill, newItem);
        menuItem->stock -= slNhap;
    }
}
```

### 5.2. Hàm `addToCart()` chỉ dành cho DLL

Sửa chữ ký và nội dung `addToCart()`:

```c
void addToCart(Bill *currentBill, OrderItem item) {
    cartAddItem(&currentBill->cart, item);
}
```

- Hàm này không cần tìm `MenuItem` và không cần đọc `menu[id-1]`.
- Nó chỉ xử lý thêm `OrderItem` vào `Doubly Linked List`.

---

## 6. Sửa `removeFromCart()` và `updateQuantity()`

### 6.1. `removeFromCart()`

- Lấy node giỏ hàng theo STT bằng `cartGetNodeByIndex()`.
- Tra cứu `MenuItem` bằng `hashTableSearch(menuHashTable, node->item.id)`.
- Hoàn trả tồn kho.
- Xóa node qua `cartRemoveItemByIndex()`.

Ví dụ:

```c
void removeFromCart(Bill *currentBill) {
    if (currentBill->cart.itemCount == 0) return;
    int stt = getValidInt(">> Nhap STT mon muon xoa: ");
    if (stt < 1 || stt > currentBill->cart.itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    if (!node) return;

    MenuItem *menuItem = hashTableSearch(menuHashTable, node->item.id);
    if (menuItem) menuItem->stock += node->item.quantity;
    cartRemoveItemByIndex(&currentBill->cart, idx);
    printf("  => Da xoa mon va hoan lai ton kho!\n");
}
```

### 6.2. `updateQuantity()`

- Lấy node theo STT.
- Kiểm tra giới hạn món chính nếu cần.
- Kiểm tra tồn kho với `menuItem->stock`.
- Cập nhật `cartUpdateQuantityByIndex()`.
- Điều chỉnh `menuItem->stock` theo `diff`.

Ví dụ:

```c
void updateQuantity(Bill *currentBill) {
    if (currentBill->cart.itemCount == 0) return;
    int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
    if (stt < 1 || stt > currentBill->cart.itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    if (!node) return;

    int newQty = getValidInt(">> Nhap so luong moi: ");
    if (newQty <= 0) {
        printf("  !! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.\n");
        return;
    }

    MenuItem *menuItem = hashTableSearch(menuHashTable, node->item.id);
    int oldQty = node->item.quantity;
    if (node->item.id <= 12) {
        int countMainExceptThis = 0;
        CartNode *tmp = currentBill->cart.head;
        while (tmp) {
            if (tmp != node && tmp->item.id <= 12) countMainExceptThis += tmp->item.quantity;
            tmp = tmp->next;
        }
        if (countMainExceptThis + newQty > 5) {
            printf("  !! Loi: Sua SL khien tong mon chinh > 5.\n");
            return;
        }
    }

    int diff = newQty - oldQty;
    if (diff > 0 && menuItem && diff > menuItem->stock) {
        printf("  !! Loi: Kho khong du hang (Con: %d).\n", menuItem->stock);
        return;
    }

    if (menuItem) menuItem->stock -= diff;
    cartUpdateQuantityByIndex(&currentBill->cart, idx, newQty);
    printf("  => Da cap nhat so luong!\n");
}
```

---

## 7. Sửa `cart_ui.c` để hiện thị Doubly Linked List

- Thay `for` duyệt `currentBill->items[i]` bằng duyệt `CartNode *node = currentBill->cart.head`.
- In STT theo thứ tự node đi qua.

Ví dụ:

```c
void showCartUI(Bill *currentBill) {
    printf("\n============================= GIO HANG CUA BAN ==============================\n");
    if (currentBill->cart.itemCount == 0) {
        printf("          ( Gio hang hien dang trong )\n");
        printf("==========================================================\n");
        return;
    }

    double tamTinh = 0;
    printf("%-3s | %-28s | %-4s | %10s | %12s\n", "STT", "Ten mon", "SL", "Don gia", "Thanh tien");
    printf("=============================================================================\n");

    CartNode *node = currentBill->cart.head;
    int i = 1;
    while (node) {
        OrderItem item = node->item;
        printf("%-3d | %-25.25s | %-3d | %12.0lf | %15.0lf\n",
               i, item.name, item.quantity, item.price, item.totalPrice);
        if (item.option > 0) {
            printf("    | [Loai: %s]\n", item.option == 2 ? "Suon cay" : "Cot let");
        }
        if (strlen(item.note) > 0) {
            printf("    | [Ghi chu: %s]\n", item.note);
        }
        tamTinh += item.totalPrice;
        node = node->next;
        i++;
    }

    printf("=============================================================================\n");
    printf(" >> TONG TAM TINH: %41.0lf VND\n", tamTinh);
    printf("=============================================================================\n");
}
```

---

## 8. Sửa `main.c` để khởi tạo giỏ hàng

- Trong `main.c`, sau khi tạo `Bill currentBill;`, gọi `cartInit(&currentBill.cart);`.
- Nếu `currentBill` là biến toàn cục hoặc static, vẫn cần khởi tạo trước khi sử dụng.

---

## 9. Ghi chú quan trọng

- `addToCart()` bây giờ chỉ là hàm chèn `OrderItem` vào `Cart`.
- Hàm đặt món (menu lookup) phải dùng `hashTableSearch()` để lấy `MenuItem`.
- Sau khi tạo `OrderItem`, gọi `addToCart(currentBill, newItem);`.
- Không dùng `menu[id-1]` trực tiếp trong phần đặt món.
- `HashTable` chỉ phục vụ tìm nhanh menu; `Doubly Linked List` chỉ phục vụ quản lý giỏ hàng.

---

## 10. Đề xuất cây workflow sửa code

1. Sửa `app/models/models.h`.
2. Tạo `app/services/hash_service.h` và `app/services/hash_service.c`.
3. Tạo `app/services/cart_service.h` và `app/services/cart_service.c`.
4. Sửa `app/services/file_service.c` để build hash table.
5. Sửa `app/controllers/order_controller.c` theo hai tầng:
   - `handleOrderMenu()` chỉ lookup + tạo `OrderItem`.
   - `addToCart()` chỉ thêm vào DLL.
6. Sửa `app/ui/cart_ui.c` để duyệt DLL.
7. Sửa `main.c` để khởi tạo giỏ hàng.

---

## 11. Kết luận

Với các sửa này, bạn sẽ hiện thực đúng ý tưởng:
- `menu.txt` → `Hash Table` để tìm món nhanh.
- `Doubly Linked List` để quản lý giỏ hàng theo thứ tự thêm.
- `addToCart()` chỉ dùng sau khi đã xác định `OrderItem` từ menu lookup.

## Các file cần sửa chính xác

### 1. app/controllers/order_controller.h
- Sửa chữ ký `addToCart` để nhận `OrderItem`.
- Thay:
```c
void addToCart(Bill *currentBill);
```
- Bằng:
```c
void addToCart(Bill *currentBill, OrderItem item);
```

### 2. app/services/checkout_service.c
- Sửa `calculateSubtotal()` để tính từ `Bill->cart` thay vì `bill->items`.
- Thay toàn bộ hàm bằng:
```c
#include "checkout_service.h"
#include <string.h>

double calculateSubtotal(Bill* bill) {
    double total = 0;
    CartNode *node = bill->cart.head;
    while (node) {
        total += node->item.price * node->item.quantity;
        node = node->next;
    }
    return total;
}
```

### 3. app/controllers/checkout_controller.c
- Sửa kiểm tra giỏ hàng rỗng từ `currentBill->itemCount` sang `currentBill->cart.itemCount`.
- Thay:
```c
if (currentBill->itemCount == 0) {
```
- Bằng:
```c
if (currentBill->cart.itemCount == 0) {
```

- Sau khi thanh toán thành công, xóa giỏ hàng bằng cách reset `Cart` thay vì `currentBill->itemCount = 0`.
- Thêm hoặc thay bằng:
```c
currentBill->cart.head = NULL;
currentBill->cart.tail = NULL;
currentBill->cart.itemCount = 0;
```

### 4. app/services/history_service.c
- Sửa `saveBillToHistoryFile()` và `saveBillToGlobalFile()` để lần lượt ghi từng item từ `bill->cart.head`.
- Thay vòng lặp cũ dùng `bill->items[i]` bằng:
```c
CartNode *node = bill->cart.head;
while (node) {
    fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%.3f|%s\n",
            node->item.id,
            node->item.name,
            node->item.price,
            node->item.quantity,
            node->item.option,
            node->item.totalPrice,
            node->item.note);
    node = node->next;
}
```
- Sửa trường thông tin bill từ `bill->itemCount` sang `bill->cart.itemCount` khi ghi dòng `BILL|...`.

- Trong `loadHistoryFromFile()`, khi đọc dòng `ITEM|...`, đừng gán vào `tempBill.items[itemCount]` nữa.
- Thay bằng mẫu:
```c
OrderItem item;
item.id = id;
strcpy(item.name, name);
item.price = price;
item.quantity = quantity;
item.option = option;
item.totalPrice = totalPrice;
strcpy(item.note, note);
cartAddItem(&tempBill.cart, item);
itemCount++;
```
- Không dùng `tempBill.itemCount`; dùng `tempBill.cart.itemCount` hoặc chỉ cần giữ `itemCount` cho việc đọc file.

### 5. app/ui/history_ui.c
- Sửa toàn bộ các vòng lặp in lịch sử từ `current->bill.items[i]` và `billNode->bill.items[i]` sang duyệt `CartNode *node = current->bill.cart.head`.
- Thay `for (int i = 0; i < current->bill.itemCount; i++)` bằng:
```c
int i = 1;
CartNode *node = current->bill.cart.head;
while (node) {
    ... node->item ...
    node = node->next;
    i++;
}
```
- Tương tự với `printBillDetail()`.

### 6. app/ui/bill_ui.c
- Sửa `printBillUI()` để in hóa đơn từ `bill->cart`.
- Thay vòng `for (int i = 0; i < bill->itemCount; i++)` bằng:
```c
int i = 1;
CartNode *node = bill->cart.head;
while (node) {
    double thanhTienMon = node->item.price * node->item.quantity;
    printf("%-4d | %-40.40s | %-5d | %15.3f\n",
           i, node->item.name, node->item.quantity, thanhTienMon);
    node = node->next;
    i++;
}
```

### 7. app/services/file_service.c
- Nếu chưa sửa, phải đảm bảo:
  - `#include "../services/hash_service.h"`
  - `menuHashTable = createHashTable();`
  - `hashTableInsert(menuHashTable, menu[i].id, menu[i]);`

### 8. app/controllers/order_controller.c
- Nếu chưa có, giữ `addToCart()` như sau để kiểm tra món đã tồn tại trong giỏ:
```c
void addToCart(Bill *currentBill, OrderItem item) {
    CartNode *existing = findCartNodeById(&currentBill->cart, item.id);
    if (existing) {
        existing->item.quantity += item.quantity;
        existing->item.totalPrice = existing->item.quantity * existing->item.price;
        return;
    }
    cartAddItem(&currentBill->cart, item);
}
```

### 9. Kiểm tra lại `app/models/models.h`
- `Bill` phải chứa `Cart cart;`.
- `HashTable` phải dùng `HashNode **table;` và `extern HashTable* menuHashTable;`.

---

## Ghi chú quan trọng
- Các file dùng `bill->items` phải chuyển sang `bill->cart.head`/`bill->cart.itemCount`.
- Nơi nào dùng `itemCount` trực tiếp trên `Bill` cần sửa lại thành `cart.itemCount`.
- `order_controller.c` là nơi giữ logic `addToCart()` và kiểm tra trùng item.
- `history_service.c` là nơi cần cập nhật cả ghi file và đọc file lịch sử.

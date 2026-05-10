# Các phần cần sửa để thực hiện ý tưởng mới

## 1. Thay đổi trong models.h

### Thêm HashTable cho menu
- Thêm `extern HashTable* menuHash;` để lưu menu từ file.

### Sửa struct Bill
- Bỏ `HashTable* itemsHash;` và `DoublyLinkedList* cartDisplay;`
- Thêm `DoublyLinkedList* cart;` để quản lý giỏ hàng.
- Giữ `OrderItem* items;` và `int itemCount;` để tương thích với history (hoặc bỏ nếu không cần).

```c
typedef struct {
    int id;
    Customer customer;
    DoublyLinkedList* cart;  // Thay thế itemsHash và cartDisplay
    OrderItem* items;        // Giữ để tương thích history
    int itemCount;
    double total;
    double discount;
    double finalPrice;
    char dateTime[50];
} Bill;
```

## 2. Tạo service mới cho menu hash table

### Tạo file app/services/menu_hash_service.h và .c
- Hàm `HashTable* loadMenuToHashTable(const char* filename);`
- Hàm `MenuItem* getMenuItemFromHash(HashTable* ht, int id);`

## 3. Sửa order_controller.c

### Sửa handleOrderMenu
- Thêm khởi tạo `menuHash` nếu chưa có.
- Thay vì khởi tạo `itemsHash` và `cartDisplay`, chỉ khởi tạo `cart`.

### Sửa addToCart
- Thay vì dùng `menu[idx]`, dùng `getMenuItemFromHash(menuHash, idNhap)`.
- Thay vì insert vào `itemsHash`, append trực tiếp vào `currentBill->cart`.
- Cập nhật `currentBill->itemCount = dllGetCount(currentBill->cart);`
- Bỏ logic `dllAppend` riêng, vì giờ cart là DLL chính.

### Sửa removeFromCart và updateQuantity
- Thay vì thao tác trên `itemsHash` và `cartDisplay`, chỉ thao tác trên `cart` (DLL).
- Dùng `dllRemoveAt`, `dllUpdateQuantityAt` (cần thêm hàm này nếu chưa có).

## 4. Sửa cart_ui.c

### Sửa showCartUI
- Thay vì `hashTableToArray`, duyệt trực tiếp `currentBill->cart` bằng `dllTraverse`.
- Tính `tamTinh` trong lúc duyệt.

## 5. Sửa checkout_service.c

### Sửa calculateSubtotal
- Thay vì `hashTableToArray`, duyệt `bill->cart` để tính tổng.

## 6. Sửa history_service.c

### Sửa addBillToHistory và saveBillToHistoryFile
- Thay vì copy từ `itemsHash`, copy từ `bill->cart` bằng cách duyệt DLL và tạo mảng `OrderItem`.

### Sửa saveBillToGlobalFile
- Tương tự, dùng `bill->cart` thay vì `itemsHash`.

## 7. Thay đổi trong main.c hoặc nơi khởi tạo Bill

- Khi tạo `Bill currentBill = {0};`, đảm bảo `currentBill.cart = dllCreate();`
- Load `menuHash` từ file khi khởi động ứng dụng.

## 8. Các hàm DLL cần bổ sung

- `int dllGetCount(DoublyLinkedList* dll);`
- `void dllUpdateQuantityAt(DoublyLinkedList* dll, int index, int newQty);` (hoặc sửa `dllUpdateQuantity` để nhận index)
- Hàm duyệt DLL để lấy mảng OrderItem cho history.

---

Những thay đổi này sẽ tách rõ: Hash Table chỉ dùng cho menu lookup, Doubly Linked List dùng cho giỏ hàng với 3 chức năng thêm/xóa/chỉnh sửa.
# Plan_CN1+2.md - Hướng dẫn triển khai Hash Table cho CN1 và Hash Table + DLL cho CN2

## Tổng quan
Bạn đã hoàn thành Phase 1 (tạo hash_table_service và doubly_linked_list_service). File này hướng dẫn chi tiết cách sửa từng file từ Phase 2 trở đi để tích hợp Hash Table cho CN1 (đặt món) và Hash Table + Doubly Linked List cho CN2 (giỏ hàng).

---

## Phase 2: Cập nhật Data Models

### 2.1 Sửa `app/models/models.h`
**Mục đích**: Struct `Bill` đã được cập nhật với `HashTable* itemsHash` và `DoublyLinkedList* cartDisplay`.

**Thay đổi**:
- Struct `Bill` đã có `itemsHash` và `cartDisplay`
- Đã include các header service

### 2.2 Sửa `main.c`
**Mục đích**: Khởi tạo Hash Table và DLL cho Bill.

**Thay đổi**:
```c
// Trong main(), thay thế:
Bill currentBill = {0}; // Khởi tạo giỏ hàng rỗng

// Thành:
Bill currentBill = {0};
currentBill.itemsHash = hashTableCreate(HASH_TABLE_SIZE);
currentBill.cartDisplay = dllCreate();
currentBill.itemCount = 0;

// Trước return 0; thêm:
hashTableFree(currentBill.itemsHash);
dllFree(currentBill.cartDisplay);
```

---

## Phase 3: Refactor Controllers & Services

### 3.1 Sửa `app/controllers/order_controller.c`

#### 3.1.1 Sửa `addToCart(Bill *currentBill)`
**Code hiện tại** (dùng array):
```c
// Kiểm tra giới hạn
if (currentBill->itemCount >= MAX_GIO_HANG) {
    printf("\n[!] Gio hang da day! Vui long thanh toan.\n");
    break;
}
// ... nhập id, sl, option, note
// Đưa vào giỏ hàng
int pos = currentBill->itemCount;
currentBill->items[pos].id = idNhap;
// ... copy các field
menu[idx].stock -= slNhap;
currentBill->itemCount++;
```

**Code mới** (dùng hash table + DLL):
```c
// Loại bỏ kiểm tra MAX_GIO_HANG vì hash table không giới hạn

// ... nhập id, sl, option, note (giữ nguyên)

// Tạo OrderItem
OrderItem item;
item.id = idNhap;
strcpy(item.name, menu[idx].name);
item.price = giaThucTe;
item.quantity = slNhap;
item.option = opt;
strcpy(item.note, ghiChu);
item.totalPrice = giaThucTe * slNhap;

// Insert vào hash table (tự động cộng SL nếu trùng)
int result = hashTableInsert(currentBill->itemsHash, &item);
if (result == 0) {
    // Nếu item mới (không phải duplicate), thêm vào DLL
    OrderItem* existing = hashTableGet(currentBill->itemsHash, item.id);
    if (existing && existing->quantity == slNhap) {  // Item mới
        dllAppend(currentBill->cartDisplay, &item);
    }
    currentBill->itemCount = currentBill->itemsHash->count;
    
    menu[idx].stock -= slNhap;
    printf("  => Da them %d '%s'", slNhap, menu[idx].name);
    if (menu[idx].hasOptions) {
        if (opt == 1) printf(" (Cot let)");
        else printf(" (Suon cay)");
    }
    printf(" vao gio!\n");
} else {
    printf("  !! Loi: Khong the them mon an!\n");
}
```

#### 3.1.2 Sửa `removeFromCart(Bill *currentBill)`
**Code hiện tại**:
```c
int stt = getValidInt(">> Nhap STT mon muon xoa: ");
if (stt < 1 || stt > currentBill->itemCount) {
    printf("  !! STT khong hop le.\n");
    return;
}

int idx = stt - 1;
int menuIdx = currentBill->items[idx].id - 1;

// Hoàn trả tồn kho
menu[menuIdx].stock += currentBill->items[idx].quantity;

// Dịch chuyển mảng để xóa
for (int i = idx; i < currentBill->itemCount - 1; i++) {
    currentBill->items[i] = currentBill->items[i + 1];
}
currentBill->itemCount--;
```

**Code mới**:
```c
int stt = getValidInt(">> Nhap STT mon muon xoa: ");
if (stt < 1 || stt > currentBill->itemCount) {
    printf("  !! STT khong hop le.\n");
    return;
}

// Lấy item từ DLL
DLLNode* node = dllGetNodeAt(currentBill->cartDisplay, stt);
if (!node) {
    printf("  !! Loi: Khong tim thay item!\n");
    return;
}

int itemId = node->item.id;
int qty = node->item.quantity;
int menuIdx = itemId - 1;

// Xóa khỏi hash table
hashTableDelete(currentBill->itemsHash, itemId);

// Xóa khỏi DLL
dllRemoveAt(currentBill->cartDisplay, stt);

// Hoàn trả tồn kho
menu[menuIdx].stock += qty;

currentBill->itemCount = currentBill->cartDisplay->count;
printf("  => Da xoa mon va hoan lai ton kho!\n");
```

#### 3.1.3 Sửa `updateQuantity(Bill *currentBill)`
**Code hiện tại**:
```c
int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
if (stt < 1 || stt > currentBill->itemCount) {
    printf("  !! STT khong hop le.\n");
    return;
}

int idx = stt - 1;
int menuIdx = currentBill->items[idx].id - 1;
int oldQty = currentBill->items[idx].quantity;

// ... kiểm tra newQty, stock, món chính
int diff = newQty - oldQty;
menu[menuIdx].stock -= diff;
currentBill->items[idx].quantity = newQty;
currentBill->items[idx].totalPrice = newQty * currentBill->items[idx].price;
```

**Code mới**:
```c
int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
if (stt < 1 || stt > currentBill->itemCount) {
    printf("  !! STT khong hop le.\n");
    return;
}

// Lấy node từ DLL
DLLNode* node = dllGetNodeAt(currentBill->cartDisplay, stt);
if (!node) {
    printf("  !! Loi: Khong tim thay item!\n");
    return;
}

int itemId = node->item.id;
int menuIdx = itemId - 1;
int oldQty = node->item.quantity;

int newQty = getValidInt(">> Nhap so luong moi: ");
if (newQty <= 0) {
    printf("  !! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.\n");
    return;
}

// Kiểm tra món chính
if (itemId <= 12) {
    int countMainExceptThis = 0;
    // Đếm từ hash table
    int count;
    OrderItem* arr = hashTableToArray(currentBill->itemsHash, &count);
    for (int i = 0; i < count; i++) {
        if (arr[i].id <= 12 && arr[i].id != itemId) {
            countMainExceptThis += arr[i].quantity;
        }
    }
    free(arr);
    if (countMainExceptThis + newQty > 5) {
        printf("  !! Loi: Sua SL khien tong mon chinh > 5.\n");
        return;
    }
}

// Kiểm tra stock
int diff = newQty - oldQty;
if (diff > menu[menuIdx].stock) {
    printf("  !! Loi: Kho khong du hang (Con: %d).\n", menu[menuIdx].stock);
    return;
}

// Cập nhật DLL
dllUpdateQuantity(currentBill->cartDisplay, stt, newQty);

// Cập nhật hash table
hashTableUpdateQuantity(currentBill->itemsHash, itemId, newQty);

menu[menuIdx].stock -= diff;
printf("  => Da cap nhat so luong!\n");
```

#### 3.1.4 Sửa `handleOrderMenu(Bill* currentBill)`
**Thêm khởi tạo**:
```c
void handleOrderMenu(Bill* currentBill) {
    // Khởi tạo nếu chưa có
    if (!currentBill->itemsHash) {
        currentBill->itemsHash = hashTableCreate(HASH_TABLE_SIZE);
    }
    if (!currentBill->cartDisplay) {
        currentBill->cartDisplay = dllCreate();
    }
    
    showMenuUI();
    addToCart(currentBill);
}
```

---

### 3.2 Sửa `app/ui/cart_ui.c`

#### 3.2.1 Sửa `showCartUI(Bill *currentBill)`
**Code hiện tại**:
```c
for (int i = 0; i < currentBill->itemCount; i++) {
    OrderItem item = currentBill->items[i];
    printf("%-3d | %-25.25s | %-3d | %12.0lf | %15.0lf\n", 
           i + 1, item.name, item.quantity, item.price, item.totalPrice);
    // ... in option, note
    tamTinh += item.totalPrice;
}
```

**Code mới**:
```c
// Lấy array từ hash table
int count;
OrderItem* arr = hashTableToArray(currentBill->itemsHash, &count);

for (int i = 0; i < count; i++) {
    OrderItem item = arr[i];
    printf("%-3d | %-25.25s | %-3d | %12.0lf | %15.0lf\n", 
           i + 1, item.name, item.quantity, item.price, item.totalPrice);
    
    if (item.option > 0) {
        printf("    | [Loai: %s]\n", item.option == 2 ? "Suon cay" : "Cot let");
    }
    if (strlen(item.note) > 0) {
        printf("    | [Ghi chu: %s]\n", item.note);
    }
    tamTinh += item.totalPrice;
}

free(arr);
```

---

### 3.3 Sửa `app/services/checkout_service.c`

#### 3.3.1 Sửa `calculateSubtotal(Bill* bill)`
**Code hiện tại**:
```c
double calculateSubtotal(Bill* bill) {
    double total = 0;
    for (int i = 0; i < bill->itemCount; i++) {
        total += bill->items[i].price * bill->items[i].quantity;
    }
    return total;
}
```

**Code mới**:
```c
double calculateSubtotal(Bill* bill) {
    double total = 0;
    
    // Lấy array từ hash table
    int count;
    OrderItem* arr = hashTableToArray(bill->itemsHash, &count);
    
    for (int i = 0; i < count; i++) {
        total += arr[i].price * arr[i].quantity;
    }
    
    free(arr);
    return total;
}
```

---

### 3.4 Sửa `app/services/history_service.c`

#### 3.4.1 Sửa `addBillToHistory(int customerId, Bill* bill)`
**Code hiện tại**:
```c
int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) {  
        printf("[!] Loi: Khong du bo nho\n");
        return -1;
    }
    newNode->customerId = customerId;
    newNode->bill = *bill; // Sao chép toàn bộ
    newNode->next = historyHead;
    historyHead = newNode;
    return 0;
}
```

**Code mới**:
```c
int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) {  
        printf("[!] Loi: Khong du bo nho\n");
        return -1;
    }
    newNode->customerId = customerId;
    
    // Copy thông tin cơ bản
    newNode->bill.id = bill->id;
    newNode->bill.customer = bill->customer;
    newNode->bill.total = bill->total;
    newNode->bill.discount = bill->discount;
    newNode->bill.finalPrice = bill->finalPrice;
    strcpy(newNode->bill.dateTime, bill->dateTime);
    
    // Convert hash table thành array
    int count;
    OrderItem* arr = hashTableToArray(bill->itemsHash, &count);
    newNode->bill.itemCount = count;
    
    // Giả sử HistoryNode có field OrderItem* items (cần sửa struct nếu chưa có)
    newNode->bill.items = (OrderItem*)malloc(count * sizeof(OrderItem));
    memcpy(newNode->bill.items, arr, count * sizeof(OrderItem));
    
    free(arr);
    
    newNode->next = historyHead;
    historyHead = newNode;
    return 0;
}
```

**Lưu ý**: Cần sửa struct `HistoryNode` để có `OrderItem* items` thay vì array tĩnh.

#### 3.4.2 Sửa `saveBillToHistoryFile(int customerId, Bill* bill)`
**Code hiện tại**:
```c
// Lưu từng item
for (int i = 0; i < bill->itemCount; i++) {
    fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%.3f|%s\n",
            bill->items[i].id,
            bill->items[i].name,
            bill->items[i].price,
            bill->items[i].quantity,
            bill->items[i].option,
            bill->items[i].totalPrice,
            bill->items[i].note);
}
```

**Code mới**:
```c
// Convert và lưu items
int count;
OrderItem* arr = hashTableToArray(bill->itemsHash, &count);

// Lưu từng item
for (int i = 0; i < count; i++) {
    fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%.3f|%s\n",
            arr[i].id,
            arr[i].name,
            arr[i].price,
            arr[i].quantity,
            arr[i].option,
            arr[i].totalPrice,
            arr[i].note);
}

free(arr);
```

**Tương tự cho `saveBillToGlobalFile()`**.

---

## Phase 4: Integration Testing
Test các flow: Đặt món → Xem giỏ → Sửa SL → Xóa → Thanh toán

## Phase 5: Optimization & Polish
- Kiểm tra memory leak
- Tối ưu hash collision
- Final testing

---

**Lưu ý quan trọng**:
- Đồng bộ `itemsHash` và `cartDisplay`
- Khi thêm: insert hash, nếu mới thì append DLL
- Khi xóa: delete hash, removeAt DLL
- Khi update: update cả hash và DLL
- Dùng `hashTableToArray()` cho hiển thị/lưu trữ
- Giải phóng memory trong main.c

**Cập nhật lần cuối**: 09/05/2026
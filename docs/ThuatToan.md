# 📐 Thuật Toán Các Hàm Chính — PBL1

> Tất cả các bước đều được viết dựa trên **code thực tế** của dự án.

---

## 1. `hashTableSearch` — Tìm món ăn theo ID

**File:** `app/services/hash_service.c`  
**Mục đích:** Tra cứu thông tin món ăn (`MenuItem`) trong bảng băm theo mã món (ID).

```c
MenuItem *hashTableSearch(HashTable *ht, int key) {
    int index = hash(key, ht->size);        // key % 101
    HashNode *node = ht->table[index];
    while (node) {
        if (node->key == key) return &node->item;
        node = node->next;
    }
    return NULL;
}
```

### Các bước thuật toán:

- **B1:** Nhận vào bảng băm `ht` và khóa tìm kiếm `key` (ID món ăn).
- **B2:** Tính vị trí bucket: `index = key % 101`.
- **B3:** Trỏ `node` vào đầu danh sách liên kết tại `table[index]`.
- **B4:** Kiểm tra `node` có NULL không?
  - Nếu **có** → không tìm thấy, trả về `NULL`. **(KẾT THÚC)**
- **B5:** So sánh `node->key` với `key`:
  - Nếu **bằng** → tìm thấy, trả về địa chỉ `&node->item`. **(KẾT THÚC)**
  - Nếu **khác** → đặt `node = node->next`, quay lại **B4**.

---

## 2. Cart — Giỏ Hàng (Doubly Linked List)

**File:** `app/controllers/order_controller.c` + `app/services/cart_service.c`

---

### 2.1. `addToCart` — Thêm món vào giỏ

**Mục đích:** Thêm `OrderItem` vào giỏ hàng. Nếu món đã tồn tại (cùng ID, option, ghi chú) thì cộng thêm số lượng.

```c
void addToCart(Bill *currentBill, OrderItem item) {
    CartNode *node = currentBill->cart.head;
    while (node) {
        if (node->item.id == item.id &&
            node->item.option == item.option &&
            strcmp(node->item.note, item.note) == 0) {
            node->item.quantity += item.quantity;
            node->item.totalPrice = node->item.quantity * node->item.price;
            return;
        }
        node = node->next;
    }
    cartAddItem(&currentBill->cart, item);
}
```

#### Các bước thuật toán:

- **B1:** Nhận vào giỏ hàng `currentBill` và món mới `item`.
- **B2:** Trỏ `node` vào `head` của giỏ hàng.
- **B3:** Duyệt từng node trong giỏ:
  - **B3a:** Kiểm tra `node` có NULL không?
    - Nếu **có** → không tìm thấy món trùng, chuyển sang **B4**.
  - **B3b:** So sánh `node->item.id`, `node->item.option`, `node->item.note` với `item`:
    - Nếu **trùng** → cộng thêm số lượng: `quantity += item.quantity`, cập nhật `totalPrice`. **(KẾT THÚC)**
    - Nếu **khác** → `node = node->next`, quay lại **B3a**.
- **B4:** Gọi `cartAddItem()` để tạo node mới và nối vào cuối danh sách.

---

### 2.2. `removeFromCart` — Xóa món khỏi giỏ

**Mục đích:** Nhận STT từ người dùng, xóa node tương ứng khỏi danh sách liên kết đôi.

```c
void removeFromCart(Bill *currentBill) {
    int stt = getValidInt(">> Nhap STT mon muon xoa: ");
    // validate stt...
    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    cartRemoveItemByIndex(&currentBill->cart, idx);
}

// cartRemoveItemByIndex:
int cartRemoveItemByIndex(Cart *cart, int index) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (node->prev) node->prev->next = node->next;
    else cart->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else cart->tail = node->prev;
    free(node);
    cart->itemCount--;
    return 1;
}
```

#### Các bước thuật toán:

- **B1:** Nhận STT từ bàn phím, kiểm tra hợp lệ (`1 ≤ stt ≤ itemCount`).
- **B2:** Tính `index = stt - 1`.
- **B3:** Duyệt từ `head`, đi qua `index` lần để lấy con trỏ `node` cần xóa.
- **B4:** Nối lại danh sách bỏ qua `node`:
  - Nếu `node` có `prev` → `node->prev->next = node->next`.
  - Nếu `node` là HEAD → cập nhật `cart->head = node->next`.
  - Nếu `node` có `next` → `node->next->prev = node->prev`.
  - Nếu `node` là TAIL → cập nhật `cart->tail = node->prev`.
- **B5:** Giải phóng bộ nhớ: `free(node)`.
- **B6:** Giảm `cart->itemCount--`. **(KẾT THÚC)**

---

### 2.3. `updateQuantity` — Cập nhật số lượng món

**Mục đích:** Nhận STT và số lượng mới, cập nhật node tương ứng trong giỏ hàng.

```c
void updateQuantity(Bill *currentBill) {
    int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
    int newQty = getValidInt(">> Nhap so luong moi: ");
    // validate...
    cartUpdateQuantityByIndex(&currentBill->cart, idx, newQty);
}

// cartUpdateQuantityByIndex:
int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    node->item.quantity = newQuantity;
    node->item.totalPrice = node->item.price * newQuantity;
    return 1;
}
```

#### Các bước thuật toán:

- **B1:** Nhận STT từ bàn phím, kiểm tra hợp lệ.
- **B2:** Nhận số lượng mới `newQty` từ bàn phím, kiểm tra `newQty > 0`.
- **B3:** Nếu là món chính (`id ≤ 12`): kiểm tra tổng số lượng món chính sau khi sửa không vượt quá 5.
- **B4:** Duyệt từ `head` đến vị trí `index` để lấy `node`.
- **B5:** Cập nhật `node->item.quantity = newQty`.
- **B6:** Tính lại `node->item.totalPrice = price × newQty`. **(KẾT THÚC)**

---

## 3. `searchBTree` — Tìm khách hàng theo số điện thoại

**File:** `app/services/btree_service.c`  
**Mục đích:** Tìm kiếm đệ quy trong B-Tree bậc 4, so sánh SĐT theo chuỗi (`strcmp`).

```c
Customer* searchBTree(BTreeNode* root, char* phone) {
    if (root == NULL) return NULL;
    int i = 0;
    while (i < root->numKeys && strcmp(phone, root->customers[i]->phone) > 0) i++;
    if (i < root->numKeys && strcmp(phone, root->customers[i]->phone) == 0)
        return root->customers[i];
    if (root->isLeaf) return NULL;
    return searchBTree(root->children[i], phone);
}
```

### Các bước thuật toán:

- **B1:** Nhận vào node hiện tại `root` và SĐT cần tìm `phone`.
- **B2:** Nếu `root == NULL` → cây rỗng hoặc đã đi hết, trả về `NULL`. **(KẾT THÚC)**
- **B3:** Đặt `i = 0`. Duyệt từ trái sang phải trong mảng `customers[]` của node:
  - Khi `i < numKeys` và `phone > customers[i]->phone` → tăng `i++`.
  - Dừng khi tìm được vị trí phù hợp.
- **B4:** Kiểm tra `i < numKeys` và `phone == customers[i]->phone`:
  - Nếu **đúng** → tìm thấy, trả về `customers[i]`. **(KẾT THÚC)**
- **B5:** Kiểm tra `root->isLeaf`:
  - Nếu **là lá** → không tìm thấy, trả về `NULL`. **(KẾT THÚC)**
- **B6:** Đệ quy xuống node con: gọi `searchBTree(root->children[i], phone)`. Quay lại **B1**.

---

## 4. Lịch Sử Hóa Đơn — `history_service.c`

**Cấu trúc:** Singly Linked List (`HistoryNode`) — mỗi node lưu `customerId` + `Bill`.

---

### 4.1. `addBillToHistory` — Thêm hóa đơn vào lịch sử (bộ nhớ)

```c
int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) { return -1; }
    newNode->customerId = customerId;
    newNode->bill = *bill;
    newNode->next = historyHead;
    historyHead = newNode;
    return 0;
}
```

#### Các bước thuật toán:

- **B1:** Cấp phát bộ nhớ cho `HistoryNode` mới bằng `malloc`.
- **B2:** Nếu `malloc` trả về `NULL` → báo lỗi, trả về `-1`. **(KẾT THÚC)**
- **B3:** Gán `newNode->customerId = customerId`.
- **B4:** Sao chép toàn bộ dữ liệu hóa đơn: `newNode->bill = *bill`.
- **B5:** Trỏ `newNode->next = historyHead` (chèn vào đầu danh sách).
- **B6:** Cập nhật `historyHead = newNode`. Trả về `0` (thành công). **(KẾT THÚC)**

---

### 4.2. `getCustomerHistory` — Lấy lịch sử của một khách

```c
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* result = NULL;
    HistoryNode* current = historyHead;
    while (current != NULL) {
        if (current->customerId == customerId) {
            HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
            *newNode = *current;
            newNode->next = result;
            result = newNode;
        }
        current = current->next;
    }
    return result;
}
```

#### Các bước thuật toán:

- **B1:** Khởi tạo danh sách kết quả `result = NULL`. Trỏ `current = historyHead`.
- **B2:** Kiểm tra `current` có NULL không?
  - Nếu **có** → đã duyệt hết, trả về `result`. **(KẾT THÚC)**
- **B3:** Kiểm tra `current->customerId == customerId`:
  - Nếu **đúng**:
    - **B3a:** Tạo bản sao node: `malloc` → `*newNode = *current`.
    - **B3b:** Chèn bản sao vào đầu `result`: `newNode->next = result`, `result = newNode`.
- **B4:** Tiến sang node tiếp: `current = current->next`. Quay lại **B2**.

---

### 4.3. `searchBillById` — Tìm hóa đơn theo ID Bill

```c
HistoryNode* searchBillById(int billId) {
    HistoryNode* current = historyHead;
    while (current != NULL) {
        if (current->bill.id == billId) return current;
        current = current->next;
    }
    return NULL;
}
```

#### Các bước thuật toán:

- **B1:** Trỏ `current = historyHead`.
- **B2:** Kiểm tra `current` có NULL không?
  - Nếu **có** → không tìm thấy, trả về `NULL`. **(KẾT THÚC)**
- **B3:** So sánh `current->bill.id == billId`:
  - Nếu **đúng** → trả về `current`. **(KẾT THÚC)**
  - Nếu **sai** → `current = current->next`, quay lại **B2**.

---

## 5. Thống Kê Doanh Thu — `report_service.c`

**File:** `app/services/report_service.c`  
**Nguồn dữ liệu:** Đọc trực tiếp từ file `app/database/history.txt`.

---

### 5.1. `calculateTotalRevenue` — Tính tổng doanh thu

```c
double calculateTotalRevenue() {
    FILE* file = fopen("app/database/history.txt", "r");
    double totalRevenue = 0.0;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "BILL|", 5) == 0) {
            double finalPrice;
            sscanf(line, "BILL|%*d|%*d|%*[^|]|%*[^|]|%*[^|]|%*lf|%*lf|%lf", &finalPrice);
            totalRevenue += finalPrice;
        }
    }
    fclose(file);
    return totalRevenue;
}
```

#### Các bước thuật toán:

- **B1:** Mở file `history.txt` ở chế độ đọc.
- **B2:** Nếu mở thất bại → trả về `0.0`. **(KẾT THÚC)**
- **B3:** Khởi tạo `totalRevenue = 0`.
- **B4:** Đọc từng dòng bằng `fgets`:
  - Nếu hết file → chuyển sang **B6**.
- **B5:** Kiểm tra dòng có bắt đầu bằng `"BILL|"` không?
  - Nếu **đúng** → parse lấy `finalPrice` bằng `sscanf`, cộng vào `totalRevenue += finalPrice`.
  - Nếu **sai** → bỏ qua (dòng ITEM hoặc END).
  - Quay lại **B4**.
- **B6:** Đóng file. Trả về `totalRevenue`. **(KẾT THÚC)**

---

### 5.2. `analyzeByCustomer` — Thống kê doanh thu theo từng khách

```c
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount) {
    FILE* file = fopen("app/database/history.txt", "r");
    *customerCount = 0;
    char line[500];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "BILL|", 5) == 0) {
            int customerId; double finalPrice;
            sscanf(line, "BILL|%*d|%d|...|%lf|...", &customerId, &finalPrice);
            CustomerRevenue* cust = findOrCreateCustomer(customerStats, customerCount, customerId);
            if (cust != NULL) {
                cust->billCount++;
                cust->revenue += finalPrice;
            }
        }
    }
    fclose(file);
}
```

#### Các bước thuật toán:

- **B1:** Mở file `history.txt` ở chế độ đọc.
- **B2:** Nếu mở thất bại → thoát. **(KẾT THÚC)**
- **B3:** Đặt `*customerCount = 0`.
- **B4:** Đọc từng dòng bằng `fgets`:
  - Nếu hết file → chuyển sang **B7**.
- **B5:** Kiểm tra dòng có bắt đầu bằng `"BILL|"` không?
  - Nếu **sai** → bỏ qua, quay lại **B4**.
- **B6:** Parse `customerId` và `finalPrice` từ dòng BILL.
  - Gọi `findOrCreateCustomer()`:
    - Duyệt mảng `customerStats` tìm entry có cùng `customerId`.
    - Nếu tìm thấy → trả về con trỏ entry đó.
    - Nếu chưa có → tạo entry mới với `billCount=0`, `revenue=0`.
  - Tăng `cust->billCount++`.
  - Cộng dồn `cust->revenue += finalPrice`.
  - Quay lại **B4**.
- **B7:** Đóng file. **(KẾT THÚC)**

---

*📝 Tài liệu thuật toán — PBL1 Hệ Thống Quản Lý Quán Cơm Tấm*  
*🗓 Ngày tạo: 23/06/2026*

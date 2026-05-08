# Kiểm tra lại mã nguồn PBL1

## Kết luận chung
Sau khi kiểm tra lại các file hiện tại, mã nguồn **vẫn chưa sạch**. Một số phần đã được sửa tốt, nhưng dự án vẫn còn nhiều lỗi an toàn và logic đáng sửa, đặc biệt ở các module `file_service`, `history_service`, `customer_controller`, `report_service`, `history_ui`, và `order_controller`.

---

## Những chỗ đã cải thiện được
- `app/utils/validator.c` đã dùng `scanf("%14s", phone)` thay vì `scanf("%s", phone)`, đây là bước đúng để hạn chế tràn bộ đệm cho số điện thoại.
- `app/services/file_service.c` đã kiểm tra `fopen()` trong `saveAllCustomersToFile()` và không crash thẳng nếu mở file lỗi.
- `app/services/history_service.c` đã cập nhật `addBillToHistory()` để trả về mã lỗi khi malloc thất bại.
- `app/ui/history_ui.c` đã gọi `freeHistoryList(customerHistory)` sau khi in xong, khắc phục phần nào memory leak khi in lịch sử khách hàng.

---

## Vấn đề chính còn tồn tại và code sửa chi tiết

### 1. `app/services/file_service.c`
- `strcpy(menu[i].name, token);` không có giới hạn kích thước, dễ overflow nếu file menu có tên quá dài.
- `strcpy(c->phone, token);`, `strcpy(c->name, token);`, `strcpy(c->address, token);`, `strcpy(c->rank, token);` đều chưa dùng `strncpy()` hoặc bounds check.
- `Customer* c = malloc(sizeof(Customer));` được cấp phát nhưng **không được free** sau khi copy vào `danhSachKH`. Đây là memory leak.

#### Code sửa:
```c
// Trong loadMenuFromFile:
if (token != NULL) {
    strncpy(menu[i].name, token, sizeof(menu[i].name) - 1);
    menu[i].name[sizeof(menu[i].name) - 1] = '\0';
}

// Trong loadCustomersFromFile:
if (token) {
    strncpy(c->phone, token, sizeof(c->phone) - 1);
    c->phone[sizeof(c->phone) - 1] = '\0';
}
if (token) {
    strncpy(c->name, token, sizeof(c->name) - 1);
    c->name[sizeof(c->name) - 1] = '\0';
}
if (token) {
    strncpy(c->address, token, sizeof(c->address) - 1);
    c->address[sizeof(c->address) - 1] = '\0';
}
if (token) {
    strncpy(c->rank, token, sizeof(c->rank) - 1);
    c->rank[sizeof(c->rank) - 1] = '\0';
}

// Sau khi gán vào danhSachKH và insertBTree:
danhSachKH[soLuongKH] = *c;
insertBTree(root, &danhSachKH[soLuongKH]);
soLuongKH++;
free(c);  // Thêm dòng này để giải phóng memory
```

---

### 2. `app/controllers/customer_controller.c`
- Sai vị trí `soLuongKH++`: sau khi gán `danhSachKH[soLuongKH] = newCust;`, code tiếp tục `soLuongKH++;` rồi gọi `insertBTree(&btreeRoot, &danhSachKH[soLuongKH]);` và `currentBill->customer = danhSachKH[soLuongKH];`.
- Đây là lỗi chỉ số mảng: cần dùng chỉ số hiện tại trước khi tăng `soLuongKH`.

#### Code sửa:
```c
// Trong handleCustomer:
if (choice == 1) {
    Customer newCust;
    // ... populate newCust ...
    
    int idx = soLuongKH;  // Dùng chỉ số tạm
    danhSachKH[idx] = newCust;
    soLuongKH++;  // Tăng ngay sau khi gán
    insertBTree(&btreeRoot, &danhSachKH[idx]);
    currentBill->customer = danhSachKH[idx];
    
    // Gọi saveAllCustomersToFile();
}
```

---

### 3. `app/services/history_service.c`
- `sscanf()` trong phần đọc dòng `BILL` dùng `%[^|]` không có giới hạn size. Các buffer `nameKhach[50]`, `phoneKhach[15]`, `dateTime[50]`, `rankKhach[20]` đang bị rủi ro overflow.
- `sscanf()` trong phần đọc dòng `ITEM` cũng không giới hạn `name` và `note` độ dài.
- Dòng `addBillToHistory(currentCustomerId, &tempBill);` bị gọi **hai lần**, gây lưu trùng bill vào linked list.
- `sprintf(filename, "app/database/history_khach_%d.txt", customerId);` tốt hơn nên dùng `snprintf()` để an toàn.

#### Code sửa:
```c
// Trong loadHistoryFromFile - phần đọc BILL:
sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d|%19[^|]|%lf",
       &billId, &customerId, nameKhach, phoneKhach, dateTime,
       &total, &discount, &finalPrice, &itemCountInFile, rankKhach, &totalSpent);

// Phần đọc ITEM:
sscanf(line, "ITEM|%d|%49[^|]|%lf|%d|%d|%lf|%99[^\n]",
       &id, name, &price, &quantity, &option, &totalPrice, note);

// Chuẩn hóa note:
note[strcspn(note, "\n")] = 0;
strncpy(tempBill.items[itemCount].name, name, sizeof(tempBill.items[itemCount].name) - 1);
tempBill.items[itemCount].name[sizeof(tempBill.items[itemCount].name) - 1] = '\0';
strncpy(tempBill.items[itemCount].note, note, sizeof(tempBill.items[itemCount].note) - 1);
tempBill.items[itemCount].note[sizeof(tempBill.items[itemCount].note) - 1] = '\0';

// Xóa lần gọi trùng lặp:
if (addBillToHistory(currentCustomerId, &tempBill) != 0) {
    printf("[!] Canh bao: Khong the them bill vao lich su\n");
}
// Xóa dòng: addBillToHistory(currentCustomerId, &tempBill);

// Thay sprintf bằng snprintf:
snprintf(filename, sizeof(filename), "app/database/history_khach_%d.txt", customerId);
```

---

### 4. `app/services/report_service.c`
- `sscanf()` trong `calculateTotalRevenue()`, `calculateTotalItems()`, `analyzeByCustomer()` dùng `%[^|]` không giới hạn size cho `nameKhach`, `phone`, `dateTime`.

#### Code sửa:
```c
// Trong calculateTotalRevenue:
sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
       &billId, &customerId, nameKhach, phone, dateTime,
       &total, &discount, &finalPrice, &itemCount);

// Trong calculateTotalItems:
sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
       &billId, &customerId, nameKhach, phone, dateTime,
       &total, &discount, &finalPrice, &itemsInBill);

// Trong analyzeByCustomer:
sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
       &billId, &customerId, nameKhach, phone, dateTime,
       &total, &discount, &finalPrice, &itemCount);
```

---

### 5. `app/ui/history_ui.c`
- `sscanf()` đọc dòng `BILL` vẫn dùng `%[^|]` không có size giới hạn.

#### Code sửa:
```c
// Trong printAllHistory:
sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d|%19[^|]|%lf",
       &billId, &customerId, customerName, phone, dateTime,
       &total, &discount, &finalPrice, &itemCount, rank, &totalSpent);
```

---

### 6. `app/controllers/order_controller.c`
- `strcpy(currentBill->items[pos].name, menu[idx].name);` chưa dùng `strncpy()`.
- `strcpy(currentBill->items[pos].note, ghiChu);` chưa giới hạn độ dài note.

#### Code sửa:
```c
// Trong handleAddToCart:
strncpy(currentBill->items[pos].name, menu[idx].name, sizeof(currentBill->items[pos].name) - 1);
currentBill->items[pos].name[sizeof(currentBill->items[pos].name) - 1] = '\0';

strncpy(currentBill->items[pos].note, ghiChu, sizeof(currentBill->items[pos].note) - 1);
currentBill->items[pos].note[sizeof(currentBill->items[pos].note) - 1] = '\0';
```

---

### 7. `app/controllers/checkout_controller.c`
- Nhiều `strcpy()` vẫn chưa giới hạn kích thước.
- Reset sau thanh toán chỉ xóa một phần thông tin customer, chưa reset toàn bộ cấu trúc `currentBill`.

#### Code sửa:
```c
// Trong processCheckout:
strncpy(oldRank, khGoc->rank, sizeof(oldRank) - 1);
oldRank[sizeof(oldRank) - 1] = '\0';

strncpy(currentBill->customer.name, khGoc->name, sizeof(currentBill->customer.name) - 1);
currentBill->customer.name[sizeof(currentBill->customer.name) - 1] = '\0';

strncpy(currentBill->customer.phone, khGoc->phone, sizeof(currentBill->customer.phone) - 1);
currentBill->customer.phone[sizeof(currentBill->customer.phone) - 1] = '\0';

strncpy(currentBill->customer.rank, khGoc->rank, sizeof(currentBill->customer.rank) - 1);
currentBill->customer.rank[sizeof(currentBill->customer.rank) - 1] = '\0';

// Reset toàn bộ customer sau thanh toán:
memset(&currentBill->customer, 0, sizeof(currentBill->customer));
currentBill->itemCount = 0;
```

---

### 8. `app/services/checkout_service.c`
- Các rank string vẫn dùng hardcoded text.

#### Code sửa:
```c
// Thêm vào models.h:
#define RANK_BRONZE "Bronze"
#define RANK_SILVER "Silver"
#define RANK_GOLD "Gold"
#define RANK_DIAMOND "Diamond"

// Trong updateCustomerRank:
if (cust->totalSpent >= 10000000) {
    strcpy(cust->rank, RANK_DIAMOND);
} else if (cust->totalSpent >= 5000000) {
    strcpy(cust->rank, RANK_GOLD);
} else if (cust->totalSpent >= 1000000) {
    strcpy(cust->rank, RANK_SILVER);
} else {
    strcpy(cust->rank, RANK_BRONZE);
}
```

---

### 9. `app/ui/menu_ui.c`
- Dùng chỉ số cứng như `menu[12+i]`, `menu[20+i]`.

#### Code sửa:
```c
// Thêm vào models.h:
#define MENU_CATEGORY_MAIN_START 0
#define MENU_CATEGORY_SIDEDISH_START 12
#define MENU_CATEGORY_DRINK_START 20

// Trong printMenu:
sprintf(left, " %2d.%-20.20s %9.0lf (K:%-3d)", menu[MENU_CATEGORY_SIDEDISH_START + i].id, menu[MENU_CATEGORY_SIDEDISH_START + i].name, menu[MENU_CATEGORY_SIDEDISH_START + i].price, menu[MENU_CATEGORY_SIDEDISH_START + i].stock);

sprintf(right, " %2d.%-20.20s %9.0lf (K:%-3d)", menu[MENU_CATEGORY_DRINK_START + i].id, menu[MENU_CATEGORY_DRINK_START + i].name, menu[MENU_CATEGORY_DRINK_START + i].price, menu[MENU_CATEGORY_DRINK_START + i].stock);
```

---

## Tiếp theo
Bạn chỉ cần copy các đoạn code trên vào các file tương ứng để sửa lỗi. Sau đó chạy lại kiểm tra để đảm bảo code sạch hơn.

# Kiểm tra và Sửa lỗi PBL1 - Code Review Chi Tiết

## Tổng quan
Dự án PBL1 có **21 lỗi** cần sửa, từ **Critical** đến **Minor**. Tài liệu này liệt kê chi tiết từng lỗi, nguyên nhân, tác động và cách sửa.

---

## 🔴 CÁC LỖI CRITICAL (PHẢI SỬA NGAY)

### 1. Memory Leak: Double-Free Trong Load Khách Hàng
**File**: `app/services/file_service.c`  
**Dòng**: 70-90  
**Vấn đề**: 
```c
Customer* c = (Customer*)malloc(sizeof(Customer));
// ... populate c ...
danhSachKH[soLuongKH] = *c;  // Copy vào array
insertBTree(root, &danhSachKH[soLuongKH]);  // B-Tree giữ con trỏ
soLuongKH++;
free(c);  // ❌ LỖIO: c bị free, nhưng B-Tree vẫn giữ con trỏ
```
**Nguyên nhân**: B-Tree lưu con trỏ đến khách hàng, sau đó con trỏ tạm được free. Tạo ra dangling pointer.  
**Tác động**: **CRASH** khi truy cập B-Tree sau này.  
**Cách sửa**: Bỏ `free(c)` - bộ nhớ đã được quản lý bởi array global `danhSachKH`.
```c
// ✓ Sửa: Xóa dòng free(c)
Customer* c = (Customer*)malloc(sizeof(Customer));
// ... populate c ...
danhSachKH[soLuongKH] = *c;
insertBTree(root, &danhSachKH[soLuongKH]);
soLuongKH++;
// ❌ Xóa: free(c);  <-- XÓA DÒNG NÀY
```

---

### 2. Buffer Overflow Trong Input Số Điện Thoại
**File**: `app/utils/validator.c`  
**Dòng**: 73  
**Vấn đề**: 
```c
void getValidPhoneNumber(char* phone, int maxLen, const char* prompt) {
    while (1) {
        printf("%s", prompt);
        scanf("%s", phone);  // ❌ Không có giới hạn size - bị tràn bộ đệm!
```
**Nguyên nhân**: `scanf("%s")` bỏ qua tham số `maxLen`, cho phép nhập vượt quá dung lượng buffer.  
**Tác động**: **CRASH** hoặc **INJECTION ATTACK** khi user nhập số điện thoại dài.  
**Cách sửa**: 
```c
void getValidPhoneNumber(char* phone, int maxLen, const char* prompt) {
    while (1) {
        printf("%s", prompt);
        scanf("%14s", phone);  // ✓ Giới hạn: maxLen - 1 (cho '\0')
        // hoặc: scanf("%*s", maxLen-1, phone);
```

---

### 3. Use-After-Free Trong Đăng Ký Khách Hàng
**File**: `app/controllers/customer_controller.c`  
**Dòng**: 33-50  
**Vấn đề**: 
```c
if (choice == 1) {
    // ... populate newCust ...
    insertBTree(&btreeRoot, &danhSachKH[soLuongKH]);  // Line 48
    currentBill->customer = danhSachKH[soLuongKH];   // Line 49
    soLuongKH++;  // Line 50 - tăng SAU khi dùng!
}
```
**Nguyên nhân**: Sử dụng index array trước khi `soLuongKH` được tăng. Nếu array bị reallocate, con trỏ trở thành invalid.  
**Tác động**: **CRASH** hoặc dữ liệu bị hỏng khi array tăng kích thước.  
**Cách sửa**: 
```c
if (choice == 1) {
    // ... populate newCust ...
    soLuongKH++;  // ✓ Tăng TRƯỚC khi dùng index
    insertBTree(&btreeRoot, &danhSachKH[soLuongKH - 1]);
    currentBill->customer = danhSachKH[soLuongKH - 1];
}
```

---

### 4. Buffer Overflow Trong sscanf Khi Đọc Dữ Liệu
**File**: `app/services/history_service.c`  
**Dòng**: 159 (và các chỗ khác)  
**Vấn đề**: 
```c
sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|...",
        &billId, &customerId, nameKhach,  // ❌ Không giới hạn size!
        phoneKhach, dateTime,              // ❌ Không giới hạn size!
```
**Nguyên nhân**: Format specifier `%[^|]` không có giới hạn kích thước trong `sscanf`.  
**Tác động**: **CRASH** khi dữ liệu trong file dài hơn buffer.  
**Cách sửa**: 
```c
// ❌ Sai: %[^|]
// ✓ Đúng: %49[^|] (cho char[50])
sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|...",
        &billId, &customerId, nameKhach,  // 49 chars max
        phoneKhach, dateTime,
```
**Chỗ cần sửa**:
- `app/services/history_service.c` - dòng 124, 159
- `app/services/report_service.c` - dòng 65
- Tất cả sscanf với `%[^|]` pattern

---

### 5. Thiếu Kiểm Tra File Handle
**File**: `app/services/file_service.c`  
**Dòng**: 95-105 (hàm saveAllCustomersToFile)  
**Vấn đề**: 
```c
void saveAllCustomersToFile() {
    FILE *f = fopen("app/database/khachhang.txt", "w");
    // ❌ Không kiểm tra fopen() thất bại
    for (int i = 0; i < soLuongKH; i++) {
        fprintf(f, ...);  // Crash nếu f == NULL!
    }
    fclose(f);
}
```
**Nguyên nhân**: Không xử lý trường hợp `fopen()` trả về NULL.  
**Tác động**: **CRASH** khi file không thể mở (permissions, path, etc).  
**Cách sửa**: 
```c
void saveAllCustomersToFile() {
    FILE *f = fopen("app/database/khachhang.txt", "w");
    if (f == NULL) {  // ✓ Kiểm tra lỗi
        printf("[!] Loi: Khong the mo file khachhang.txt de ghi\n");
        return;
    }
    for (int i = 0; i < soLuongKH; i++) {
        fprintf(f, ...);
    }
    fclose(f);
}
```

---

### 6. Memory Leak: Linked List Không Được Giải Phóng
**File**: `app/services/history_service.c`  
**Dòng**: 252 (hàm getCustomerHistory)  
**Vấn đề**: 
```c
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* result = NULL;
    // ... 
    while (current != NULL) {
        if (current->customerId == customerId) {
            HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));  // ❌ Allocate
            *newNode = *current;
            newNode->next = result;
            result = newNode;  // ❌ Trả về nhưng không bao giờ được free!
        }
        current = current->next;
    }
    return result;
}
```
**Nguyên nhân**: Hàm allocate nodes nhưng caller không biết rằng phải free chúng.  
**Tác động**: **MEMORY LEAK** - bộ nhớ tích lũy theo thời gian.  
**Cách sửa** (cách 1 - thêm hàm free):
```c
// ✓ Thêm hàm để giải phóng
void freeHistoryList(HistoryNode* head) {
    while (head != NULL) {
        HistoryNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// ✓ Cập nhật lại history_ui.c - printCustomerHistory
// Sau khi in xong, gọi: freeHistoryList(customerHistory);
```

Hoặc (cách 2 - không allocate, trả về global list):
```c
// ✓ Trả về con trỏ tới global historyHead (không allocate)
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* current = historyHead;
    while (current != NULL && current->customerId != customerId) {
        current = current->next;
    }
    return current;  // Chỉ trả về con trỏ, không allocate
}
```

---

## 🟠 CÁC LỖI IMPORTANT (PHẢI SỬA)

### 7. Đạo Hàm Null Pointer Sau B-Tree Search
**File**: `app/controllers/checkout_controller.c`  
**Dòng**: 32-45, 87-93  
**Vấn đề**: 
```c
Customer* khGoc = searchBTree(btreeRoot, currentBill->customer.phone);
// ... nhiều dòng code ...
if (khGoc != NULL) {  // Kiểm tra muộn!
    khGoc->totalSpent += finalPrice;  // Nếu phone trống, khGoc == NULL!
}
```
**Nguyên nhân**: Kiểm tra `khGoc != NULL` xảy ra quá muộn, sau khi đã tính toán.  
**Tác động**: Logic sai khi customer.phone trống (khách vãng lai).  
**Cách sửa**: 
```c
// ✓ Kiểm tra ngay sau search
Customer* khGoc = NULL;
if (strlen(currentBill->customer.phone) > 0) {
    khGoc = searchBTree(btreeRoot, currentBill->customer.phone);
}
// Sau đó mới tính toán
if (khGoc != NULL) {
    discountRate = getDiscountPercent(khGoc->rank);
}
```

---

### 8. Mismatch Rank System
**File**: `app/controllers/customer_controller.c` vs `app/services/checkout_service.c`  
**Dòng**: 43 vs 15-25  
**Vấn đề**: 
```c
// customer_controller.c
strcpy(newCust.rank, "Bronze");  // ✓

// checkout_service.c
if (strcmp(rank, "Diamond") == 0) return 0.30;
if (strcmp(rank, "Gold") == 0) return 0.20;
if (strcmp(rank, "Silver") == 0) return 0.10;
// "Bronze" không trong list - sẽ trả về 0.0
```
**Nguyên nhân**: Hệ thống rank không nhất quán, thiếu comment về logic.  
**Tác động**: Logic hoạt động nhưng nhầm lẫn, khó bảo trì.  
**Cách sửa**: Sử dụng constants:
```c
// ✓ Thêm vào models.h
#define RANK_BRONZE "Bronze"
#define RANK_SILVER "Silver"
#define RANK_GOLD "Gold"
#define RANK_DIAMOND "Diamond"

// ✓ Cập nhật checkout_service.c
double getDiscountPercent(char* rank) {
    if (strcmp(rank, RANK_DIAMOND) == 0) return 0.30;
    if (strcmp(rank, RANK_GOLD) == 0) return 0.20;
    if (strcmp(rank, RANK_SILVER) == 0) return 0.10;
    return 0.0;  // Bronze hoặc khách mới
}
```

---

### 9. Kiểm Tra Bounds Array Không Đầy Đủ
**File**: `app/controllers/order_controller.c`  
**Dòng**: 28-50  
**Vấn đề**: 
```c
int idx = idNhap - 1;  // Line 41
// Không kiểm tra xem idx có hợp lệ trong [0, TONG_SO_MON)
if (idNhap <= 12) {  // Magic number!
    // ...
}
// ...
if (slNhap <= 0 || slNhap > menu[idx].stock) {  
    // ❌ menu[idx] truy cập mà chưa chắc idx hợp lệ!
}
```
**Nguyên nhân**: Kiểm tra điều kiện ở các chỗ khác nhau, không tập trung.  
**Tác động**: Crash nếu user nhập ID ngoài range.  
**Cách sửa**: 
```c
// ✓ Kiểm tra bounds ngay ở đầu
int idx = idNhap - 1;
if (idx < 0 || idx >= TONG_SO_MON) {
    printf("[!] ID mon khong hop le (1-%d)\n", TONG_SO_MON);
    return;
}
// Sau đó mới dùng idx an toàn
if (slNhap <= 0 || slNhap > menu[idx].stock) {
    printf("[!] So luong khong hop le\n");
    return;
}
```

---

### 10. Resource Leak Khi malloc Thất Bại
**File**: `app/services/history_service.c`  
**Dòng**: 180-210  
**Vấn đề**: 
```c
void loadHistoryFromFile() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) return;  // OK
    
    // ... 60+ lines ...
    
    if (strcmp(line, "END") == 0 && billStarted) {
        addBillToHistory(currentCustomerId, &tempBill);  // ❌ Có thể malloc fail
        // Nếu fail, không có error handling
    }
    fclose(file);  // Chỉ được gọi nếu hoàn thành
}
```
**Nguyên nhân**: Không xử lý lỗi malloc trong `addBillToHistory`.  
**Tác động**: Memory leak hoặc crash nếu hệ thống thiếu bộ nhớ.  
**Cách sửa**: 
```c
// ✓ Thêm kiểm tra lỗi
int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) {  // ✓ Kiểm tra lỗi
        printf("[!] Loi: Khong du bo nho\n");
        return -1;  // Trả về error code
    }
    newNode->customerId = customerId;
    newNode->bill = *bill;
    newNode->next = historyHead;
    historyHead = newNode;
    return 0;  // Success
}

// ✓ Cập nhật loadHistoryFromFile
if (strcmp(line, "END") == 0 && billStarted) {
    if (addBillToHistory(currentCustomerId, &tempBill) != 0) {
        printf("[!] Canh bao: Khong the them bill vao lich su\n");
        // Có thể continue hoặc break tùy logic
    }
}
```

---

## 🟡 CÁC LỖI MINOR (NÊN SỬA ĐỂ CLEAN CODE)

### 11. Magic Numbers Không Có Hằng Số
**File**: `app/controllers/order_controller.c`, `app/ui/menu_ui.c`  
**Dòng**: 35, 38, 27-31  
**Vấn đề**: 
```c
if (idNhap <= 12) {  // Magic number 12 - Đó là cái gì?
    countMain++;
}
// ...
for (int i = 0; i < 8; i++) {  // Magic 8
    sprintf(mid, " %2d.%-20.20s", menu[12+i].id, ...);  // Magic 12
    sprintf(right, " %2d.%-20.20s", menu[20+i].id, ...);  // Magic 20
}
```
**Nguyên nhân**: Hardcode số thay vì dùng hằng số.  
**Tác động**: Khó bảo trì, dễ nhầm lẫn nếu thay đổi menu structure.  
**Cách sửa**: Thêm vào `models.h`:
```c
#define MAIN_DISH_COUNT 12       // Số công thức chính
#define SIDE_DISH_COUNT 8        // Số công thức phụ
#define DRINK_COUNT 5            // Số thức uống
#define MAIN_DISH_START 0        // Chỉ số bắt đầu
#define SIDE_DISH_START 12       // Chỉ số bắt đầu
#define DRINK_START 20           // Chỉ số bắt đầu

// ✓ Sau đó cập nhật code
if (idNhap <= MAIN_DISH_COUNT) {
    countMain++;
}
// ...
sprintf(mid, " %2d.%-20.20s", menu[SIDE_DISH_START+i].id, ...);
sprintf(right, " %2d.%-20.20s", menu[DRINK_START+i].id, ...);
```

---

### 12. Thiếu Comment Trên Logic Phức Tạp
**File**: `app/services/btree_service.c`  
**Dòng**: 35-55  
**Vấn đề**: B-Tree split logic không có comment.  
**Cách sửa**: Thêm comments chi tiết:
```c
// Hàm tách node B-Tree khi đầy
// Điều kiện: node phải có BTREE_ORDER - 1 keys (đầy)
// Kết quả: Node được tách thành 2, key giữa đẩy lên parent
void splitNode(BTreeNode* parent, int childIndex) {
    BTreeNode* fullChild = parent->children[childIndex];
    BTreeNode* newRight = createNode(fullChild->isLeaf);
    
    // Copy nửa phải của keys và children sang newRight
    // ...
}
```

---

### 13. Kiểm Tra Input Không Rõ Ràng
**File**: `app/utils/validator.c`  
**Dòng**: 20 (hàm getValidInt)  
**Vấn đề**: 
```c
printf("  !! Loi: Vui long nhap so hop le! \n");  // Quá chung chung
```
**Cách sửa**: 
```c
// ✓ Thêm context
printf("  !! Loi: Vui long nhap so hop le (0-999999)! \n");
// hoặc truyền range vào function
void getValidIntInRange(int min, int max, const char* prompt) {
    // ...
    printf("  !! Loi: Nhap so trong khoang [%d, %d]\n", min, max);
}
```

---

### 14. Kiểm Tra Toàn Vẹn Dữ Liệu Khi Load Từ File
**File**: `app/services/history_service.c`, `app/services/file_service.c`  
**Vấn đề**: Khi load từ file, không kiểm tra xem dữ liệu có hợp lệ không.  
**Cách sửa**: Thêm validation:
```c
// ✓ Kiểm tra khi load customer
if (soLuongKH >= 1000) {  // Max capacity
    printf("[!] Canh bao: Vuot qua so luong khach hang toi da\n");
    break;
}
if (strlen(nameKhach) == 0 || strlen(phoneKhach) == 0) {
    printf("[!] Loi: Khach hang khong hop le, bo qua\n");
    continue;
}
```

---

### 15. Inconsistent Struct Initialization
**File**: `main.c`, `app/controllers/checkout_controller.c`  
**Vấn đề**: Một số chỗ dùng `{0}`, một số không.  
**Cách sửa**: Luôn dùng `{0}`:
```c
// ✓ Luôn dùng
Bill currentBill = {0};
Customer tempCust = {0};
// Hoặc cải thiện hơn:
Customer tempCust = {
    .id = -1,           // -1 = khách mới (không registered)
    .totalSpent = 0,
    .rank = "Bronze"
};
```

---

## 📋 BẢNG TỔNG HỢP

| # | Vấn đề | File | Loại | Độ ưu tiên |
|---|--------|------|------|-----------|
| 1 | Double-Free Memory | file_service.c | Memory | 🔴 Critical |
| 2 | Buffer Overflow scanf | validator.c | Security | 🔴 Critical |
| 3 | Use-After-Free Array | customer_controller.c | Memory | 🔴 Critical |
| 4 | Buffer Overflow sscanf | history_service.c | Memory | 🔴 Critical |
| 5 | Thiếu NULL check fopen | file_service.c | Reliability | 🔴 Critical |
| 6 | Memory Leak LinkedList | history_service.c | Memory | 🔴 Critical |
| 7 | Null Pointer Dereference | checkout_controller.c | Logic | 🟠 Important |
| 8 | Rank System Mismatch | checkout_service.c | Logic | 🟠 Important |
| 9 | Array Bounds Check | order_controller.c | Security | 🟠 Important |
| 10 | Resource Leak malloc | history_service.c | Reliability | 🟠 Important |
| 11 | Magic Numbers | order_controller.c | CodeQuality | 🟡 Minor |
| 12 | Missing Comments | btree_service.c | Maintenance | 🟡 Minor |
| 13 | Unclear Error Msg | validator.c | UX | 🟡 Minor |
| 14 | No Data Validation | file_service.c | Reliability | 🟡 Minor |
| 15 | Inconsistent Init | checkout_controller.c | CodeQuality | 🟡 Minor |

---

## ✅ HƯỚNG DẪN SỬA CHỮA TỪNG BƯỚC

### Phase 1: Fix Critical Issues (2-3 tiếng)
1. Xóa `free(c)` trong file_service.c
2. Fix buffer overflow ở validator.c - thêm giới hạn size
3. Fix format specifier sscanf - thêm %49[^|], %14[^|]
4. Thêm NULL check cho fopen() - tất cả hàm file

### Phase 2: Fix Important Issues (1-2 tiếng)
5. Thêm hàm freeHistoryList() và gọi ở đúng chỗ
6. Fix soLuongKH++ positioning
7. Thêm validation B-Tree search
8. Fix bounds checking array

### Phase 3: Code Quality (1 tiếng)
9. Thêm constants cho magic numbers
10. Thêm comments
11. Cải thiện error messages
12. Standardize struct initialization

### Phase 4: Test & Verify (2-3 tiếng)
- Compile lại, fix all warnings
- Test crash scenarios
- Valgrind check memory leaks
- Test edge cases

---

## 🚀 KHI HOÀN THÀNH

- Tất cả code phải compile **không có warning**
- Chạy `valgrind` không có memory leak
- Xử lý được tất cả invalid input
- Code có comment rõ ràng
- Cấu trúc dữ liệu nhất quán


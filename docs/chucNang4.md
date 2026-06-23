# CHỨC NĂNG 4: THANH TOÁN

## I. TỔNG QUÁT

**Tên chức năng:** Thanh Toán  
**Lựa chọn:** Case 4 trong menu chính  
**Mô tả:** Xử lý thanh toán hóa đơn, áp dụng giảm giá theo hạng VIP, cập nhật thông tin khách hàng, lưu lịch sử bán hàng, và dọn dẹp giỏ hàng.

---

## II. QUY TRÌNH THANH TOÁN

```
Kiểm tra Giỏ Rỗng?
│
├─ YES → Báo lỗi & Thoát
│
└─ NO → Tiếp tục
    │
    ├─ Tính Tổng Tiền
    │
    ├─ Tìm Khách Hàng
    │
    ├─ Áp Dụng Giảm Giá
    │
    ├─ Hiển Thị Chi Tiết
    │
    ├─ Nhập Tiền Khách
    │
    ├─ In Hóa Đơn
    │
    ├─ Xác Nhận?
    │  ├─ YES:
    │  │  ├─ Cập nhật Khách Hàng
    │  │  ├─ Nâng Hạng (nếu có)
    │  │  ├─ Lưu Khách Hàng
    │  │  ├─ Lưu Hóa Đơn Chung
    │  │  ├─ Lưu Lịch Sử Khách
    │  │  └─ Dọn Giỏ
    │  │
    │  └─ NO:
    │     └─ Hủy & Quay lại
```

---

## III. CẤU TRÚC DỮ LIỆU CHÍNH

### Bill (Hóa đơn)
```c
typedef struct {
    int id;              // ID hóa đơn
    Customer customer;   // Thông tin khách hàng
    Cart cart;           // Giỏ hàng
    double total;        // Tổng tiền trước giảm
    double discount;     // Tiền giảm giá
    double finalPrice;   // Tổng tiền sau giảm
    char dateTime[50];   // Ngày giờ
} Bill;
```

### Customer (Khách hàng - trong Bill)
```c
typedef struct {
    int id;              // ID khách (0 = Guest)
    char phone[15];
    char name[50];
    char address[100];
    double totalSpent;   // Cộng dồn chi tiêu
    char rank[20];       // Hạng VIP
} Customer;
```

### Hạng VIP & Giảm Giá
```
Bronze:   < 1.000.000 VND       → 0%
Silver:   1.000.000 - 5.000.000 → 5%
Gold:     5.000.000 - 10.000.000 → 10%
Diamond:  ≥ 10.000.000 VND      → 15%
```

---

## IV. HÀM CHÍNH

### `processCheckout(Bill* currentBill)` - Controller

**Vị trí:** `app/controllers/checkout_controller.c`  
**Mô tả:** Xử lý toàn bộ quy trình thanh toán

---

## V. HÀM PHỤ TRỢ

### 1. `calculateSubtotal(Bill* bill)` - Service

**Vị trí:** `app/services/checkout_service.c`  
**Mô tả:** Tính tổng tiền trước khi giảm giá

**Pseudocode:**
```c
double calculateSubtotal(Bill* bill) {
    double total = 0.0;
    CartNode* node = bill->cart.head;
    
    // Duyệt tất cả item trong giỏ
    while(node) {
        total += node->item.totalPrice;  // price * quantity
        node = node->next;
    }
    return total;
}
```

**Độ phức tạp:** O(k) - k là số mục trong giỏ

### 2. `getDiscountPercent(char* rank)` - Service

**Vị trí:** `app/services/checkout_service.c`  
**Mô tả:** Trả về tỉ lệ giảm giá theo hạng

**Pseudocode:**
```c
double getDiscountPercent(char* rank) {
    if (strcmp(rank, "Diamond") == 0) return 0.15;  // 15%
    if (strcmp(rank, "Gold") == 0)    return 0.10;  // 10%
    if (strcmp(rank, "Silver") == 0)  return 0.05;  // 5%
    if (strcmp(rank, "Bronze") == 0)  return 0.0;   // 0%
    return 0.0;
}
```

### 3. `updateCustomerRank(Customer* cust)` - Service

**Vị trí:** `app/services/checkout_service.c`  
**Mô tả:** Cập nhật hạng VIP dựa trên tổng chi tiêu

**Pseudocode:**
```c
void updateCustomerRank(Customer* cust) {
    if (cust->totalSpent >= 10000000) 
        strcpy(cust->rank, "Diamond");
    else if (cust->totalSpent >= 5000000) 
        strcpy(cust->rank, "Gold");
    else if (cust->totalSpent >= 1000000) 
        strcpy(cust->rank, "Silver");
    else 
        strcpy(cust->rank, "Bronze");
}
```

### 4. `getCurrentDateTime(char* buffer, size_t size)` - Utility

**Vị trí:** `app/utils/helper.c`  
**Mô tả:** Lấy ngày giờ hiện tại

**Format:** `YYYY-MM-DD HH:MM:SS`

### 5. `searchBTree(BTreeNode* root, char* phone)` - Service

**Vị trí:** `app/services/btree_service.c`  
**Mô tả:** Tìm khách hàng trong B-Tree để áp dụng giảm giá

**Độ phức tạp:** O(log n)

### 6. `printBillUI(Bill* currentBill, double moneyPaid, double change)` - UI

**Vị trị:** `app/ui/bill_ui.c`  
**Mô tả:** In chi tiết hóa đơn

**In ra:**
```
==================== HOA DON ====================
ID: 123456
Khách Hàng: Nguyen Van A
Số Điện Thoại: 0987654321
Ngày Giờ: 2024-01-15 10:30:45

STT | Tên Món        | Giá    | SL | Ghi Chú    | Tổng
----|----------------|--------|----|-----------|---------
1   | Sườn Nướng (Su)| 45000  | 2  |           | 90000
2   | Canh Chua      | 8000   | 1  |           | 8000
3   | Cơm            | 5000   | 2  |           | 10000
----|----------------|--------|----|-----------|---------
    | TONG TIEN BAN DAU     :        108.000 VND
    | HANG KHACH HANG      : GOLD (Giam 10%)
    | SO TIEN DUOC GIAM    :        10.800 VND
    | TONG TIEN CAN TRA    :        97.200 VND
    | KHACH DUA            :       100.000 VND
    | TIEN THUA            :        2.800 VND
=================================================
```

### 7. `getValidDouble(const char* prompt)` - Utility

**Vị trí:** `app/utils/validator.c`  
**Mô tả:** Xác thực và nhập số thực

**Xác thực:**
- Phải là số
- Phải >= 0
- Nhập lại nếu không hợp lệ

### 8. `saveAllCustomersToFile()` - Service

**Vị trị:** `app/services/file_service.c`  
**Mô tả:** Lưu tất cả khách hàng vào file

**Mục đích:** Persist thông tin khách hàng được cập nhật

### 9. `saveBillToGlobalFile(Bill* bill, int customerId)` - Service

**Vị trí:** `app/services/file_service.c`  
**Mô tả:** Lưu hóa đơn vào file lịch sử chung

**Format:** `app/database/history.txt`
```
BILL|ID|CustomerID|Name|Phone|DateTime|Total|Discount|FinalPrice|ItemCount
ITEM|ItemID|ItemName|Price|Qty|Option|Note|TotalPrice
ITEM|...
```

**Ví dụ:**
```
BILL|123456|1|Nguyen Van A|0987654321|2024-01-15 10:30|150000|15000|135000|3
ITEM|1|Sườn Nướng|40000|1|2|Không cay|40000
ITEM|5|Cơm|5000|2|0||10000
ITEM|20|Nước Ngọt|12000|1|0||12000
```

### 10. `saveBillToHistoryFile(int customerId, Bill* bill)` - Service

**Vị trí:** `app/services/file_service.c`  
**Mô tả:** Lưu hóa đơn vào file riêng của khách

**File:** `app/database/history_khach_X.txt`

### 11. `addBillToHistory(int customerId, Bill* bill)` - Service

**Vị trí:** `app/services/history_service.c`  
**Mô tả:** Thêm hóa đơn vào linked list lịch sử (bộ nhớ)

**Pseudocode:**
```c
int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    newNode->customerId = customerId;
    newNode->bill = *bill;
    newNode->next = NULL;
    
    // Thêm vào cuối linked list
    if (historyHead == NULL) {
        historyHead = newNode;
    } else {
        HistoryNode* temp = historyHead;
        while (temp->next != NULL) 
            temp = temp->next;
        temp->next = newNode;
    }
    return 1;
}
```

---

## VI. LUỒNG HOẠT ĐỘNG CHI TIẾT

```
processCheckout()
│
├─→ 1. KIỂM TRA GIỎ:
│   ├─ IF cart->itemCount == 0:
│   │  ├─ printf("[!] Gio hang cua ban dang trong!")
│   │  └─ RETURN (Thoát)
│   └─ CONTINUE
│
├─→ 2. TÍNH TOÁN:
│   ├─ subtotal = calculateSubtotal(currentBill)
│   │
│   ├─ Tìm Khách Cũ:
│   │  ├─ IF strlen(currentBill->customer.phone) > 0:
│   │  │  └─ khGoc = searchBTree(btreeRoot, phone)
│   │  │
│   │  └─ ELSE:
│   │     └─ khGoc = NULL (Guest)
│   │
│   ├─ Áp Dụng Giảm Giá:
│   │  ├─ IF khGoc != NULL:
│   │  │  ├─ discountRate = getDiscountPercent(khGoc->rank)
│   │  │  ├─ discountAmount = subtotal * discountRate
│   │  │  └─ finalPrice = subtotal - discountAmount
│   │  │
│   │  └─ ELSE:
│   │     ├─ discountRate = 0
│   │     ├─ discountAmount = 0
│   │     └─ finalPrice = subtotal
│   │
│   └─ Cập Nhật Bill:
│      ├─ currentBill->total = subtotal
│      ├─ currentBill->discount = discountAmount
│      ├─ currentBill->finalPrice = finalPrice
│      ├─ currentBill->id = rand() % 900000 + 100000  // ID 6 chữ số
│      └─ getCurrentDateTime(currentBill->dateTime)
│
├─→ 3. HIỂN THỊ CHI TIẾT:
│   ├─ printf("TONG TIEN BAN DAU: %.3f VND", subtotal)
│   │
│   ├─ IF khGoc != NULL:
│   │  ├─ printf("RANK: %s (Giam %.0f%%)", rank, discountRate*100)
│   │  └─ printf("SO TIEN DUOC GIAM: %.3f VND", discountAmount)
│   │
│   └─ printf("TONG TIEN CAN TRA: %.3f VND", finalPrice)
│
├─→ 4. NHẬP TIỀN:
│   ├─ customerMoney = getValidDouble("Nhap so tien khach dua: ")
│   │
│   └─ WHILE customerMoney < finalPrice:
│      └─ Báo lỗi & nhập lại
│
├─→ 5. IN HÓA ĐƠN:
│   └─ printBillUI(currentBill, customerMoney, change)
│
├─→ 6. XÁC NHẬN:
│   └─ xacNhan = getValidInt("Xac nhan (1: Dong y / 0: Huy bo): ")
│
├─→ 7A. NẾU XÁC NHẬN (xacNhan == 1):
│   │
│   ├─ IF khGoc != NULL (Khách Cũ):
│   │  │
│   │  ├─ Lưu Thông Tin Cũ:
│   │  │  ├─ oldTotal = khGoc->totalSpent
│   │  │  └─ strcpy(oldRank, khGoc->rank)
│   │  │
│   │  ├─ Cập Nhật Khách Hàng:
│   │  │  ├─ khGoc->totalSpent += finalPrice
│   │  │  └─ updateCustomerRank(khGoc)
│   │  │
│   │  ├─ Cập Nhật Bill:
│   │  │  ├─ currentBill->customer.id = khGoc->id
│   │  │  ├─ strcpy(name, phone, rank, ...)
│   │  │  └─ totalSpent = khGoc->totalSpent
│   │  │
│   │  ├─ Lưu Khách Hàng:
│   │  │  └─ saveAllCustomersToFile()
│   │  │
│   │  └─ Hiển Thị Cập Nhật:
│   │     ├─ printf("Tong chi tieu cu: %.3f VND", oldTotal)
│   │     ├─ printf("Tong chi tieu moi: %.3f VND", newTotal)
│   │     ├─ printf("Hang hien tai: %s", newRank)
│   │     │
│   │     └─ IF strcmp(oldRank, newRank) != 0:
│   │        └─ printf("CHUC MUNG! Da duoc nang hang tu %s len %s!", oldRank, newRank)
│   │
│   ├─ ELSE (Khách Mới/Guest):
│   │  ├─ currentBill->customer.id = 0
│   │  ├─ strcpy(rank, "Guest")
│   │  └─ totalSpent = finalPrice
│   │
│   ├─ Lưu Hóa Đơn Chung:
│   │  └─ saveBillToGlobalFile(currentBill, customerId)
│   │
│   ├─ Lưu Lịch Sử Khách (Nếu Registered):
│   │  ├─ IF khGoc != NULL AND strlen(phone) > 0:
│   │  │  ├─ addBillToHistory(customerId, currentBill)
│   │  │  └─ saveBillToHistoryFile(customerId, currentBill)
│   │  └─ ELSE:
│   │     └─ Không lưu (Guest)
│   │
│   ├─ Hiển Thị Thành Công:
│   │  └─ printf("[OK] Thanh toan thanh cong!")
│   │
│   └─ Dọn Dẹp Giỏ:
│      ├─ currentBill->cart.head = NULL
│      ├─ currentBill->cart.tail = NULL
│      ├─ currentBill->cart.itemCount = 0
│      ├─ memset(customer.phone, 0, sizeof)
│      └─ memset(customer.name, 0, sizeof)
│
└─→ 7B. NẾU HỦY (xacNhan != 1):
    └─ printf("[!] Da huy thanh toan.")
       └─ Quay lại menu (Giỏ KHÔNG bị dọn)
```

---

## VII. PSEUDOCODE ĐẦY ĐỦ

```c
void processCheckout(Bill* currentBill) {
    printf("\n=== THANH TOAN ===\n");
    
    // 1. Kiểm tra giỏ rỗng
    if (currentBill->cart.itemCount == 0) {
        printf("[!] Gio hang dang trong!\n");
        return;
    }
    
    // 2. Tính toán
    double subtotal = calculateSubtotal(currentBill);
    double discountRate = 0;
    Customer* khGoc = NULL;
    
    // Tìm khách cũ
    if (strlen(currentBill->customer.phone) > 0) {
        khGoc = searchBTree(btreeRoot, currentBill->customer.phone);
        if (khGoc != NULL) {
            discountRate = getDiscountPercent(khGoc->rank);
        }
    }
    
    double discountAmount = subtotal * discountRate;
    double finalPrice = subtotal - discountAmount;
    
    currentBill->total = subtotal;
    currentBill->discount = discountAmount;
    currentBill->finalPrice = finalPrice;
    getCurrentDateTime(currentBill->dateTime, sizeof(currentBill->dateTime));
    currentBill->id = rand() % 900000 + 100000;
    
    // 3. Hiển thị chi tiết
    printf(">> TONG TIEN BAN DAU: %.3f VND\n", subtotal);
    
    if (khGoc != NULL) {
        printf(">> RANK: %s (Giam %.0f%%)\n", khGoc->rank, discountRate * 100);
        printf(">> SO TIEN DUOC GIAM: %.3f VND\n", discountAmount);
    } else {
        printf(">> RANK: Guest (Khong giam gia)\n");
    }
    
    printf(">> TONG TIEN CAN TRA: %.3f VND\n", finalPrice);
    
    // 4. Nhập tiền
    double customerMoney = 0;
    customerMoney = getValidDouble("-> Nhap so tien khach dua: ");
    
    while (customerMoney < finalPrice) {
        printf("[!] Khach dua thieu %.3f VND!\n", finalPrice - customerMoney);
        customerMoney = getValidDouble("-> Nhap lai: ");
    }
    
    // 5. In hóa đơn
    printBillUI(currentBill, customerMoney, customerMoney - finalPrice);
    
    // 6. Xác nhận
    int xacNhan = getValidInt("Xac nhan (1: Dong y / 0: Huy bo): ");
    
    if (xacNhan == 1) {
        // 7A. Xác nhận thanh toán
        
        if (khGoc != NULL) {
            // Lưu thông tin cũ
            double oldTotal = khGoc->totalSpent;
            char oldRank[20];
            strcpy(oldRank, khGoc->rank);
            
            // Cập nhật
            khGoc->totalSpent += finalPrice;
            updateCustomerRank(khGoc);
            
            // Gán vào bill
            currentBill->customer.id = khGoc->id;
            strcpy(currentBill->customer.name, khGoc->name);
            strcpy(currentBill->customer.phone, khGoc->phone);
            strcpy(currentBill->customer.rank, khGoc->rank);
            currentBill->customer.totalSpent = khGoc->totalSpent;
            
            // Lưu file
            saveAllCustomersToFile();
            
            // Hiển thị cập nhật
            printf("\n[CAP NHAT KHACH HANG]\n");
            printf(">> Tong chi tieu cu: %.3f VND\n", oldTotal);
            printf(">> Tong chi tieu moi: %.3f VND\n", khGoc->totalSpent);
            printf(">> Hang hien tai: %s\n", khGoc->rank);
            
            if (strcmp(oldRank, khGoc->rank) != 0) {
                printf("\n[CHUC MUNG] Nang hang tu %s len %s!\n", oldRank, khGoc->rank);
            }
        } else {
            currentBill->customer.id = 0;
            strcpy(currentBill->customer.rank, "Guest");
            currentBill->customer.totalSpent = finalPrice;
        }
        
        // Lưu hóa đơn
        saveBillToGlobalFile(currentBill, currentBill->customer.id);
        
        // Lưu lịch sử riêng (nếu khách đã đăng ký)
        if (khGoc != NULL && strlen(currentBill->customer.phone) > 0) {
            addBillToHistory(currentBill->customer.id, currentBill);
            saveBillToHistoryFile(currentBill->customer.id, currentBill);
        }
        
        printf("\n[OK] Thanh toan thanh cong!\n");
        
        // Dọn dẹp giỏ
        currentBill->cart.head = NULL;
        currentBill->cart.tail = NULL;
        currentBill->cart.itemCount = 0;
        memset(currentBill->customer.phone, 0, sizeof(currentBill->customer.phone));
        memset(currentBill->customer.name, 0, sizeof(currentBill->customer.name));
    } 
    else {
        // 7B. Hủy thanh toán
        printf("\n[!] Da huy thanh toan.\n");
    }
}
```

---

## VIII. VÍ DỤ THANH TOÁN

### Trường Hợp 1: Khách Cũ (Nâng Hạng)
```
Tổng tiền ban đầu: 5.000.000 VND
Khách hàng: Nguyen Van A (Silver, 1.200.000 VND cũ)
Giảm giá: 5%
Tiền giảm: 250.000 VND
Tổng cần trả: 4.750.000 VND

Khách dua: 5.000.000 VND
Tiền thừa: 250.000 VND

Sau thanh toán:
  - Tổng chi tiêu mới: 1.200.000 + 4.750.000 = 6.000.000 VND
  - Hạng mới: Gold (giam 10%)
  - Lưu file khachhang.txt
  - Lưu hóa đơn vào history.txt
  - Lưu vào history_khach_1.txt
```

### Trường Hợp 2: Khách Guest
```
Tổng tiền ban đầu: 200.000 VND
Khách hàng: Guest
Giảm giá: 0%
Tiền giảm: 0 VND
Tổng cần trả: 200.000 VND

Khách dua: 200.000 VND
Tiền thừa: 0 VND

Sau thanh toán:
  - Không cập nhật khách hàng
  - Lưu hóa đơn vào history.txt (customerId = 0)
  - KHÔNG lưu history_khach_X.txt
```

---

## IX. FILE LIÊN QUAN

| File | Vị trí | Chức năng |
|------|--------|----------|
| checkout_controller.c | app/controllers/ | `processCheckout()` |
| checkout_service.h/c | app/services/ | `calculateSubtotal()`, `getDiscountPercent()`, `updateCustomerRank()` |
| file_service.c | app/services/ | `saveAllCustomersToFile()`, `saveBillToGlobalFile()`, `saveBillToHistoryFile()` |
| history_service.c | app/services/ | `addBillToHistory()` |
| btree_service.c | app/services/ | `searchBTree()` |
| bill_ui.c | app/ui/ | `printBillUI()` |
| helper.c | app/utils/ | `getCurrentDateTime()` |
| validator.c | app/utils/ | `getValidDouble()`, `getValidInt()` |

---

## X. TÓM TẮT

| Khía cạnh | Chi tiết |
|-----------|---------|
| **Cấu trúc dữ liệu** | Bill, Cart, B-Tree (tìm khách) |
| **Tính toán** | Subtotal → Discount % → Final Price |
| **Giảm giá** | Theo hạng VIP (0%, 5%, 10%, 15%) |
| **Nâng hạng** | Tự động khi totalSpent vượt ngưỡng |
| **Lưu trữ** | File (history.txt, history_khach_X.txt, khachhang.txt) |
| **Xác thực** | Tiền khách ≥ finalPrice |
| **Dọn dẹp** | Giỏ hàng, thông tin khách (nếu xác nhận) |

# CHỨC NĂNG 5: XEM LỊCH SỬ

## I. TỔNG QUÁT

**Tên chức năng:** Xem Lịch Sử  
**Lựa chọn:** Case 5 trong menu chính  
**Mô tả:** Xem lịch sử bán hàng của quán. Có thể xem tất cả hóa đơn, tìm lịch sử khách theo số điện thoại, hoặc xem chi tiết hóa đơn theo ID.

---

## II. CHỨC NĂNG CON

```
Xem Lịch Sử
│
├─ 1. Tất Cả Hóa Đơn (Mặc định)
│  └─ Hiển thị bảng: ID, Khách, SDT, Tổng, Ngày giờ
│
├─ 2. Lịch Sử Khách Theo SDT
│  ├─ Nhập SDT
│  ├─ Tìm khách
│  └─ Hiển thị tất cả hóa đơn của khách
│
├─ 3. Chi Tiết Hóa Đơn Theo ID Bill
│  ├─ Nhập ID Bill
│  ├─ Tìm hóa đơn
│  └─ Hiển thị chi tiết (item, tổng, v.v)
│
└─ 0. Quay Lại Menu
```

---

## III. CẤU TRÚC DỮ LIỆU CHÍNH

### HistoryNode (Nút lịch sử)
```c
typedef struct HistoryNode {
    int customerId;      // ID khách hàng (0 = Guest)
    Bill bill;           // Thông tin hóa đơn
    struct HistoryNode* next;
} HistoryNode;
```
**Lưu trữ:** Singly Linked List

### Bill (Hóa đơn)
```c
typedef struct {
    int id;              // ID hóa đơn
    Customer customer;   // Thông tin khách hàng
    Cart cart;           // Giỏ hàng (các item)
    double total;        // Tổng tiền trước giảm
    double discount;     // Tiền giảm giá
    double finalPrice;   // Tổng tiền sau giảm
    char dateTime[50];   // Ngày giờ
} Bill;
```

### Customer (Thông tin khách)
```c
typedef struct {
    int id;
    char phone[15];
    char name[50];
    char address[100];
    double totalSpent;
    char rank[20];
} Customer;
```

---

## IV. HÀM CHÍNH

### `handleViewHistory()` - Controller

**Vị trí:** `app/controllers/history_controller.c`  
**Mô tả:** Hiển thị menu lịch sử và xử lý các lựa chọn

**Luồng:**
```
1. Hiển thị tất cả lịch sử (mặc định)
2. Menu con:
   - 1: Xem lịch sử khách theo SDT
   - 2: Tìm hóa đơn theo ID
   - 0: Quay lại
3. Lặp lại đến khi chọn 0
```

---

## V. HÀM PHỤ TRỢ

### 1. `printAllHistory()` - UI

**Vị trí:** `app/ui/history_ui.c`  
**Mô tả:** In tất cả hóa đơn từ linked list hoặc file

**Pseudocode:**
```c
void printAllHistory() {
    printf("\n===== TAT CA HOA DON =====\n");
    printf("ID     | Khach        | SDT        | Tong      | Ngay Gio\n");
    printf("-------|--------------|------------|-----------|---------------------\n");
    
    HistoryNode* current = historyHead;
    while (current) {
        printf("%d | %-12s | %-10s | %9.0f | %s\n",
               current->bill.id,
               current->bill.customer.name,
               current->bill.customer.phone,
               current->bill.finalPrice,
               current->bill.dateTime);
        current = current->next;
    }
    printf("===========================\n");
}
```

### 2. `getCustomerIdByPhone(char* phone)` - Service

**Vị trí:** `app/services/history_service.c`  
**Mô tả:** Tìm ID khách hàng theo số điện thoại

**Pseudocode:**
```c
int getCustomerIdByPhone(char* phone) {
    HistoryNode* current = historyHead;
    
    while (current) {
        if (strcmp(current->bill.customer.phone, phone) == 0) {
            return current->bill.customer.id;
        }
        current = current->next;
    }
    
    return -1;  // Không tìm thấy
}
```

**Độ phức tạp:** O(m) - m là số hóa đơn

### 3. `getCustomerHistory(int customerId)` - Service

**Vị trí:** `app/services/history_service.c`  
**Mô tả:** Lấy linked list chỉ chứa hóa đơn của khách hàng

**Pseudocode:**
```c
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* head = NULL;
    HistoryNode* tail = NULL;
    
    HistoryNode* current = historyHead;
    while (current) {
        if (current->bill.customer.id == customerId) {
            // Tạo node mới
            HistoryNode* newNode = 
                (HistoryNode*)malloc(sizeof(HistoryNode));
            newNode->customerId = customerId;
            newNode->bill = current->bill;
            newNode->next = NULL;
            
            // Thêm vào danh sách
            if (head == NULL) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
        }
        current = current->next;
    }
    
    return head;
}
```

**Độ phức tạp:** O(m) - m là số hóa đơn

### 4. `searchBillById(int billId)` - Service

**Vị trí:** `app/services/history_service.c`  
**Mô tả:** Tìm hóa đơn theo ID

**Pseudocode:**
```c
HistoryNode* searchBillById(int billId) {
    HistoryNode* current = historyHead;
    
    while (current) {
        if (current->bill.id == billId) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;  // Không tìm thấy
}
```

**Độ phức tạp:** O(m)

### 5. `printCustomerHistory(HistoryNode* head)` - UI

**Vị trí:** `app/ui/history_ui.c`  
**Mô tả:** In lịch sử của một khách hàng

**In ra:**
```
===== LICH SU KHACH HANG =====
ID: 1
Tên: Nguyen Van A
SDT: 0987654321

Hóa Đơn:
BillID | Tổng      | Giảm      | Cuối cùng | Ngày Giờ
-------|-----------|-----------|-----------|---------------------
123456 | 150000    | 15000     | 135000    | 2024-01-15 10:30:45
123457 | 200000    | 20000     | 180000    | 2024-01-15 14:20:00
===============================
```

### 6. `printBillDetail(HistoryNode* billNode)` - UI

**Vị trí:** `app/ui/history_ui.c`  
**Mô tả:** In chi tiết của một hóa đơn

**In ra:**
```
==================== CHI TIET HOA DON ====================
Bill ID: 123456
Khách: Nguyen Van A
SDT: 0987654321
Ngày Giờ: 2024-01-15 10:30:45

STT | Tên Món           | Giá    | SL | Ghi Chú | Tổng
----|-------------------|--------|----|---------|---------
1   | Sườn Nướng (Suon) | 45000  | 2  |         | 90000
2   | Canh Chua         | 8000   | 1  |         | 8000
3   | Cơm               | 5000   | 2  |         | 10000
----|-------------------|--------|----|---------|---------
Tổng tiền ban đầu: 108.000 VND
Hạng VIP: GOLD (Giảm 10%)
Tiền giảm: 10.800 VND
Tổng tiền: 97.200 VND
=========================================================
```

---

## VI. LUỒNG HOẠT ĐỘNG CHI TIẾT

```
handleViewHistory()
│
├─→ 1. HIỂN THỊ TẤT CẢ (MẶC ĐỊNH):
│   ├─ printAllHistory()
│   │  └─ Duyệt historyHead → In bảng
│   └─ Hiển thị tất cả hóa đơn
│
├─→ 2. MENU CON:
│   ├─ Hiển thị:
│   │  "1. Xem lich su cua khach hang theo so dien thoai"
│   │  "2. Tim hoa don theo ID bill"
│   │  "0. Quay lai"
│   │
│   └─ choice = getValidInt("-> Chon: ")
│
├─→ 3A. CASE 1 - XEM LỊCH SỬ KHÁCH:
│   │
│   ├─ Nhập SDT:
│   │  └─ getValidPhoneNumber(phone, ...)
│   │
│   ├─ Tìm ID Khách:
│   │  ├─ customerId = getCustomerIdByPhone(phone)
│   │  │  └─ Duyệt linked list, so sánh phone
│   │  │
│   │  └─ IF customerId == -1:
│   │     ├─ printf("[!] Khong tim thay khach hang")
│   │     └─ CONTINUE (Lặp lại menu con)
│   │
│   ├─ Lấy Lịch Sử Khách:
│   │  ├─ currentHistory = getCustomerHistory(customerId)
│   │  │  └─ Duyệt linked list, lọc bill của khách
│   │  │
│   │  └─ printCustomerHistory(currentHistory)
│   │     └─ In bảng hóa đơn của khách
│   │
│   └─ Lặp lại menu con
│
├─→ 3B. CASE 2 - TÌM HÓA ĐƠN THEO ID:
│   │
│   ├─ Nhập ID Bill:
│   │  └─ billId = getValidInt(">> Nhap ID bill: ")
│   │
│   ├─ Tìm Hóa Đơn:
│   │  ├─ billNode = searchBillById(billId)
│   │  │  └─ Duyệt linked list, so sánh bill.id
│   │  │
│   │  └─ IF billNode == NULL:
│   │     ├─ printf("[!] Khong tim thay bill")
│   │     └─ CONTINUE
│   │
│   ├─ In Chi Tiết:
│   │  └─ printBillDetail(billNode)
│   │     └─ In chi tiết bill: khách, item, tổng
│   │
│   └─ Lặp lại menu con
│
├─→ 3C. CASE 0 - QUAY LẠI:
│   └─ break (Thoát vòng lặp con)
│
└─→ 4. QUAY LẠI MENU CHÍNH
```

---

## VII. PSEUDOCODE ĐẦY ĐỦ

```c
void handleViewHistory() {
    // 1. Hiển thị tất cả hóa đơn (mặc định)
    printAllHistory();
    
    while (1) {
        printf("\n=== LUA CHON ===\n");
        printf("1. Xem lich su khach hang theo so dien thoai\n");
        printf("2. Tim hoa don theo ID bill\n");
        printf("0. Quay lai\n");
        
        int choice = getValidInt("-> Chon: ");
        
        if (choice == 1) {
            // Tìm lịch sử khách
            char phone[15];
            getValidPhoneNumber(phone, sizeof(phone), ">> Nhap so dien thoai: ");
            
            int customerId = getCustomerIdByPhone(phone);
            
            if (customerId != -1) {
                HistoryNode* currentHistory = getCustomerHistory(customerId);
                printCustomerHistory(currentHistory);
            } else {
                printf("[!] Khong tim thay khach hang voi so dien thoai nay.\n");
            }
        } 
        else if (choice == 2) {
            // Tìm hóa đơn theo ID
            int billId = getValidInt(">> Nhap ID bill: ");
            HistoryNode* billNode = searchBillById(billId);
            
            if (billNode != NULL) {
                printBillDetail(billNode);
            } else {
                printf("[!] Khong tim thay bill voi ID nay.\n");
            }
        }
        else if (choice == 0) {
            break;  // Quay lại menu chính
        }
        else {
            printf("[!] Lua chon khong hop le.\n");
        }
    }
}
```

---

## VIII. VÍ DỤ DUYỆT LINKED LIST

### Cấu trúc Linked List Lịch Sử
```
Node 1               Node 2               Node 3
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ CustID: 1    │ ─→ │ CustID: 0    │ ─→ │ CustID: 1    │ ─→ NULL
│ Bill 123456  │    │ Bill 123457  │    │ Bill 123458  │
└──────────────┘    └──────────────┘    └──────────────┘
      ▲
      │
   historyHead
```

### Tìm Lịch Sử Khách ID=1
```
getCustomerHistory(1):
  - Duyệt từ historyHead
  - Node 1: CustID == 1 ✓ → Thêm vào kết quả
  - Node 2: CustID == 0 ✗ → Bỏ qua
  - Node 3: CustID == 1 ✓ → Thêm vào kết quả
  
Kết quả:
  Node 1' (Copy)
    ↓
  Node 3' (Copy)
    ↓
  NULL
```

---

## IX. CẤU TRÚC FILE DỮ LIỆU

### `app/database/history.txt` - Lịch sử chung
```
BILL|123456|1|Nguyen Van A|0987654321|2024-01-15 10:30|150000|15000|135000|3
ITEM|1|Sườn Nướng|40000|1|2|Không cay|40000
ITEM|5|Cơm|5000|2|0||10000
ITEM|20|Nước Ngọt|12000|1|0||12000
BILL|123457|0|Guest||||60000|0|60000|1
ITEM|3|Thịt Lợn Nướng|60000|1|0||60000
BILL|123458|1|Nguyen Van A|0987654321|2024-01-15 14:20|200000|20000|180000|2
ITEM|2|Sườn Kho|32000|1|0|Thêm cơm|32000
ITEM|19|Cơm|5000|1|0||5000
```

**Format:**
- `BILL|ID|CustomerID|Name|Phone|DateTime|Total|Discount|FinalPrice|ItemCount`
- `ITEM|ItemID|Name|Price|Qty|Option|Note|TotalPrice`

### `app/database/history_khach_1.txt` - Lịch sử khách 1
```
(Cùng định dạng như history.txt, chỉ chứa bill của khách 1)
```

---

## X. KHỞI TẠO LINKED LIST

### `loadHistoryFromFile()` - Service

**Vị trí:** `app/services/file_service.c`  
**Mô tả:** Nạp lịch sử từ file vào linked list

**Pseudocode:**
```
loadHistoryFromFile():
  file = fopen("app/database/history.txt", "r")
  
  WHILE fgets(line, file):
    IF line starts with "BILL|":
      Parse: billId, customerId, name, phone, dateTime, ...
      
      Tạo HistoryNode mới:
        newNode->customerId = customerId
        newNode->bill.id = billId
        newNode->bill.customer.id = customerId
        ... (Gán các field khác)
        newNode->next = NULL
      
      Thêm vào cuối linked list:
        IF historyHead == NULL:
          historyHead = newNode
        ELSE:
          temp = historyHead
          WHILE temp->next != NULL:
            temp = temp->next
          temp->next = newNode
    
    ELSE IF line starts with "ITEM|":
      Parse: itemId, name, price, qty, option, note, totalPrice
      Thêm vào cart của bill hiện tại
  
  fclose(file)
```

---

## XI. FILE LIÊN QUAN

| File | Vị trí | Chức năng |
|------|--------|----------|
| history_controller.c | app/controllers/ | `handleViewHistory()` |
| history_service.h/c | app/services/ | `getCustomerIdByPhone()`, `getCustomerHistory()`, `searchBillById()` |
| file_service.c | app/services/ | `loadHistoryFromFile()` |
| history_ui.h/c | app/ui/ | `printAllHistory()`, `printCustomerHistory()`, `printBillDetail()` |
| models.h | app/models/ | Định nghĩa struct HistoryNode, Bill |
| validator.h/c | app/utils/ | `getValidPhoneNumber()`, `getValidInt()` |

---

## XII. TÓM TẮT

| Khía cạnh | Chi tiết |
|-----------|---------|
| **Cấu trúc dữ liệu** | Singly Linked List |
| **Lưu trữ** | Bộ nhớ (RAM) + File (history.txt) |
| **Độ phức tạp** | O(m) - m là số hóa đơn |
| **Tìm kiếm** | Theo SDT (O(m)) hoặc BillID (O(m)) |
| **Hiển thị** | Tất cả, Khách riêng, Chi tiết |
| **Khởi tạo** | Nạp từ file khi chương trình khởi động |

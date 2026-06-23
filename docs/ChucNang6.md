# CHỨC NĂNG 6: THỐNG KÊ DOANH THU & XUẤT FILE BÁO CÁO

## I. TỔNG QUÁT

**Tên chức năng:** Thống Kê Doanh Thu & Xuất File Báo Cáo  
**Lựa chọn:** Case 6 trong menu chính  
**Mô tả:** Thống kê doanh thu tổng quát, số hóa đơn, số món đã bán, phân tích theo từng khách hàng, và xuất file báo cáo chi tiết.

---

## II. CÁC CHỈ SỐ THỐNG KÊ

```
1. Tổng Doanh Thu
   = Σ(finalPrice) của tất cả bill

2. Tổng Số Hóa Đơn
   = Số lượng dòng "BILL|" trong file

3. Tổng Số Món Đã Bán
   = Σ(itemCount) của tất cả bill

4. Phân Tích Theo Khách
   = CustomerID | Số Hóa Đơn | Tổng Doanh Thu
```

---

## III. CẤU TRÚC DỮ LIỆU CHÍNH

### CustomerRevenue (Thống kê khách hàng)
```c
typedef struct {
    int customerId;      // ID khách hàng
    int billCount;       // Số hóa đơn của khách
    double revenue;      // Tổng doanh thu từ khách
} CustomerRevenue;
```

---

## IV. HÀM CHÍNH

### `handleGenerateReport()` - Controller

**Vị trí:** `app/services/report_service.c`  
**Mô tả:** Xử lý toàn bộ quá trình tạo báo cáo

**Luồng:**
```
1. Tính các chỉ số chính
2. Phân tích theo khách hàng
3. Hiển thị báo cáo
4. Xuất file báo cáo
```

---

## V. HÀM PHỤ TRỢ

### 1. `calculateTotalRevenue()` - Service

**Vị trị:** `app/services/report_service.c`  
**Mô tả:** Tính tổng doanh thu từ tất cả hóa đơn

**Pseudocode:**
```c
double calculateTotalRevenue() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        printf("[!] Khong the mo file history.txt\n");
        return 0.0;
    }
    
    double totalRevenue = 0.0;
    char line[500];
    
    while (fgets(line, sizeof(line), file)) {
        // Loại bỏ ký tự xuống dòng
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        
        // Chỉ xử lý dòng BILL
        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemCount;
            char nameKhach[50], phone[15], dateTime[50];
            double total, discount, finalPrice;
            
            // Parse thông tin từ dòng BILL
            // Format: BILL|ID|CustomerID|Name|Phone|DateTime|Total|Discount|FinalPrice|ItemCount
            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount);
            
            totalRevenue += finalPrice;  // Cộng tiền cuối cùng
        }
    }
    
    fclose(file);
    return totalRevenue;
}
```

**Độ phức tạp:** O(n) - n là số dòng file

### 2. `calculateTotalBills()` - Service

**Vị trị:** `app/services/report_service.c`  
**Mô tả:** Tính tổng số hóa đơn

**Pseudocode:**
```c
int calculateTotalBills() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    int billCount = 0;
    char line[500];
    
    while (fgets(line, sizeof(line), file)) {
        // Đếm số dòng bắt đầu bằng "BILL|"
        if (strncmp(line, "BILL|", 5) == 0) {
            billCount++;
        }
    }
    
    fclose(file);
    return billCount;
}
```

**Độ phức tạp:** O(n)

### 3. `calculateTotalItems()` - Service

**Vị trị:** `app/services/report_service.c`  
**Mô tả:** Tính tổng số món đã bán

**Pseudocode:**
```c
int calculateTotalItems() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return 0;
    }
    
    int itemCount = 0;
    char line[500];
    
    while (fgets(line, sizeof(line), file)) {
        // Loại bỏ ký tự xuống dòng
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        
        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemsInBill;
            char nameKhach[50], phone[15], dateTime[50];
            double total, discount, finalPrice;
            
            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemsInBill);
            
            itemCount += itemsInBill;  // Cộng dồn số item
        }
    }
    
    fclose(file);
    return itemCount;
}
```

**Độ phức tạp:** O(n)

### 4. `analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount)` - Service

**Vị trị:** `app/services/report_service.c`  
**Mô tả:** Phân tích doanh thu theo từng khách hàng

**Pseudocode:**
```c
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount) {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        *customerCount = 0;
        return;
    }
    
    *customerCount = 0;
    char line[500];
    
    while (fgets(line, sizeof(line), file)) {
        // Loại bỏ ký tự xuống dòng
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }
        
        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemCount;
            char nameKhach[50], phone[15], dateTime[50];
            double total, discount, finalPrice;
            
            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount);
            
            // Tìm hoặc tạo entry cho khách
            CustomerRevenue* cust = 
                findOrCreateCustomer(customerStats, customerCount, customerId);
            
            if (cust != NULL) {
                cust->billCount++;
                cust->revenue += finalPrice;
            }
        }
    }
    
    fclose(file);
}
```

**Độ phức tạp:** O(n × m) - n dòng file, m khách

### 5. `findOrCreateCustomer()` - Service

**Vị trị:** `app/services/report_service.c`  
**Mô tả:** Tìm khách trong mảng hoặc tạo entry mới

**Pseudocode:**
```c
CustomerRevenue* findOrCreateCustomer(
    CustomerRevenue* customerStats, 
    int* customerCount, 
    int customerId) {
    
    // Tìm khách đã tồn tại
    for (int i = 0; i < *customerCount; i++) {
        if (customerStats[i].customerId == customerId) {
            return &customerStats[i];
        }
    }
    
    // Tạo entry mới (nếu không quá giới hạn)
    #define MAX_CUSTOMERS 100
    if (*customerCount < MAX_CUSTOMERS) {
        customerStats[*customerCount].customerId = customerId;
        customerStats[*customerCount].billCount = 0;
        customerStats[*customerCount].revenue = 0.0;
        return &customerStats[(*customerCount)++];
    }
    
    return NULL;
}
```

### 6. `printReportUI()` - UI

**Vị trị:** `app/ui/report_ui.c`  
**Mô tả:** In báo cáo lên màn hình

**In ra:**
```
=================================================
    BAO CAO DOANH THU QUAN COM TAM
=================================================
Thoi gian: 2024-01-15 15:30:45

A. TONG HOP
  - Tong doanh thu: 1.250.000 VND
  - Tong so hoa don: 15
  - Tong so mon da ban: 42

B. PHAN TICH THEO KHACH HANG
  CustomerID | So Hoa Don | Doanh Thu
  1          | 5          | 650.000 VND
  2          | 3          | 300.000 VND
  3          | 7          | 300.000 VND

=================================================
```

### 7. `exportReportToFile()` - Service

**Vị trị:** `app/ui/report_ui.c`  
**Mô tả:** Xuất báo cáo vào file

**File:** `app/database/report_doanhthu.txt`

**Format:**
```
=================================================
    BAO CAO DOANH THU QUAN COM TAM
=================================================
Thoi gian: [Ngày giờ]

A. TONG HOP
  - Tong doanh thu: X.XXX VND
  - Tong so hoa don: Y
  - Tong so mon da ban: Z

B. PHAN TICH THEO KHACH HANG
  CustomerID | So Hoa Don | Doanh Thu
  ...
```

---

## VI. LUỒNG HOẠT ĐỘNG CHI TIẾT

```
handleGenerateReport()
│
├─→ 1. TÍNH TOÁN CÁC CHỈ SỐ:
│   │
│   ├─ Tổng Doanh Thu:
│   │  ├─ calculateTotalRevenue()
│   │  │  ├─ Mở file history.txt
│   │  │  ├─ Duyệt từng dòng
│   │  │  ├─ Nếu là BILL: Parse & lấy finalPrice
│   │  │  ├─ Cộng dồn vào totalRevenue
│   │  │  └─ Đóng file
│   │  └─ Kết quả: totalRevenue
│   │
│   ├─ Tổng Số Hóa Đơn:
│   │  ├─ calculateTotalBills()
│   │  │  ├─ Mở file history.txt
│   │  │  ├─ Duyệt từng dòng
│   │  │  ├─ Đếm dòng bắt đầu "BILL|"
│   │  │  └─ Đóng file
│   │  └─ Kết quả: totalBills
│   │
│   ├─ Tổng Số Món:
│   │  ├─ calculateTotalItems()
│   │  │  ├─ Mở file history.txt
│   │  │  ├─ Duyệt từng dòng
│   │  │  ├─ Nếu là BILL: Lấy itemCount
│   │  │  ├─ Cộng dồn vào totalItems
│   │  │  └─ Đóng file
│   │  └─ Kết quả: totalItems
│   │
│   └─ Phân Tích Theo Khách:
│      ├─ analyzeByCustomer(customerStats, &customerCount)
│      │  ├─ Mở file history.txt
│      │  ├─ Duyệt từng dòng
│      │  ├─ Nếu là BILL: Parse customerId
│      │  ├─ findOrCreateCustomer()
│      │  │  ├─ Tìm customerId trong mảng
│      │  │  ├─ Nếu tồn tại: Trả về con trỏ
│      │  │  ├─ Nếu chưa: Tạo entry mới
│      │  │  └─ Trả về con trỏ
│      │  ├─ Cộng dồn: billCount++, revenue += finalPrice
│      │  └─ Đóng file
│      └─ Kết quả: customerStats[] + customerCount
│
├─→ 2. HIỂN THỊ BÁO CÁO:
│   │
│   ├─ printReportUI(
│   │      totalRevenue,
│   │      totalBills,
│   │      totalItems,
│   │      customerStats,
│   │      customerCount)
│   │  │
│   │  ├─ In:
│   │  │  "BAO CAO DOANH THU QUAN COM TAM"
│   │  │  "Thoi gian: [datetime]"
│   │  │
│   │  ├─ In TONG HOP:
│   │  │  - Tong doanh thu: totalRevenue
│   │  │  - Tong so hoa don: totalBills
│   │  │  - Tong so mon da ban: totalItems
│   │  │
│   │  ├─ In PHAN TICH:
│   │  │  - Bảng: CustomerID, Số Hóa Đơn, Doanh Thu
│   │  │  - Duyệt customerStats[]
│   │  │  - In từng khách
│   │  │
│   │  └─ Tạo file report_doanhthu.txt:
│   │     └─ exportReportToFile()
│   │
│   └─ Báo cáo xuất xong
│
└─→ 3. QUAY LẠI MENU CHÍNH
```

---

## VII. PSEUDOCODE ĐẦY ĐỦ

```c
void handleGenerateReport() {
    printf("\n=== THONG KE DOANH THU ===\n");
    
    // 1. Tính toán
    double totalRevenue = calculateTotalRevenue();
    int totalBills = calculateTotalBills();
    int totalItems = calculateTotalItems();
    
    // 2. Phân tích theo khách
    #define MAX_CUSTOMERS 100
    CustomerRevenue customerStats[MAX_CUSTOMERS];
    int customerCount = 0;
    analyzeByCustomer(customerStats, &customerCount);
    
    // 3. Hiển thị báo cáo
    printReportUI(totalRevenue, totalBills, totalItems, 
                  customerStats, customerCount);
    
    // 4. Xuất file
    exportReportToFile(totalRevenue, totalBills, totalItems,
                       customerStats, customerCount);
    
    printf("\n[OK] Xuat file bao cao thanh cong!\n");
    printf("File: app/database/report_doanhthu.txt\n");
}
```

---

## VIII. VÍ DỤ PHÂN TÍCH

### Input File: `history.txt`
```
BILL|123456|1|Nguyen Van A|0987654321|2024-01-15 10:30|150000|15000|135000|3
ITEM|1|Sườn Nướng|40000|1|2||40000
ITEM|5|Cơm|5000|2|0||10000
ITEM|20|Nước Ngọt|12000|1|0||12000

BILL|123457|0|Guest||||60000|0|60000|1
ITEM|3|Thịt Lợn|60000|1|0||60000

BILL|123458|1|Nguyen Van A|0987654321|2024-01-15 14:20|200000|20000|180000|2
ITEM|2|Sườn Kho|32000|1|0||32000
ITEM|19|Cơm|5000|1|0||5000

BILL|123459|2|Tran Thi B|0912345678|2024-01-15 16:45|100000|10000|90000|2
ITEM|4|Cút Kho|26000|1|0||26000
ITEM|19|Cơm|5000|2|0||10000
```

### Quá Trình Tính Toán

**1. Tổng Doanh Thu:**
```
Duyệt từng dòng BILL:
  - Bill 123456: finalPrice = 135000 → total = 135000
  - Bill 123457: finalPrice = 60000 → total = 195000
  - Bill 123458: finalPrice = 180000 → total = 375000
  - Bill 123459: finalPrice = 90000 → total = 465000

totalRevenue = 465000 VND
```

**2. Tổng Số Hóa Đơn:**
```
Đếm dòng BILL:
  - BILL|123456
  - BILL|123457
  - BILL|123458
  - BILL|123459

totalBills = 4
```

**3. Tổng Số Món:**
```
Duyệt từng dòng BILL, lấy itemCount (tham số cuối):
  - Bill 123456: itemCount = 3 → total = 3
  - Bill 123457: itemCount = 1 → total = 4
  - Bill 123458: itemCount = 2 → total = 6
  - Bill 123459: itemCount = 2 → total = 8

totalItems = 8
```

**4. Phân Tích Theo Khách:**
```
Duyệt từng dòng BILL, nhóm theo customerId:

customerId = 0 (Guest):
  - Bill 123457: revenue = 60000
  → CustomerRevenue: {0, 1, 60000}

customerId = 1 (Nguyen Van A):
  - Bill 123456: revenue = 135000
  - Bill 123458: revenue = 180000
  → CustomerRevenue: {1, 2, 315000}

customerId = 2 (Tran Thi B):
  - Bill 123459: revenue = 90000
  → CustomerRevenue: {2, 1, 90000}

Kết quả:
  customerStats[] = [
    {0, 1, 60000},
    {1, 2, 315000},
    {2, 1, 90000}
  ]
  customerCount = 3
```

---

## IX. FILE XUẤT RA

### `app/database/report_doanhthu.txt`
```
=================================================
    BAO CAO DOANH THU QUAN COM TAM
=================================================
Thoi gian: 2024-01-15 17:00:00

A. TONG HOP
  - Tong doanh thu: 465.000 VND
  - Tong so hoa don: 4
  - Tong so mon da ban: 8
  - Tong so khach hang (co dang ky): 2

B. PHAN TICH THEO KHACH HANG
  CustomerID | Tên                | So Hoa Don | Doanh Thu
  0          | Guest              | 1          | 60.000 VND
  1          | Nguyen Van A       | 2          | 315.000 VND
  2          | Tran Thi B         | 1          | 90.000 VND

C. TOP KHACH HANG (Theo Doanh Thu)
  1. Nguyen Van A (ID: 1) - 315.000 VND
  2. Tran Thi B (ID: 2) - 90.000 VND
  3. Guest (ID: 0) - 60.000 VND

=================================================
```

---

## X. FILE LIÊN QUAN

| File | Vị trí | Chức năng |
|------|--------|----------|
| report_service.h/c | app/services/ | Toàn bộ hàm tính toán |
| report_ui.h/c | app/ui/ | `printReportUI()`, `exportReportToFile()` |
| file_service.c | app/services/ | I/O file |
| models.h | app/models/ | Định nghĩa struct CustomerRevenue |

---

## XI. PHÂN TÍCH ĐỘ PHỨC TẠP

| Hàm | Độ phức tạp | Ghi chú |
|-----|------------|---------|
| `calculateTotalRevenue()` | O(n) | n = số dòng file |
| `calculateTotalBills()` | O(n) | n = số dòng file |
| `calculateTotalItems()` | O(n) | n = số dòng file |
| `analyzeByCustomer()` | O(n × m) | n = dòng, m = khách |
| `findOrCreateCustomer()` | O(m) | m = khách |
| `handleGenerateReport()` | O(n × m) | n dòng, m khách |

---

## XII. TÓM TẮT

| Khía cạnh | Chi tiết |
|-----------|---------|
| **Chỉ số chính** | Tổng doanh thu, Số bill, Số món |
| **Phân tích** | Theo từng khách hàng |
| **Lưu trữ** | File history.txt → Parse & Tính |
| **Độ phức tạp** | O(n × m) - n dòng file, m khách |
| **Xuất ra** | Report file (report_doanhthu.txt) |
| **Format** | Text có cấu trúc, dễ đọc |
| **Thời điểm** | Bất kỳ lúc nào (case 6) |

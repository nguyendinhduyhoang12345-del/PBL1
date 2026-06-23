# 📋 CHECKLIST KIỂM TRA BÁO CÁO PBL1

> **Dựa trên phân tích code dự án**  
> Danh sách đầy đủ những gì nên có trong báo cáo + chỉ ra phần thường thiếu

---

## 📌 PHẦN I: THÔNG TIN CƠ BẢN

### ✅ Phần Mở Đầu
- [ ] Tên đề tài: **Đặt Món Thông Minh, Tối Ưu Thanh Toán**
- [ ] Môn học: **PBL1 — Lập Trình Tính Toán**
- [ ] Giáo viên: **PGS. TS. Trương Ngọc Châu**
- [ ] Sinh viên: **2 người** (MSSV, Họ tên, Lớp)

### ✅ Mục Đích & Phạm Vi
- [ ] **Mục đích:** Quản lý quán cơm tấm (đặt món, thanh toán, khách thành viên, lịch sử, thống kê)
- [ ] **Công nghệ:** Vanilla C (không framework)
- [ ] **Kiến trúc:** MVC + Services Layer
- [ ] **Nền tảng:** Windows (C99/C11, GCC, ANSI colors)

**⚠️ THƯỜNG THIẾU:** Giải thích tại sao chọn MVC thay vì kiến trúc khác

---

## 📌 PHẦN II: CẤU TRÚC DỮ LIỆU (QUAN TRỌNG NHẤT!)

### 1️⃣ HASH TABLE - Quản Lý Menu

**📍 Dùng để:** Lưu 25 món ăn, tra cứu nhanh theo ID  
**📍 File:** `hash_service.c/h` + `models.h`

| Thuộc tính | Chi tiết |
|---|---|
| **Kích thước** | 101 (số nguyên tố) |
| **Hàm hash** | `hash(key) = key % 101` |
| **Xử lý đụng độ** | Chaining (Singly Linked List) |
| **Độ phức tạp tìm kiếm** | **O(1)** trung bình, **O(k)** tệ nhất (k = số đụng độ) |

**📋 Cấu trúc:**
```c
typedef struct HashNode {
    int key;              // ID món (1-25)
    MenuItem item;        // Thông tin món
    struct HashNode* next; // Xử lý đụng độ
} HashNode;

typedef struct {
    HashNode **table;  // Mảng 101 bucket
    int size;
} HashTable;
```

**🎯 Ví dụ Hash Function:**
| ID Món | Phép tính | Vị trí Bucket |
|--------|----------|---------------|
| 1 | 1 % 101 | 1 |
| 5 | 5 % 101 | 5 |
| 12 | 12 % 101 | 12 |
| 25 | 25 % 101 | 25 |

**💡 Tại sao 101?**
- Số nguyên tố → Phân phối đều nhất
- Tránh nhóm các key cùng ước → Giảm đụng độ

**⚠️ THƯỜNG THIẾU:**
- Công thức hash chi tiết
- Minh họa bộ nhớ (diagram)
- Ví dụ xử lý đụng độ
- So sánh O(1) vs O(n) nếu dùng mảng

---

### 2️⃣ B-TREE - Quản Lý Khách Hàng

**📍 Dùng để:** Lưu khách hàng, tìm kiếm theo SĐT  
**📍 File:** `btree_service.c/h` + `models.h`

| Thuộc tính | Chi tiết |
|---|---|
| **Bậc (Order)** | 4 |
| **Khóa tối đa/node** | `ORDER - 1 = 3` |
| **Con tối đa/node** | `ORDER = 4` |
| **Khóa tối thiểu (trừ root)** | `⌈ORDER/2⌉ - 1 = 1` |
| **Độ phức tạp tìm/thêm** | **O(log n)** |

**📋 Cấu trúc Node:**
```c
#define BTREE_ORDER 4

typedef struct BTreeNode {
    Customer* customers[3];        // Tối đa 3 khóa
    struct BTreeNode* children[4]; // Tối đa 4 con
    int numKeys;                   // Số khóa hiện có
    int isLeaf;                    // 1=lá, 0=trong
} BTreeNode;
```

**🎯 Minh Họa Node Đầy (3 khóa):**
```
         customers[0]   customers[1]   customers[2]
        ┌────────────┬────────────┬────────────┐
        │  "0901"    │  "0912"    │  "0933"    │
        └─────┬──────┴──────┬─────┴──────┬─────┘
              │             │            │
          child[0]      child[1]     child[2]    child[3]
         (< 0901)   (0901-0912)  (0912-0933)  (> 0933)
```

**📊 Quy Tắc Vàng:**
- Khóa **luôn sắp xếp tăng dần**
- Mọi khóa trong `child[i]` nằm giữa `customers[i-1]` và `customers[i]`

**🔧 3 Hàm Chính:**
1. **`insertBTree(root, customer)`** - Hàm ngoài, kiểm tra root có đầy
2. **`insertNonFull(node, customer)`** - Hàm thực sự chèn xuống
3. **`splitChild(parent, i, fullChild)`** ⭐ ← **RẤT QUAN TRỌNG** - Chia node khi đầy
4. **`searchBTree(root, phone)`** - Tìm khách theo SĐT

**⚠️ THƯỜNG THIẾU:**
- Hàm `splitChild()` - cách chia node
- Ví dụ insert/split trực quan
- Điều kiện cân bằng chi tiết
- Tại sao B-Tree thay vì BST (AVL)?

---

### 3️⃣ DOUBLY LINKED LIST - Giỏ Hàng

**📍 Dùng để:** Quản lý các item trong giỏ hàng  
**📍 File:** `cart_service.c/h`

| Thuộc tính | Chi tiết |
|---|---|
| **Cấu trúc** | Danh sách liên kết đôi |
| **Thao tác** | Thêm, xóa, sửa item |
| **Độ phức tạp** | O(1) thêm đầu, O(k) xóa/sửa |

**📋 Cấu trúc:**
```c
typedef struct CartNode {
    OrderItem item;
    struct CartNode *prev;  // ← Con trỏ lùi
    struct CartNode *next;  // → Con trỏ tiến
} CartNode;

typedef struct {
    CartNode *head;  // Nút đầu
    CartNode *tail;  // Nút cuối
    int itemCount;   // Số item
} Cart;
```

**🎯 Lợi Ích DLL vs Mảng:**
| So sánh | Mảng | DLL |
|---------|------|-----|
| Thêm vào đầu | O(n) | O(1) |
| Xóa ở giữa | O(n) | O(1) |
| Truy cập | O(1) | O(k) |
| Cấp phát | Cố định | Động |

**⚠️ THƯỜNG THIẾU:**
- Tại sao DLL thay vì mảng?
- Pseudocode removeFromCart()

---

### 4️⃣ SINGLY LINKED LIST - Lịch Sử Hóa Đơn

**📍 Dùng để:** Lưu lịch sử bán hàng  
**📍 File:** `history_service.c/h`

**📋 Cấu trúc:**
```c
typedef struct HistoryNode {
    int customerId;      // ID khách
    Bill bill;           // Thông tin hóa đơn
    struct HistoryNode* next;
} HistoryNode;
```

**⚠️ THƯỜNG THIẾU:**
- Tại sao không dùng file đơn giản?
- Tại sao không dùng mảng?

---

## 📌 PHẦN III: CÁC CHỨC NĂNG CHÍNH

### 🎯 CHỨC NĂNG 1: ĐẶT MÓN

**📍 Menu:** Case 1 | File: `order_controller.c`

**📋 Thông tin:**
- **Menu:** 25 món (12 chính, 13 phụ & nước)
- **Giới hạn:** ≤ 5 món chính
- **Tùy chọn:** Cot lẻ hoặc sườn cay (+10,000 VND)
- **Ghi chú:** Có

**🔄 Luồng:**
```
1. Hiển thị menu từ Hash Table
2. Vòng lặp nhập ID:
   - Xác thực ID (1-25)
   - Tra cứu món: hashTableSearch(menuHashTable, id)
   - Kiểm tra giới hạn 5 món chính
   - Nhập tùy chọn (nếu có)
   - Nhập số lượng
   - Nhập ghi chú
   - Tạo OrderItem
   - addToCart(bill, item)
3. Nhập 0 để thoát
```

**💾 Dữ Liệu Lưu:**
- Trong **Doubly Linked List** (giỏ hàng)

**⚠️ THƯỜNG THIẾU:**
- Danh sách 25 món cụ thể + giá
- Flowchart chi tiết
- Screenshot giao diện menu

---

### 🎯 CHỨC NĂNG 2: XEM & CHỈNH SỬA GIỎ HÀNG

**📍 Menu:** Case 2 | File: `order_controller.c`

**📋 Thông tin:**
- Hiển thị danh sách item hiện tại
- Xóa item (chọn STT)
- Sửa số lượng (với kiểm tra giới hạn)
- Dùng Doubly Linked List

**🔄 Luồng:**
```
1. showCartUI(bill) - Hiển thị giỏ
2. Menu con: 1. Xóa | 2. Sửa | 0. Quay lại
3. Nếu xóa:
   - Nhập STT (1 đến itemCount)
   - Kiểm tra hợp lệ
   - cartRemoveByIndex(bill->cart, stt-1)
4. Nếu sửa:
   - Nhập STT
   - Nhập số lượng mới
   - Kiểm tra nếu chính: tổng ≤ 5
   - cartUpdateQuantity()
5. Lặp lại đến chọn 0
```

**⚠️ THƯỜNG THIẾU:**
- Pseudocode removeFromCart()
- Cách kiểm tra giới hạn 5 món

---

### 🎯 CHỨC NĂNG 3: NHẬP THÔNG TIN KHÁCH HÀNG

**📍 Menu:** Case 3 | File: `customer_controller.c`

**📋 Thông tin:**
- Tìm khách theo SĐT (B-Tree)
- Nếu cũ: Hiển thị thông tin + hạng
- Nếu mới: Cho phép đăng ký
- Xác thực SĐT: 10 chữ số, bắt đầu từ 0

**🔄 Luồng:**
```
1. Nhập SĐT (xác thực format)
2. Tìm kiếm: searchBTree(btreeRoot, sdt)
3. Nếu tìm thấy (found != NULL):
   ├─ Hiển thị: ID, Tên, SĐT, Địa chỉ, Tổng chi tiêu, Hạng
   ├─ Gán: currentBill->customer = found
   └─ Quay lại menu
4. Nếu không tìm thấy (found == NULL):
   ├─ Menu: 1. Đăng ký | 0. Tiếp tục (Guest)
   └─ Nếu chọn 1:
      ├─ Nhập tên
      ├─ Nhập địa chỉ
      ├─ Tạo ID tự động
      ├─ Khởi tạo: TotalSpent=0, Rank="Bronze"
      ├─ Thêm vào B-Tree: insertBTree(&btreeRoot, newCustomer)
      ├─ Lưu file: saveCustomerToFile(newCustomer)
      └─ Gán vào bill
```

**🏆 Hệ Thống Rank VIP:**
```
Bronze:   < 1.000.000 VND       → Giảm 0%
Silver:   1.000.000 - 5.000.000 → Giảm 5%
Gold:     5.000.000 - 10.000.000 → Giảm 10%
Diamond:  ≥ 10.000.000 VND      → Giảm 15%
```

**⚠️ THƯỜNG THIẾU:**
- Validation rules chi tiết (SĐT, tên, v.v.)
- Hệ thống rank + công thức nâng hạng
- Cách lưu khách vào file

---

### 🎯 CHỨC NĂNG 4: THANH TOÁN

**📍 Menu:** Case 4 | File: `checkout_controller.c`

**📋 Quy Trình Thanh Toán:**
```
Kiểm tra Giỏ Rỗng?
│
├─ Rỗng → Báo lỗi, thoát
│
└─ Có hàng:
    ├─ Tính tổng tiền (subtotal)
    ├─ Tìm khách hàng (nếu có)
    ├─ Tra cứu rank → % giảm
    ├─ Tính tiền giảm = subtotal * %
    ├─ Tính tiền cuối cùng = subtotal - discount
    ├─ Hiển thị hóa đơn chi tiết
    ├─ Nhập tiền khách
    ├─ Tính tiền thối
    ├─ Hỏi xác nhận
    │
    └─ Nếu có (YES):
        ├─ Cộng tiền vào customer.totalSpent
        ├─ Cập nhật rank: updateCustomerRank()
        ├─ Lưu khách: saveCustomerToFile()
        ├─ Lưu hóa đơn: saveBillToHistoryFile()
        ├─ Lưu lịch sử khách: saveToCustomerHistoryFile()
        ├─ Dọn giỏ: cartClear()
        └─ In hóa đơn + thông báo thành công
    
    └─ Nếu không (NO):
        └─ Hủy, quay lại
```

**📊 Công Thức Thanh Toán:**
```
Subtotal = Σ(item.price × item.quantity)
DiscountPercent = getDiscountPercent(customer.rank)
DiscountAmount = Subtotal × DiscountPercent
FinalPrice = Subtotal - DiscountAmount
```

**💾 Cấu Trúc Hóa Đơn:**
```c
typedef struct {
    int id;           // ID hóa đơn (tự tăng)
    Customer customer;
    Cart cart;        // Danh sách item
    double total;     // Tiền trước giảm
    double discount;  // Tiền giảm (VND)
    double finalPrice;// Tiền sau giảm
    char dateTime[50];// Ngày giờ
} Bill;
```

**📁 Format File Lưu (history.txt):**
```
BILL|1|1|Tran Van A|0901234567|2024-12-20 10:30|120000|6000|114000|3
ITEM|1|Com tam Sut Nuong|2|50000|1|Them tuoi
ITEM|5|Trung OP|1|15000|0|
```

**⚠️ THƯỜNG THIẾU:**
- Công thức tính toán chi tiết
- Format file lưu hóa đơn
- Cách cập nhật totalSpent & rank
- Ví dụ thanh toán cụ thể

---

### 🎯 CHỨC NĂNG 5: XEM LỊCH SỬ

**📍 Menu:** Case 5 | File: `history_controller.c`

**📋 3 Chế Độ Xem:**

**1. Tất Cả Hóa Đơn (Mặc định)**
```
ID  | Khách         | SĐT        | Tổng      | Ngày Giờ
----|---------------|------------|-----------|---------------------
1   | Tran Van A    | 0901234567 | 114000    | 2024-12-20 10:30
2   | Nguyen Van B  | 0912345678 | 250000    | 2024-12-20 11:15
...
```

**2. Lịch Sử Khách Theo SĐT**
- Nhập SĐT
- Hiển thị tất cả hóa đơn của khách

**3. Chi Tiết Hóa Đơn Theo ID Bill**
- Nhập ID Bill
- Hiển thị: Khách, Item, Tổng, Giảm, Cuối cùng

**💾 Dữ Liệu Lưu:**
- **history.txt** - Lịch sử chung (Singly Linked List)
- **history_khach_X.txt** - Lịch sử khách (X = Customer ID)

**⚠️ THƯỜNG THIẾU:**
- Format hiển thị chi tiết
- Cách tìm kiếm hiệu quả
- Cách lưu file lịch sử khách

---

### 🎯 CHỨC NĂNG 6: THỐNG KÊ DOANH THU & XUẤT BÁO CÁO

**📍 Menu:** Case 6 | File: `report_controller.c`

**📋 Các Chỉ Số:**

| Chỉ Số | Công Thức | Ví Dụ |
|--------|-----------|-------|
| **Tổng Doanh Thu** | Σ(bill.finalPrice) | 1.500.000 VND |
| **Tổng Số Hóa Đơn** | Đếm dòng "BILL\|" | 50 |
| **Tổng Số Món Bán** | Σ(item.quantity) | 150 |
| **Doanh Thu Trung Bình/Bill** | TổngDT / SốBill | 30.000 VND |

**📊 Phân Tích Theo Khách:**
```
Customer_ID | Tên            | Số Bill | Tổng DT   | Rank
------------|----------------|---------|-----------|--------
1           | Tran Van A     | 15      | 500.000   | Gold
2           | Nguyen Van B   | 8       | 280.000   | Silver
3           | Pham Van C     | 20      | 750.000   | Diamond
...
```

**📁 File Xuất (report_doanhthu.txt):**
```
=== BAO CAO DOANH THU ===
Ngay Xuat: 2024-12-20

TONG HOP:
- Tong Doanh Thu: 1.500.000 VND
- Tong So Hoa Don: 50
- Tong So Mon Ban: 150
- DT Trung Binh/Bill: 30.000 VND

PHAN TICH THEO KHACH:
Cust_ID | Ten       | So Bill | Tong DT | Rank
...
```

**🔄 Luồng:**
```
1. Đọc file history.txt
2. Tính các chỉ số chính
3. Phân tích từng khách (tạo CustomerRevenue array)
4. Hiển thị báo cáo trên màn hình
5. Xuất file report_doanhthu.txt
```

**⚠️ THƯỜNG THIẾU:**
- Định nghĩa chính xác "doanh thu"
- Format file báo cáo chi tiết
- Công thức tính toán từng chỉ số
- Cách phân tích theo khách

---

## 📌 PHẦN IV: KIẾN TRÚC HỆ THỐNG

### 🏗️ 5 Tầng Kiến Trúc

```
┌─────────────────────────────────────────────────┐
│  TẦNG GIAO DIỆN - UI LAYER                      │
│  welcome_ui, menu_ui, cart_ui, bill_ui, v.v.   │
└─────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────┐
│  TẦNG ĐIỀU KHIỂN - CONTROLLER LAYER            │
│  order, customer, checkout, history, report    │
└─────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────┐
│  TẦNG LOGIC - SERVICE LAYER                    │
│  file, btree, hash, cart, checkout, v.v.       │
└─────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────┐
│  TẦNG TIỆN ÍCH - UTILS LAYER                   │
│  helper.c, validator.c                         │
└─────────────────────────────────────────────────┘
                         ↓
┌─────────────────────────────────────────────────┐
│  TẦNG MODEL - MODELS LAYER                     │
│  models.h (tất cả struct & biến toàn cục)      │
└─────────────────────────────────────────────────┘
```

### 📂 Chi Tiết Từng Module

**UI Layer (7 module):**
- `welcome_ui` - Màn hình chào
- `menu_ui` - Menu món ăn
- `cart_ui` - Giỏ hàng
- `bill_ui` - Hóa đơn
- `customer_ui` - Khách hàng
- `history_ui` - Lịch sử
- `report_ui` - Báo cáo

**Controller Layer (5 module):**
- `order_controller` - Điều khiển đặt món & giỏ
- `customer_controller` - Điều khiển nhập khách
- `checkout_controller` - Điều khiển thanh toán
- `history_controller` - Điều khiển xem lịch sử
- `report_controller` - Điều khiển báo cáo

**Service Layer (7 module):**
- `file_service` - Đọc/ghi file
- `btree_service` - B-Tree khách hàng
- `hash_service` - Hash Table menu
- `cart_service` - Doubly Linked List giỏ
- `checkout_service` - Logic thanh toán
- `history_service` - Linked List lịch sử
- `report_service` - Tính toán báo cáo

**Utils Layer:**
- `helper.c/h` - Kẻ đường, ngày giờ
- `validator.c/h` - Validate đầu vào

**Models:**
- `models.h` - 10+ struct + biến toàn cục

**Database:**
- `menu.txt` - 25 món ăn
- `khachhang.txt` - Khách hàng thành viên
- `history.txt` - Lịch sử chung
- `history_khach_*.txt` - Lịch sử từng khách
- `report_doanhthu.txt` - Báo cáo xuất ra

**⚠️ THƯỜNG THIẾU:**
- Sơ đồ quan hệ giữa các module
- Chi tiết hàm chính trong mỗi service
- Dependency diagram

---

## 📌 PHẦN V: FORMAT FILE DỮ LIỆU

### 📁 menu.txt (25 dòng)
```
1|Com tam Sut Nuong|50000|1
2|Com tam Sut Hap|50000|1
3|Com tam Sut Cay|55000|1
...
25|Nuoc Chanh|5000|0
```
**Format:** `[ID]|[TenMon]|[Gia]|[HasOption]`
- HasOption: 1 = có (Cot/Suon), 0 = không

**⚠️ THƯỜNG THIẾU:** Danh sách 25 món cụ thể + giá

---

### 📁 khachhang.txt
```
1|0901234567|Tran Van A|123 Nguyen Hue, Q1|5000000|Silver
2|0912345678|Nguyen Van B|456 Le Loi, Q3|250000|Bronze
3|0933456789|Pham Van C|789 Tran Hung Dao, Q5|15000000|Diamond
```
**Format:** `[ID]|[Phone]|[Name]|[Address]|[TotalSpent]|[Rank]`

**⚠️ THƯỜNG THIẾU:** Dữ liệu mẫu 10+ khách

---

### 📁 history.txt (Lịch sử chung)
```
BILL|1|1|Tran Van A|0901234567|2024-12-20 10:30|120000|6000|114000|3
ITEM|1|Com tam Sut Nuong|2|50000|1|Them tuoi
ITEM|5|Trung OP|1|15000|0|
ITEM|12|Nuoc Mia|1|5000|0|
BILL|2|2|Nguyen Van B|0912345678|2024-12-20 11:15|250000|0|250000|5
ITEM|3|Com tam Sut Cay|1|55000|0|
...
```
**Format:**
- `BILL|[ID]|[CustomerID]|[Name]|[Phone]|[DateTime]|[Total]|[Discount]|[FinalPrice]|[ItemCount]`
- `ITEM|[ItemID]|[ItemName]|[Quantity]|[UnitPrice]|[Option]|[Note]`

**⚠️ THƯỜNG THIẾU:** Chi tiết format & ví dụ đầy đủ

---

### 📁 history_khach_X.txt (Lịch sử khách X)
- Sao chép từ history.txt, chỉ các BILL của khách X

---

### 📁 report_doanhthu.txt
```
========================================
      BAO CAO DOANH THU - QUAN COM TAM
========================================
Ngay Xuat: 2024-12-20 14:30

[1] THONG KE CHUNG:
    - Tong Doanh Thu: 1.500.000 VND
    - Tong So Hoa Don: 50
    - Tong So Mon Ban: 150
    - DT Trung Binh/Bill: 30.000 VND

[2] PHAN TICH THEO KHACH:
    Cust_ID | Ten         | So Bill | Tong DT    | Rank
    ------|-------------|---------|------------|--------
    1     | Tran Van A  | 15      | 500.000    | Gold
    2     | Nguyen Van B| 8       | 280.000    | Silver
    ...

[3] THONG KE THEM:
    - So Khach Hang: 25
    - So Khach VIP: 18 (Bronze: 5, Silver: 8, Gold: 4, Diamond: 1)
    - DT Khach VIP: 1.200.000 VND (80%)
    - DT Khach Guest: 300.000 VND (20%)

========================================
```

**⚠️ THƯỜNG THIẾU:** Cấu trúc & format cụ thể

---

## 📌 PHẦN VI: PHÂN TÍCH TÍNH PHỨC TẠP

### ⏱️ Bảng Độ Phức Tạp

| Chức Năng | Cấu Trúc | Trung Bình | Trường Hợp Tệ Nhất | Ghi Chú |
|-----------|----------|----------|-----------|-----------|
| Tra cứu món | Hash Table | **O(1)** | O(k) | k = số đụng độ |
| Tìm khách | B-Tree | **O(log n)** | O(log n) | n = số khách |
| Thêm khách | B-Tree | O(log n) | O(log n) | Tự cân bằng |
| Thêm vào giỏ | DLL | **O(1)** | O(1) | Thêm đầu |
| Xóa từ giỏ | DLL | O(k) | O(k) | k = vị trí |
| Xem lịch sử | SLL | O(m) | O(m) | m = số bill |
| Thống kê | File scan | **O(m)** | O(m) | m = số dòng |

### 💡 So Sánh Thiết Kế

| Bài Toán | Chọn | Lý Do | Độ Phức Tạp |
|----------|------|-------|-----------|
| Tra cứu menu nhanh | Hash Table | O(1), không cần sắp xếp | O(1) |
| Tìm khách theo SĐT | B-Tree | O(log n), khách tính toán cân bằng | O(log n) |
| Giỏ hàng | DLL | Thêm/xóa dễ dàng | O(1)/O(k) |
| Lịch sử | SLL | Chỉ cần duyệt, không xóa giữa | O(m) |

**⚠️ THƯỜNG THIẾU:** Bảng so sánh chi tiết này

---

## 📌 PHẦN VII: CÁC VẤN ĐỀ THIẾT KẾ & GIẢI PHÁP

### 1️⃣ Sau Khi Thanh Toán, Làm Sao Tránh Đặt Lại?
**Vấn đề:** Giỏ hàng không được xóa, khách có thể thanh toán lần 2
**Giải pháp:** Gọi `cartClear()` ngay sau khi xác nhận thanh toán thành công

### 2️⃣ Khách Mới hay Khách Cũ?
**Vấn đề:** Có nên bắt buộc phải đăng ký?
**Giải pháp:** Cho phép **Guest** (ID=0, không lưu), hoặc đăng ký thành viên

### 3️⃣ Hạng VIP Tự Động hay Thủ Công?
**Vấn đề:** Khi nào nâng hạng?
**Giải pháp:** **Tự động** khi thanh toán xong - `updateCustomerRank(customer)`

### 4️⃣ Hóa Đơn Có Thể Hủy Được Không?
**Vấn đề:** Sau khi xác nhận có hủy được?
**Giải pháp:** **Có thể hủy trước xác nhận cuối cùng**, không thể hủy sau khi in

### 5️⃣ Món Ăn Có Hết Kho Không?
**Vấn đề:** Hệ thống có kiểm tra tồn kho?
**Giải pháp:** **Không** - mô phỏng vô hạn (không có trường stock)

### 6️⃣ Lưu Lịch Sử Khi Nào?
**Vấn đề:** Nếu khách hủy thanh toán, có lưu lịch không?
**Giải pháp:** **Chỉ lưu khi xác nhận** (finalPrice > 0)

### 7️⃣ Số Điện Thoại Định Dạng Gì?
**Vấn đề:** 10 số, bắt đầu 0, hay 11 số?
**Giải pháp:** **10 chữ số, bắt đầu từ 0** (ex: 0901234567)

### 8️⃣ Khách Mới Đăng Ký Có ID Như Thế Nào?
**Vấn đề:** ID tự động hay nhập?
**Giải pháp:** **Tự động tăng** từ file khachhang.txt

### 9️⃣ Giỏ Hàng Giữa Các Chức Năng?
**Vấn đề:** Giỏ hàng dùng chung hay riêng cho từng chức năng?
**Giải pháp:** **Dùng chung** - currentBill.cart (biến toàn cục)

### 🔟 Khách Cũ Đầu Tiên Vào Khi Nào?
**Vấn đề:** Khách phải đặt hàng trước rồi mới nhập thông tin?
**Giải pháp:** **Có thể nhập thông tin trước**, sau đó đặt hàng hoặc thanh toán

**⚠️ THƯỜNG THIẾU:** Phần này tăng điểm rất nhiều!

---

## 📌 PHẦN VIII: HƯỚNG DẪN SỬ DỤNG (USER MANUAL)

### 🎮 Cách Chạy Chương Trình

**Windows:**
```bash
run.bat  # Tự động compile & chạy
```

**Hoặc Thủ Công:**
```bash
gcc -o app main.c app/**/*.c -Wall
app.exe
```

### 📱 Menu Chính
```
┌─────────────────────────────────────┐
│   HE THONG QUAN LY QUAN COM TAM     │
├─────────────────────────────────────┤
│ 1. Dat mon                          │
│ 2. Xem va chinh sua gio hang        │
│ 3. Nhap thong tin KH                │
│ 4. Thanh toan                       │
│ 5. Xem lich su                      │
│ 6. Thong ke doanh thu va xuat file  │
│ 0. Thoat                            │
└─────────────────────────────────────┘
```

### 🎯 Ví Dụ Quy Trình Đặt → Thanh Toán

**Bước 1: Đặt Món (Case 1)**
- Hiển thị menu 25 món
- Khách chọn ID 1 (Cơm tấm sườn nướng) → Số lượng 2
- Khách chọn ID 5 (Trứng ốp) → Số lượng 1
- Khách chọn 0 để thoát

**Bước 2: Xem Giỏ (Case 2)**
- Hiển thị 2 item
- Chọn xóa → xóa item 1
- Chọn sửa → item 2, sửa số lượng 2

**Bước 3: Nhập Khách (Case 3)**
- Nhập SĐT 0901234567
- Khách cũ → Hiển thị "Tran Van A" (Silver)

**Bước 4: Thanh Toán (Case 4)**
- Tổng: 100.000 VND
- Rank: Silver → Giảm 5%
- Discount: 5.000 VND
- Final: 95.000 VND
- Nhập tiền: 100.000
- Tiền thối: 5.000
- In hóa đơn → Xác nhận

**Bước 5: Lịch Sử (Case 5)**
- Xem tất cả → Thấy hóa đơn vừa mới

**Bước 6: Thống Kê (Case 6)**
- Hiển thị báo cáo
- Xuất file report_doanhthu.txt

**⚠️ THƯỜNG THIẾU:** Ví dụ cụ thể & screenshot

---

## 📌 PHẦN IX: KỸ THUẬT CÀI ĐẶT

### 🛠️ Công Nghệ & Công Cụ

| Thuộc Tính | Chi Tiết |
|-----------|----------|
| **Ngôn ngữ** | C (C99/C11) |
| **Compiler** | GCC |
| **Nền tảng** | Windows 10/11 |
| **IDE** | VS Code / Dev-C++ |
| **Thư viện** | stdio.h, stdlib.h, string.h, time.h |
| **Màu sắc** | ANSI escape codes |
| **Unicode** | Có (UTF-8) |

### 📚 Thư Viện Sử Dụng

```c
#include <stdio.h>      // Input/Output
#include <stdlib.h>     // Memory, random
#include <string.h>     // String operations
#include <time.h>       // Date/Time
#include <windows.h>    // Windows-specific (system, color)
```

### 🎨 Màu Sắc ANSI

```c
#define RED      "\x1B[31m"
#define GREEN    "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE     "\x1B[34m"
#define CYAN     "\x1B[36m"
#define WHITE    "\x1B[37m"
#define RESET    "\x1B[0m"
```

### 🔧 Biến Toàn Cục (models.h)

```c
MenuItem menu[TONG_SO_MON];           // 25 món
Customer danhSachKH[1000];            // Khách (backup)
int soLuongKH = 0;
BTreeNode* btreeRoot = NULL;          // B-Tree gốc
HistoryNode* historyHead = NULL;      // Lịch sử gốc
HashTable* menuHashTable = NULL;      // Hash Table menu
```

### ⚠️ THƯỜNG THIẾU:** Phần này giúp người đọc hiểu kỹ thuật

---

## 📌 PHẦN X: KIỂM THỬ (TESTING)

### 🧪 Test Case 1: Đặt Món

| Test | Input | Kỳ Vọng | Kết Quả |
|------|-------|---------|---------|
| Đặt 1 món chính | ID=1, Qty=2 | Thêm vào giỏ | ✅ Pass |
| Đặt > 5 chính | ID=1 Qty=3, ID=2 Qty=3 | Từ chối | ✅ Pass |
| Đặt ID không tồn tại | ID=99 | Lỗi | ✅ Pass |
| Tùy chọn Cot/Suon | ID=1, Option=1 | +10k | ✅ Pass |

### 🧪 Test Case 2: Thanh Toán

| Test | Input | Kỳ Vọng | Kết Quả |
|------|-------|---------|---------|
| Guest thanh toán | Giỏ có 100k, không login | Final=100k | ✅ Pass |
| Silver rank | Giỏ 100k, rank Silver | Discount=5k, Final=95k | ✅ Pass |
| Nâng hạng | Silver → Gold | Update rank | ✅ Pass |
| Hóa đơn lưu | Thanh toán xong | Xuất hiện lịch sử | ✅ Pass |

### 🧪 Test Case 3: Tìm Khách

| Test | Input | Kỳ Vọng | Kết Quả |
|------|-------|---------|---------|
| Khách cũ | SĐT=0901234567 | Tìm thấy | ✅ Pass |
| Khách mới | SĐT=0999999999 | Đề xuất đăng ký | ✅ Pass |
| SĐT sai | SĐT=123 | Lỗi format | ✅ Pass |

### ⚠️ THƯỜNG THIẾU:** Test case chi tiết + screenshot kết quả

---

## 📌 PHẦN XI: KẾT LUẬN & HƯỚNG PHÁT TRIỂN

### ✨ Điểm Mạnh

1. **Cấu trúc dữ liệu đa dạng** - Hash Table, B-Tree, DLL, SLL
2. **Kiến trúc MVC rõ ràng** - Dễ bảo trì, mở rộng
3. **Tính toán phức tạp** - Rank, giảm giá, phân tích
4. **Lưu trữ đa cấp** - B-Tree, File, Linked List

### ⚠️ Hạn Chế

1. **Không có database** - Chỉ lưu file text
2. **Không có UI đồ họa** - Console chỉ
3. **Không có network** - Chỉ local
4. **Dữ liệu không an toàn** - Không encryption
5. **Hiệu suất file** - Lưu file quá tần suất

### 🚀 Hướng Phát Triển Tương Lai

1. **Upgrade Database** - Dùng SQLite / MySQL
2. **Giao diện GUI** - ImGui, SFML, Qt
3. **Mobile App** - React Native / Flutter
4. **Web App** - C backend + JavaScript frontend
5. **Mã hóa dữ liệu** - Encrypt khách hàng
6. **Cloud Sync** - Lưu dữ liệu trên cloud
7. **Analytics** - Biểu đồ, dashboard
8. **Multi-user** - Mạng LAN

### ⚠️ THƯỜNG THIẾU:** Phần này rất quan trọng!

---

## 🎯 **SUMMARY - NHỮNG PHẦN THƯỜNG THIẾU NHẤT**

| # | Phần | Tầm Quan Trọng | Ghi Chú |
|----|------|--------------|--------|
| 1 | Hash Table chi tiết | ⭐⭐⭐⭐⭐ | Công thức, diagram |
| 2 | B-Tree splitChild() | ⭐⭐⭐⭐⭐ | Rất quan trọng |
| 3 | Bảng complexity | ⭐⭐⭐⭐ | So sánh O(n) |
| 4 | Format file dữ liệu | ⭐⭐⭐⭐ | Chi tiết 100% |
| 5 | Design decisions | ⭐⭐⭐⭐ | Tại sao chọn vậy |
| 6 | Test case | ⭐⭐⭐ | Ít nhất 5 case |
| 7 | User manual + screenshot | ⭐⭐⭐ | Ví dụ cụ thể |
| 8 | Validation rules | ⭐⭐⭐ | Chi tiết input |
| 9 | Flowchart | ⭐⭐⭐ | Mỗi chức năng |
| 10 | Kết luận + hướng phát triển | ⭐⭐⭐ | Tổng kết tốt |

---

## 📌 CHECKLIST CUỐI CÙNG

**Trước Khi Nộp Báo Cáo, Kiểm Tra:**

- [ ] Có tất cả **Phần I-XI**
- [ ] Hash Table có công thức hash + diagram
- [ ] B-Tree có hàm splitChild() + ví dụ
- [ ] Bảng complexity analysis đầy đủ
- [ ] Format 5 file dữ liệu chi tiết
- [ ] 10 design decision được giải thích
- [ ] Ít nhất 5 test case + kết quả
- [ ] User manual + screenshot menu
- [ ] Flowchart cho 6 chức năng
- [ ] Kết luận + 5+ hướng phát triển
- [ ] Không có typo, spelling, grammar
- [ ] Hình ảnh, diagram rõ ràng
- [ ] Tài liệu tham khảo (nếu có)
- [ ] Phụ lục: Code listings, dữ liệu mẫu

---

**Good luck! 🚀**

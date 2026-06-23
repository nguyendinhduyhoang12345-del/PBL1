# SLIDE THUYẾT TRÌNH — PBL1: ĐẶT MÓN THÔNG MINH, TỐI ƯU THANH TOÁN

---

## I. TỔNG QUAN ĐỀ TÀI

**Tên đề tài:** Đặt Món Thông Minh, Tối Ưu Thanh Toán
**Môn học:** PBL1 — Lập Trình Tính Toán
**GVHD:** PGS. TS. Trương Ngọc Châu

### Nhóm thực hiện
| Họ tên | MSSV |
|---|---|
| Nguyễn Đình Duy Hoàng | 102250222 |
| Thái Nguyễn Anh Kiệt | 1022M3343 |

### Mục tiêu
Xây dựng hệ thống quản lý quán cơm tấm bằng **ngôn ngữ C**, ứng dụng các cấu trúc dữ liệu nâng cao để xử lý đầy đủ nghiệp vụ:

| # | Chức năng |
|---|---|
| 1 | Đặt món từ thực đơn 25 món |
| 2 | Xem & chỉnh sửa giỏ hàng |
| 3 | Nhập / đăng ký thông tin khách hàng |
| 4 | Thanh toán & in hóa đơn |
| 5 | Xem lịch sử đơn hàng |
| 6 | Thống kê doanh thu & xuất báo cáo |

---

## II. CƠ SỞ LÝ THUYẾT

### Ngôn ngữ & Thư viện
- Ngôn ngữ **C** thuần (C99), không dùng thư viện ngoài
- Thư viện chuẩn: `stdio.h`, `stdlib.h`, `string.h`, `time.h`

### Các cấu trúc dữ liệu áp dụng

| Cấu trúc | Ứng dụng | Độ phức tạp |
|---|---|---|
| **B-Tree bậc 4** | Quản lý khách hàng theo SĐT | O(log n) |
| **Hash Table (chaining)** | Tra cứu món ăn theo ID | O(1) |
| **Doubly Linked List** | Quản lý giỏ hàng | O(n) |
| **Singly Linked List** | Lưu lịch sử hóa đơn | O(n) |

### Kiến trúc phần mềm
Áp dụng mô hình **MVC** (Model – View – Controller) + tầng Services:
- **Model:** định nghĩa struct trong `models.h`
- **View:** hiển thị UI trong `app/ui/`
- **Controller:** điều phối luồng trong `app/controllers/`
- **Services:** xử lý nghiệp vụ & CTDL trong `app/services/`

---

## III. TỔ CHỨC CẤU TRÚC DỮ LIỆU VÀ THUẬT TOÁN

### 1. B-Tree bậc 4 — Quản lý khách hàng
```c
typedef struct BTreeNode {
    Customer* customers[3]; // tối đa 3 khóa/node
    struct BTreeNode* children[4];
    int numKeys;
    int isLeaf;
} BTreeNode;
```
- **Khóa tìm kiếm:** số điện thoại (chuỗi)
- **Thao tác chính:** `insertBTree`, `searchBTree`, `splitChild`
- **Ưu điểm:** tự cân bằng, tìm kiếm O(log n)

### 2. Hash Table — Tra cứu thực đơn
```c
#define HASH_TABLE_SIZE 101
typedef struct HashNode {
    int key;          // ID món
    MenuItem item;
    struct HashNode* next; // xử lý đụng độ bằng chaining
} HashNode;
```
- **Hàm băm:** `key % 101`
- **Thao tác chính:** `hashTableInsert`, `hashTableSearch`
- **Ưu điểm:** tra cứu O(1) trung bình

### 3. Doubly Linked List — Giỏ hàng
```c
typedef struct CartNode {
    OrderItem item;
    struct CartNode *prev, *next;
} CartNode;
```
- Hỗ trợ thêm / xóa / cập nhật số lượng linh hoạt
- **Thao tác chính:** `cartAddItem`, `cartRemoveItemByIndex`, `cartUpdateQuantityByIndex`

### 4. Singly Linked List — Lịch sử hóa đơn
```c
typedef struct HistoryNode {
    int customerId;
    Bill bill;
    struct HistoryNode* next;
} HistoryNode;
```
- Thao tác: `addBillToHistory`, `getCustomerHistory`, `searchBillById`

### 5. Các Struct nghiệp vụ chính

| Struct | Mục đích |
|---|---|
| `MenuItem` | Món ăn trong thực đơn |
| `OrderItem` | Món đã đặt (có số lượng, ghi chú, tùy chọn) |
| `Customer` | Khách hàng (SĐT, hạng, tổng chi tiêu) |
| `Bill` | Hóa đơn (giỏ hàng + thông tin KH + giảm giá) |

---

## IV. LUỒNG HOẠT ĐỘNG

### Khởi động chương trình
```
main.c
  → loadMenuFromFile()       → menu[] → Hash Table
  → loadCustomersFromFile()  → danhSachKH[] → B-Tree
  → loadHistoryFromFile()    → HistoryNode Linked List
  → Hiển thị màn hình chào mừng
```

### Luồng 6 chức năng chính

```
[Menu Chính]
  │
  ├─ CN1: Đặt món
  │     Nhập ID → Hash Table tìm món → Chọn tùy chọn → Thêm vào Cart (DLL)
  │
  ├─ CN2: Chỉnh giỏ hàng
  │     Duyệt DLL → Xóa / Sửa số lượng
  │
  ├─ CN3: Nhập khách hàng
  │     Nhập SĐT → B-Tree tìm kiếm → Hiển thị / Đăng ký mới
  │
  ├─ CN4: Thanh toán
  │     Tính tiền → Áp dụng giảm giá theo hạng
  │     → In hóa đơn → Cập nhật B-Tree → Lưu file
  │
  ├─ CN5: Xem lịch sử
  │     Duyệt Linked List / Đọc file
  │     → Tìm theo SĐT hoặc ID hóa đơn
  │
  └─ CN6: Thống kê doanh thu
        Đọc history.txt → Phân tích → In báo cáo → Xuất file
```

### Hệ thống hạng thành viên

| Hạng | Điều kiện chi tiêu | Giảm giá |
|---|---|---|
| 🥉 Bronze | < 500.000đ | 0% |
| 🥈 Silver | ≥ 500.000đ | 10% |
| 🥇 Gold | ≥ 2.000.000đ | 20% |
| 💎 Diamond | ≥ 50.000.000đ | 30% |

---

## V. TỔ CHỨC CHƯƠNG TRÌNH

### Cấu trúc thư mục (MVC)
```
PBL1/
├── main.c                  ← Điểm vào, menu chính (0–6)
├── run.bat                 ← Script biên dịch & chạy tự động
└── app/
    ├── models/
    │   └── models.h        ← Toàn bộ struct & biến toàn cục
    ├── controllers/        ← Điều phối luồng xử lý
    │   ├── order_controller.c/h
    │   ├── customer_controller.c/h
    │   ├── checkout_controller.c/h
    │   └── history_controller.c/h
    ├── services/           ← Logic nghiệp vụ & CTDL
    │   ├── btree_service.c/h
    │   ├── hash_service.c/h
    │   ├── cart_service.c/h
    │   ├── checkout_service.c/h
    │   ├── history_service.c/h
    │   ├── report_service.c/h
    │   └── file_service.c/h
    ├── ui/                 ← Hiển thị giao diện
    │   ├── welcome_ui / menu_ui / cart_ui
    │   ├── bill_ui / history_ui / report_ui
    ├── utils/
    │   ├── helper.c/h      ← Kẻ đường, lấy ngày giờ
    │   └── validator.c/h   ← Kiểm tra đầu vào
    └── database/           ← Lưu trữ file .txt
        ├── menu.txt
        ├── khachhang.txt
        ├── history.txt
        ├── history_khach_*.txt
        └── report_doanhthu.txt
```

### Quan hệ giữa các tầng

```
UI Layer
  ↓  (hiển thị)
Controller Layer
  ↓  (gọi xử lý)
Service Layer  ←→  Utils (helper, validator)
  ↓  (đọc/ghi)
Data Layer (file .txt)
```

### Điểm nổi bật kỹ thuật
- **Không dùng thư viện ngoài** — thuần C chuẩn
- **Kết hợp 4 CTDL** trong một hệ thống thực tế
- **Lưu trữ file văn bản** có cấu trúc, hỗ trợ đọc lại sau mỗi lần chạy
- **Validate đầu vào** toàn diện qua `validator.c`
- **Giao diện ASCII** được trang trí bằng ANSI escape codes

# Quan Hệ Giữa Các Module Trong Dự Án PBL1

## 1. Tổng Quan Kiến Trúc

Dự án tuân theo **Kiến Trúc Tầng (Layered Architecture)** gồm 5 tầng:
- **Tầng Giao Diện (UI Layer)**: Hiển thị dữ liệu cho người dùng
- **Tầng Điều Khiển (Controller Layer)**: Điều phối luồng xử lý
- **Tầng Logic (Service Layer)**: Xử lý logic nghiệp vụ
- **Tầng Tiện Ích (Utils Layer)**: Hàm hỗ trợ chung
- **Tầng Dữ Liệu (Data Layer)**: Lưu trữ và quản lý dữ liệu

---

## 2. Chi Tiết Quan Hệ Giữa Các Module

### 2.1 MODELS (Nền Tảng)
**Vị trí**: `app/models/models.h`

**Chức năng**: Định nghĩa tất cả cấu trúc dữ liệu cơ bản
- `MenuItem` - Thông tin từng món ăn
- `OrderItem` - Mục trong đơn hàng
- `Customer` - Thông tin khách hàng
- `Cart` - Giỏ hàng (Linked List)
- `Bill` - Hóa đơn
- `BTreeNode` - Node của cây B-Tree (lưu khách hàng)
- `HistoryNode` - Node của Linked List (lưu lịch sử)
- `HashTable`, `HashNode` - Hash Table (lưu menu)

**Quan hệ**: 
- ✅ **Được sử dụng bởi**: TOÀN BỘ các module khác
- ✅ **Dependencies**: Không phụ thuộc vào module nào
- ✅ **Vị trí trung tâm**: Là nền tảng cho mọi module

---

### 2.2 UI LAYER (Tầng Giao Diện)
**Các Module**: `welcome_ui`, `menu_ui`, `cart_ui`, `bill_ui`, `customer_ui`, `history_ui`, `report_ui`

#### a) welcome_ui
- **Chức năng**: Hiển thị màn hình chào mừng
- **Được gọi từ**: `main.c`
- **Phụ thuộc vào**: `models.h`, `helper.h`

#### b) menu_ui
- **Chức năng**: Hiển thị danh sách món ăn
- **Được gọi từ**: `order_controller`
- **Phụ thuộc vào**: `models.h`, `helper.h`

#### c) cart_ui
- **Chức năng**: Hiển thị nội dung giỏ hàng
- **Được gọi từ**: `order_controller`
- **Phụ thuộc vào**: `models.h`, `helper.h`
- **Liên quan đến**: `cart_service` (lấy thông tin giỏ hàng)

#### d) bill_ui
- **Chức năng**: Hiển thị hóa đơn chi tiết
- **Được gọi từ**: `checkout_controller`
- **Phụ thuộc vào**: `models.h`, `helper.h`, `checkout_service`

#### e) customer_ui
- **Chức năng**: Hiển thị thông tin khách hàng
- **Được gọi từ**: `customer_controller`, `checkout_controller`
- **Phụ thuộc vào**: `models.h`, `helper.h`, `validator.h`

#### f) history_ui
- **Chức năng**: Hiển thị lịch sử mua hàng
- **Được gọi từ**: `history_controller`
- **Phụ thuộc vào**: `models.h`, `helper.h`, `history_service`

#### g) report_ui
- **Chức năng**: Hiển thị báo cáo doanh thu
- **Được gọi từ**: `report_controller`
- **Phụ thuộc vào**: `models.h`, `helper.h`, `report_service`

**Tóm tắt UI Layer**:
```
   main.c
     │
     ├──→ welcome_ui (chào mừng)
     │
   Controllers
     ├──→ order_controller ─→ menu_ui, cart_ui
     ├──→ checkout_controller ─→ bill_ui, customer_ui
     ├──→ history_controller ─→ history_ui
     ├──→ report_controller ─→ report_ui
     └──→ customer_controller ─→ customer_ui
```

---

### 2.3 CONTROLLER LAYER (Tầng Điều Khiển)
**Các Module**: `order_controller`, `checkout_controller`, `customer_controller`, `history_controller`, `report_controller`

#### a) order_controller
```c
void handleOrderMenu(Bill* currentBill);
void handleCartMenu(Bill* currentBill);
void addToCart(Bill* currentBill, OrderItem item);
void removeFromCart(Bill* currentBill);
void updateQuantity(Bill* currentBill);
```
- **Chức năng**: Điều phối quy trình đặt món và quản lý giỏ hàng
- **Phụ thuộc vào**: 
  - `models.h` (cấu trúc dữ liệu)
  - `menu_ui.h` (hiển thị menu)
  - `cart_ui.h` (hiển thị giỏ hàng)
  - `cart_service.h` (thêm, xóa, cập nhật giỏ hàng)
  - `validator.h` (kiểm tra đầu vào)
  - `helper.h` (hàm hỗ trợ)

#### b) checkout_controller
```c
void processCheckout(Bill* currentBill);
```
- **Chức năng**: Điều phối quy trình thanh toán
- **Phụ thuộc vào**:
  - `models.h`
  - `btree_service.h` (tìm/lưu khách hàng)
  - `checkout_service.h` (tính tiền, giảm giá, cập nhật rank)
  - `history_service.h` (lưu lịch sử)
  - `file_service.h` (lưu file)
  - `bill_ui.h` (hiển thị hóa đơn)
  - `customer_ui.h` (hiển thị khách hàng)

#### c) customer_controller
```c
void handleCustomer(Bill *currentBill);
```
- **Chức năng**: Xử lý nhập thông tin khách hàng
- **Phụ thuộc vào**:
  - `models.h`
  - `btree_service.h` (lưu khách hàng vào cây B-Tree)
  - `customer_ui.h` (hiển thị giao diện)
  - `validator.h` (kiểm tra đầu vào)
  - `file_service.h` (lưu vào file)

#### d) history_controller
```c
void handleViewHistory();
```
- **Chức năng**: Xử lý hiển thị lịch sử mua hàng
- **Phụ thuộc vào**:
  - `models.h`
  - `history_service.h` (lấy dữ liệu lịch sử)
  - `history_ui.h` (hiển thị lịch sử)
  - `validator.h` (kiểm tra đầu vào)

#### e) report_controller
```c
void handleGenerateReport();
```
- **Chức năng**: Xử lý tạo báo cáo doanh thu
- **Phụ thuộc vào**:
  - `models.h`
  - `report_service.h` (tính toán báo cáo)
  - `report_ui.h` (hiển thị báo cáo)
  - `file_service.h` (xuất file báo cáo)

**Mối quan hệ Controllers**:
```
All Controllers depend on:
  ├── models.h (dữ liệu chung)
  ├── Services (xử lý logic)
  ├── UI (hiển thị)
  └── Utils (hỗ trợ)

Order-dependent flow:
  order_controller → checkout_controller (thanh toán)
                  → customer_controller (lưu KH)
                  → history_controller (lưu lịch sử)
```

---

### 2.4 SERVICE LAYER (Tầng Logic)
**Các Module**: `cart_service`, `checkout_service`, `history_service`, `file_service`, `btree_service`, `hash_service`, `report_service`

#### a) cart_service
```c
void cartInit(Cart *cart);
void cartAddItem(Cart *cart, OrderItem item);
CartNode *cartGetNodeByIndex(Cart *cart, int index);
int cartRemoveItemByIndex(Cart *cart, int index);
int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity);
```
- **Cấu trúc dữ liệu**: Linked List (đôi chiều)
- **Chức năng**: Quản lý giỏ hàng
- **Được gọi bởi**: `order_controller`, `checkout_controller`
- **Phụ thuộc vào**: `models.h`

#### b) btree_service
- **Cấu trúc dữ liệu**: B-Tree (Order=4)
- **Chức năng**: Quản lý khách hàng (tìm kiếm hiệu quả)
- **Được gọi bởi**: `customer_controller`, `checkout_controller`
- **Phụ thuộc vào**: `models.h`
- **Ghi chú**: Lưu trữ khách hàng theo ID

#### c) hash_service
- **Cấu trúc dữ liệu**: Hash Table (kích thước 101)
- **Chức năng**: Quản lý menu món ăn
- **Được gọi bởi**: `order_controller`, `menu_ui`
- **Phụ thuộc vào**: `models.h`
- **Ghi chú**: Hash theo ID món ăn

#### d) checkout_service
```c
double calculateSubtotal(Bill* bill);
double getDiscountPercent(char* rank);
void updateCustomerRank(Customer* cust);
```
- **Chức năng**: Tính toán tiền, giảm giá, cập nhật rank khách hàng
- **Hệ thống rank**: Bronze → Silver → Gold → Diamond
- **Được gọi bởi**: `checkout_controller`
- **Phụ thuộc vào**: `models.h`

#### e) history_service
```c
int addBillToHistory(int customerId, Bill* bill);
HistoryNode* getCustomerHistory(int customerId);
int getCustomerIdByPhone(char* phone);
HistoryNode* searchBillById(int billId);
void freeHistoryList(HistoryNode* head);
```
- **Cấu trúc dữ liệu**: Linked List
- **Chức năng**: Quản lý lịch sử hóa đơn (trong bộ nhớ)
- **Được gọi bởi**: `checkout_controller`, `history_controller`, `report_controller`
- **Phụ thuộc vào**: `models.h`

#### f) file_service
```c
void loadMenuFromFile(const char* filename);
void loadCustomersFromFile(BTreeNode** root);
void saveAllCustomersToFile();
void saveBillToHistoryFile(int customerId, Bill* bill);
void saveBillToGlobalFile(Bill* bill, int customerId);
void loadHistoryFromFile();
```
- **Chức năng**: Đọc/ghi dữ liệu từ file
- **Được gọi bởi**: 
  - `main.c` (khởi tạo: load menu, khách, lịch sử)
  - `checkout_controller` (lưu hóa đơn)
  - `customer_controller` (lưu khách hàng)
  - `report_controller` (xuất báo cáo)
- **Phụ thuộc vào**: `models.h`, `btree_service`, `history_service`
- **Files liên quan**:
  - `app/database/menu.txt` (danh sách món)
  - `app/database/khachhang.txt` (danh sách khách)
  - `app/database/history.txt` (tất cả hóa đơn)
  - `app/database/history_khach_*.txt` (hóa đơn từng khách)
  - `app/database/report_doanhthu.txt` (báo cáo)

#### g) report_service
```c
double calculateTotalRevenue();
int calculateTotalBills();
int calculateTotalItems();
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount);
```
- **Chức năng**: Tính toán và phân tích thống kê
- **Được gọi bởi**: `report_controller`
- **Phụ thuộc vào**: `models.h`, `history_service`
- **Dữ liệu nguồn**: Từ `historyHead` (linked list lịch sử)

**Sơ đồ Service Layer**:
```
main.c
  ├──→ file_service (load dữ liệu ban đầu)
  │     ├──→ btree_service (tạo cây khách)
  │     ├──→ hash_service (tạo hash menu)
  │     └──→ history_service (load lịch sử)
  │
Controllers
  ├──→ order_controller
  │     ├──→ cart_service (quản lý giỏ)
  │     └──→ hash_service (lấy menu)
  │
  ├──→ checkout_controller
  │     ├──→ cart_service
  │     ├──→ btree_service (tìm KH)
  │     ├──→ checkout_service (tính tiền)
  │     ├──→ history_service (lưu lịch sử)
  │     └──→ file_service (lưu file)
  │
  ├──→ customer_controller
  │     ├──→ btree_service (lưu KH mới)
  │     └──→ file_service (lưu file)
  │
  ├──→ history_controller
  │     ├──→ history_service (lấy dữ liệu)
  │     └──→ file_service (load từ file)
  │
  └──→ report_controller
        ├──→ report_service (tính toán)
        ├──→ history_service (lấy dữ liệu)
        └──→ file_service (xuất file)
```

---

### 2.5 UTILS LAYER (Tầng Tiện Ích)
**Các Module**: `helper.h`, `validator.h`

#### a) helper.h
```c
const char* getRankColor(const char *rank);
void printLine();
void getCurrentDateTime(char* buffer, int maxSize);
```
- **Chức năng**: Hỗ trợ hiển thị, xử lý màu sắc, thời gian
- **Được gọi bởi**: UI Layer, Controllers
- **Phụ thuộc vào**: Thư viện chuẩn C

#### b) validator.h
```c
void clearBuffer();
int getValidInt(const char* prompt);
double getValidDouble(const char* prompt);
int isDigits(const char* s);
void trimString(char* s);
void getValidPhoneNumber(char* phone, int maxLen, const char* prompt);
void getValidLine(char* buffer, int maxLen, const char* prompt);
```
- **Chức năng**: Kiểm tra và xử lý đầu vào người dùng
- **Được gọi bởi**: Controllers, UI
- **Phụ thuộc vào**: Thư viện chuẩn C

**Quan hệ Utils**:
```
Utils Layer
├── helper.h (hỗ trợ hiển thị)
│    ↑ được sử dụng bởi: Tất cả UI modules
│
└── validator.h (hỗ trợ nhập liệu)
     ↑ được sử dụng bởi: Controllers, UI
```

---

## 3. Luồng Xử Lý Chính

### 3.1 Luồng Đặt Món & Thanh Toán
```
main.c
  │
  ├─→ [1] handleOrderMenu(bill) [order_controller]
  │     ├─→ showMenuUI() [menu_ui]
  │     │    └─→ hash_service (lấy menu)
  │     │
  │     └─→ addToCart(bill, item) [order_controller]
  │          └─→ cartAddItem() [cart_service]
  │
  ├─→ [2] handleCartMenu(bill) [order_controller]
  │     ├─→ showCartUI(bill) [cart_ui]
  │     ├─→ removeFromCart() [order_controller]
  │     │    └─→ cartRemoveItemByIndex() [cart_service]
  │     │
  │     └─→ updateQuantity() [order_controller]
  │          └─→ cartUpdateQuantityByIndex() [cart_service]
  │
  ├─→ [3] handleCustomer(bill) [customer_controller]
  │     ├─→ showCustomerUI() [customer_ui]
  │     ├─→ insertCustomerBTree() [btree_service]
  │     └─→ saveAllCustomersToFile() [file_service]
  │
  └─→ [4] processCheckout(bill) [checkout_controller]
        ├─→ showBillUI() [bill_ui]
        ├─→ calculateSubtotal() [checkout_service]
        ├─→ getDiscountPercent() [checkout_service]
        ├─→ updateCustomerRank() [checkout_service]
        ├─→ addBillToHistory() [history_service]
        ├─→ saveBillToHistoryFile() [file_service]
        ├─→ saveBillToGlobalFile() [file_service]
        └─→ saveBillToFile() [file_service]
```

### 3.2 Luồng Xem Lịch Sử
```
main.c
  │
  └─→ [5] handleViewHistory() [history_controller]
        ├─→ getCustomerIdByPhone() [history_service]
        ├─→ getCustomerHistory() [history_service]
        └─→ showHistoryUI() [history_ui]
```

### 3.3 Luồng Báo Cáo Doanh Thu
```
main.c
  │
  └─→ [6] handleGenerateReport() [report_controller]
        ├─→ calculateTotalRevenue() [report_service]
        ├─→ calculateTotalBills() [report_service]
        ├─→ calculateTotalItems() [report_service]
        ├─→ analyzeByCustomer() [report_service]
        ├─→ showReportUI() [report_ui]
        └─→ exportReportToFile() [file_service]
```

---

## 4. Các Cấu Trúc Dữ Liệu Chính

### 4.1 Biến Toàn Cục
```c
extern MenuItem menu[TONG_SO_MON];              // Array 25 phần tử
extern Customer danhSachKH[1000];               // Array 1000 khách
extern int soLuongKH;                           // Số lượng khách
extern BTreeNode* btreeRoot;                    // Gốc cây B-Tree
extern HistoryNode* historyHead;                // Đầu linked list lịch sử
extern HashTable* menuHashTable;                // Hash table menu
```

### 4.2 Quan Hệ Giữa Các Cấu Trúc
```
Menu (MenuItem array)
  ├─ Được load từ: file_service.loadMenuFromFile()
  ├─ Được lưu vào: hash_service (HashTable)
  └─ Được sử dụng bởi: order_controller

Customer (BTreeNode - cây B-Tree)
  ├─ Được load từ: file_service.loadCustomersFromFile()
  ├─ Được thêm bởi: customer_controller → btree_service
  ├─ Được lưu bởi: file_service.saveAllCustomersToFile()
  └─ Được tìm kiếm bởi: checkout_controller → btree_service

Cart (Linked List đôi chiều)
  ├─ Được tạo bởi: order_controller.cartInit()
  ├─ Được thao tác bởi: order_controller → cart_service
  └─ Được sử dụng trong: Bill

Bill (chứa Cart + Customer info)
  ├─ Được tạo bởi: order_controller
  ├─ Được tính tiền bởi: checkout_service
  ├─ Được lưu vào lịch sử: history_service → file_service
  └─ Được thêm vào History Linked List

History (Linked List)
  ├─ Được load từ: file_service.loadHistoryFromFile()
  ├─ Được thêm bởi: checkout_controller → history_service
  ├─ Được lưu bởi: file_service.saveBillToHistoryFile()
  └─ Được sử dụng bởi: history_controller, report_controller
```

---

## 5. Bảng Tóm Tắt Phụ Thuộc Lẫn Nhau

| Module | Phụ Thuộc Vào | Được Gọi Bởi |
|--------|----------------|--------------|
| main.c | Controllers, file_service | - |
| order_controller | models, menu_ui, cart_ui, cart_service, validator, helper | main |
| checkout_controller | models, btree_service, checkout_service, history_service, file_service, bill_ui, customer_ui | main |
| customer_controller | models, btree_service, customer_ui, validator, file_service | main |
| history_controller | models, history_service, history_ui, validator | main |
| report_controller | models, report_service, report_ui, file_service | main |
| menu_ui | models, helper | order_controller |
| cart_ui | models, helper | order_controller |
| bill_ui | models, helper, checkout_service | checkout_controller |
| customer_ui | models, helper, validator | customer_controller, checkout_controller |
| history_ui | models, helper | history_controller |
| report_ui | models, helper | report_controller |
| welcome_ui | models, helper | main |
| cart_service | models | order_controller, checkout_controller |
| btree_service | models | customer_controller, checkout_controller, file_service |
| hash_service | models | file_service, order_controller |
| checkout_service | models | checkout_controller |
| history_service | models | checkout_controller, history_controller, report_controller, file_service |
| file_service | models, btree_service, history_service | main, checkout_controller, customer_controller, history_controller, report_controller |
| report_service | models, history_service | report_controller |
| helper | - | UI modules, Controllers |
| validator | - | Controllers, UI modules |

---

## 6. Kiến Trúc Tổng Quát

```
┌─────────────────────────────────────────────────────────┐
│                    PRESENTATION LAYER                   │
│  welcome_ui │ menu_ui │ cart_ui │ bill_ui │ customer_ui │
│  history_ui │ report_ui                                 │
└────────────────────────┬────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────┐
│                  CONTROLLER LAYER                       │
│  order_controller │ checkout_controller │ customer_controller│
│  history_controller │ report_controller                 │
└────────────────────────┬────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────┐
│                    SERVICE LAYER                        │
│  cart_service │ checkout_service │ history_service      │
│  btree_service │ hash_service │ report_service          │
│  file_service                                           │
└────────────────────────┬────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────┐
│                    UTILS LAYER                          │
│           helper.c │ validator.c                        │
└────────────────────────┬────────────────────────────────┘
                         │
┌────────────────────────▼────────────────────────────────┐
│                    DATA LAYER                           │
│  menu.txt │ khachhang.txt │ history.txt                 │
│  history_khach_*.txt │ report_doanhthu.txt              │
└─────────────────────────────────────────────────────────┘
```

---

## 7. Nhận Xét Về Thiết Kế

### Điểm Mạnh
✅ **Tách biệt rõ ràng**: UI, Controller, Service, Utils, Data tách biệt
✅ **Tái sử dụng**: Các service có thể được gọi bởi nhiều controller
✅ **Cấu trúc dữ liệu đa dạng**: Sử dụng B-Tree, Linked List, Hash Table
✅ **Quản lý dữ liệu toàn diện**: Lưu vào file và bộ nhớ

### Có Thể Cải Thiện
⚠️ **Biến toàn cục**: Sử dụng nhiều biến global (menu, danhSachKH, btreeRoot...)
⚠️ **Phụ thuộc lẫn nhau**: file_service phụ thuộc vào btree_service và history_service
⚠️ **Error handling**: Có thể cần thêm kiểm tra lỗi chi tiết hơn

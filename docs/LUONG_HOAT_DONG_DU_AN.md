# HỆ THỐNG QUẢN LÝ QUÁN CÁM TẠM - LUỒNG HOẠT ĐỘNG TỔNG QUÁT

## I. TỔNG QUAN DỰ ÁN

**Tên dự án:** Hệ thống quản lý quán cơm tạm  
**Mục đích:** Quản lý menu, đơn hàng, khách hàng, thanh toán, lịch sử bán hàng, và thống kê doanh thu  
**Ngôn ngữ:** C  
**Kiến trúc:** MVC (Model - View - Controller) + Services

### Cấu trúc thư mục:
```
- main.c                    (Điểm vào chính)
- app/
  - models/                 (Định nghĩa cấu trúc dữ liệu)
  - controllers/            (Xử lý logic các chức năng)
  - services/               (Xử lý nghiệp vụ, cấu trúc dữ liệu)
  - ui/                     (Giao diện người dùng)
  - utils/                  (Hàm tiện ích)
  - database/               (Lưu trữ dữ liệu)
```

---

## II. CẤU TRÚC DỮ LIỆU CHÍNH

### 1. **MenuItem** (Mục thực đơn)
```c
typedef struct {
    int id;              // ID menu (1-25)
    char name[50];       // Tên món ăn
    double price;        // Giá tiền
    int hasOptions;      // Có tuỳ chọn hay không (cot/suon)
} MenuItem;
```
**Lưu trữ:** Hash Table (kích thước 101)  
**Tệp:** `app/database/menu.txt`

### 2. **Customer** (Khách hàng)
```c
typedef struct {
    int id;              // ID khách hàng
    char phone[15];      // Số điện thoại (khóa tìm kiếm)
    char name[50];       // Tên khách hàng
    char address[100];   // Địa chỉ
    double totalSpent;   // Tổng tiền đã chi tiêu
    char rank[20];       // Hạng (Bronze/Silver/Gold/Diamond)
} Customer;
```
**Lưu trữ:** B-Tree (cấp 4)  
**Tệp:** `app/database/khachhang.txt`  
**Hạng VIP:**
- Bronze: < 1.000.000 VND → Giảm 0%
- Silver: 1.000.000 - 5.000.000 VND → Giảm 5%
- Gold: 5.000.000 - 10.000.000 VND → Giảm 10%
- Diamond: ≥ 10.000.000 VND → Giảm 15%

### 3. **OrderItem** (Mục hóa đơn)
```c
typedef struct {
    int id;              // ID món ăn
    char name[50];       // Tên món
    double price;        // Giá tiền (sau tuỳ chọn)
    int quantity;        // Số lượng
    int option;          // Tuỳ chọn (1=Cot, 2=Suon)
    char note[100];      // Ghi chú
    double totalPrice;   // Tổng (price * quantity)
} OrderItem;
```

### 4. **Cart** (Giỏ hàng)
```c
typedef struct {
    CartNode *head;      // Nút đầu (Doubly Linked List)
    CartNode *tail;      // Nút cuối
    int itemCount;       // Số mục
} Cart;

typedef struct CartNode {
    OrderItem item;
    struct CartNode *prev;
    struct CartNode *next;
} CartNode;
```
**Lưu trữ:** Doubly Linked List (Danh sách liên kết đôi)

### 5. **Bill** (Hóa đơn)
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

### 6. **HistoryNode** (Nút lịch sử)
```c
typedef struct HistoryNode {
    int customerId;      // ID khách hàng
    Bill bill;           // Thông tin hóa đơn
    struct HistoryNode* next;
} HistoryNode;
```
**Lưu trữ:** Singly Linked List (Danh sách liên kết đơn)  
**Tệp:** `app/database/history.txt`

---

## III. SỔ ĐỒ LUỒNG CHƯƠNG TRÌNH

```
┌─────────────────────────────────────────┐
│     KHỞI ĐỘNG HỆ THỐNG (main.c)         │
├─────────────────────────────────────────┤
│ 1. Nạp menu từ file (Hash Table)       │
│ 2. Nạp khách hàng từ file (B-Tree)    │
│ 3. Nạp lịch sử từ file (Linked List)  │
│ 4. Hiển thị màn hình chào mừng         │
└───────────────┬─────────────────────────┘
                │
         ┌──────▼──────┐
         │ MENU CHÍNH  │
         └──────┬──────┘
         ┌──────┴──────────────────────┐
         │                             │
         ▼                             ▼
    ┌──────────────┐         ┌────────────────────────┐
    │ 1. Đặt Món   │ ... 6   │ 6. Báo Cáo & Xuất File │
    └──────────────┘         └────────────────────────┘
```

---

## IV. CHI TIẾT 6 CHỨC NĂNG CHÍNH

---

### **CHỨC NĂNG 1: ĐẶT MÓN** (case 1)

#### Mô tả:
Cho phép khách hàng xem menu và thêm các món ăn vào giỏ hàng. Có giới hạn:
- Món chính: Tối đa 5 cái
- Món thêm & Nước: Không giới hạn

#### Hàm chính:
- **`handleOrderMenu(Bill* currentBill)`** - Controller
  - Gọi `showMenuUI()` - Hiển thị danh sách menu từ Hash Table
  - Vòng lặp nhập đơn:
    - Tìm món bằng `hashTableSearch(menuHashTable, idNhap)`
    - Xác thực số lượng
    - Xử lý tuỳ chọn (Cot let / Suon cay +10k)
    - Nhập ghi chú
    - Tạo `OrderItem` và gọi `addToCart()`

#### Hàm phụ trợ:
- **`addToCart(Bill *currentBill, OrderItem item)`**
  - Nếu món đã tồn tại (cùng ID, tuỳ chọn, ghi chú): Cộng số lượng
  - Nếu chưa: Gọi `cartAddItem()` để thêm mới

#### Cấu trúc dữ liệu sử dụng:
| Tên | Loại | Chức năng |
|-----|------|----------|
| `menuHashTable` | Hash Table | Tìm kiếm menu O(1) |
| `currentBill->cart` | Doubly Linked List | Lưu các món đã chọn |
| `menu[]` | Array | Lưu toàn bộ menu trong RAM |

#### Luồng chi tiết:
```
1. Hiển thị menu (từ hash table)
2. Lặp: Nhập ID món
3. Tìm món trong hash table → Kiểm tra hợp lệ
4. Kiểm tra số lượng (xem đã đạt tối đa 5 mon chinh?)
5. Nhập số lượng → Xác thực
6. Nếu món có tuỳ chọn:
   - Hiển thị: 1. Cot let | 2. Suon cay (+10k)
   - Tính giá thực tế
7. Nhập ghi chú
8. Tạo OrderItem → addToCart()
9. Hiển thị thêm thành công
10. Lặp lại bước 2 đến khi nhập 0
```

#### File liên quan:
- [order_controller.c](app/controllers/order_controller.c#L1)
- [hash_service.c](app/services/hash_service.c#L1)
- [cart_service.c](app/services/cart_service.c#L1)
- [menu_ui.c](app/ui/menu_ui.c)

---

### **CHỨC NĂNG 2: XEM VÀ CHỈNH SỬA GIỎ HÀNG** (case 2)

#### Mô tả:
Hiển thị giỏ hàng hiện tại và cho phép xóa hoặc thay đổi số lượng các món đã chọn.

#### Hàm chính:
- **`handleCartMenu(Bill* currentBill)`** - Controller
  - Vòng lặp con:
    - Gọi `showCartUI(currentBill)` - Hiển thị giỏ hàng
    - Menu chọn: 1. Xóa | 2. Sửa số lượng | 0. Quay lại
    - Nếu chọn 1: `removeFromCart()`
    - Nếu chọn 2: `updateQuantity()`

#### Hàm phụ trợ:

**1. `removeFromCart(Bill *currentBill)`**
- Nhập STT món cần xóa (1 đến itemCount)
- Xác thực STT
- Gọi `cartRemoveItemByIndex()` để xóa từ linked list
- Giải phóng bộ nhớ của node

**2. `updateQuantity(Bill *currentBill)`**
- Nhập STT món cần sửa
- Nhập số lượng mới (> 0)
- **Kiểm tra ràng buộc:** Nếu là món chính (ID ≤ 12):
  - Tính tổng số mon chinh hiện tại (trừ cái cũ, cộng cái mới)
  - Nếu vượt 5 → Báo lỗi
- Gọi `cartUpdateQuantityByIndex()` để cập nhật
- Cập nhật `totalPrice = price * quantity`

#### Cấu trúc dữ liệu sử dụng:
| Tên | Loại | Chức năng |
|-----|------|----------|
| `currentBill->cart` | Doubly Linked List | Duyệt, xóa, cập nhật |
| `CartNode` | Struct | Lưu từng mục + liên kết |

#### Luồng chi tiết:
```
1. Hiển thị giỏ hàng:
   - Duyệt linked list từ head đến tail
   - Hiển thị STT, tên, giá, số lượng, tổng tiền
   - Hiển thị tổng cộng
2. Menu con:
   a) Nếu chọn Xóa:
      - Nhập STT (1-itemCount)
      - Tìm node tại index → cartGetNodeByIndex()
      - Cập nhật linked list (prev->next, next->prev)
      - Giải phóng node
      - Giảm itemCount
   
   b) Nếu chọn Sửa:
      - Nhập STT
      - Nhập số lượng mới
      - Nếu mon chinh: Kiểm tra tổng ≤ 5
      - Cập nhật node->item.quantity
      - Tính lại totalPrice
   
   c) Nếu chọn 0: Thoát vòng lặp
```

#### File liên quan:
- [order_controller.c](app/controllers/order_controller.c#L180)
- [cart_service.c](app/services/cart_service.c#L1)
- [cart_ui.c](app/ui/cart_ui.c)

---

### **CHỨC NĂNG 3: NHẬP THÔNG TIN KHÁCH HÀNG** (case 3)

#### Mô tả:
Tìm kiếm khách hàng theo số điện thoại. Nếu khách hàng tồn tại: Hiển thị thông tin & hạng VIP. Nếu chưa: Cho phép đăng ký mới.

#### Hàm chính:
- **`handleCustomer(Bill *currentBill)`** - Controller
  - Nhập số điện thoại → Xác thực format
  - Gọi `searchBTree(btreeRoot, sdt)` - Tìm kiếm O(log n)
  - **Trường hợp 1 - Khách cũ:**
    - Gọi `showCustomerInfo()` - Hiển thị thông tin
    - Gán `currentBill->customer = *found`
  - **Trường hợp 2 - Khách mới:**
    - Menu chọn: 1. Đăng ký | 0. Tiếp tục (Guest)
    - Nếu chọn 1:
      - Nhập tên, địa chỉ
      - Khởi tạo: ID = soLuongKH+1, totalSpent = 0, rank = Bronze
      - Gọi `insertBTree()` để thêm vào B-Tree
      - Gọi `saveAllCustomersToFile()` - Lưu vào `khachhang.txt`

#### Cấu trúc dữ liệu sử dụng:
| Tên | Loại | Chức năng |
|-----|------|----------|
| `btreeRoot` | B-Tree (cấp 4) | Tìm khách hàng theo SDT O(log n) |
| `danhSachKH[]` | Array | Lưu tất cả khách hàng |
| `soLuongKH` | Int | Đếm số khách hàng |

#### Luồng chi tiết:
```
1. Nhập số điện thoại (xác thực format)
2. Tìm kiếm trong B-Tree:
   - searchBTree(btreeRoot, sdt)
   - So sánh từng node (O(log n))
   
3. Nếu tìm thấy (found != NULL):
   - Hiển thị: ID, Tên, Địa chỉ, Tổng chi tiêu, Hạng VIP
   - Gán vào currentBill->customer
   
4. Nếu không tìm thấy:
   - Menu: 1. Đăng ký mới | 0. Tiếp tục (Guest)
   a) Chọn 1 - Đăng ký:
      - Nhập tên khách hàng
      - Nhập địa chỉ
      - Tạo Customer mới:
        * id = soLuongKH + 1
        * phone = sdt đã nhập
        * totalSpent = 0.0
        * rank = "Bronze"
      - Lưu vào danhSachKH[soLuongKH]
      - insertBTree(&btreeRoot, &customer) → Thêm vào B-Tree
      - soLuongKH++
      - saveAllCustomersToFile() → Lưu file
      - Hiển thị xác nhận đăng ký thành công
   
   b) Chọn 0 - Tiếp tục:
      - currentBill->customer trống (Guest)
      - Quay lại menu chính
```

#### B-Tree Chi tiết:
**Định nghĩa:** B-Tree cấp 4 (BTREE_ORDER = 4)
- Mỗi node chứa tối đa 3 khóa
- Mỗi node nội bộ có tối đa 4 con
- Tìm kiếm: So sánh SDT theo thứ tự từ điển

**Các hàm B-Tree:**
- `createBTreeNode()` - Tạo node mới
- `insertBTree()` - Thêm customer vào B-Tree (tự động cân bằng)
- `searchBTree()` - Tìm kiếm customer theo SDT
- `splitChild()` - Chia node khi đầy

#### File liên quan:
- [customer_controller.c](app/controllers/customer_controller.c#L1)
- [btree_service.c](app/services/btree_service.c#L1)
- [file_service.c](app/services/file_service.c)
- [customer_ui.c](app/ui/customer_ui.c)

---

### **CHỨC NĂNG 4: THANH TOÁN** (case 4)

#### Mô tả:
Xử lý thanh toán hóa đơn, áp dụng giảm giá theo hạng VIP, cập nhật thông tin khách hàng, lưu lịch sử và dọn dẹp giỏ hàng.

#### Hàm chính:
- **`processCheckout(Bill* currentBill)`** - Controller
  - Kiểm tra giỏ hàng rỗng
  - Tính tổng tiền: `calculateSubtotal()`
  - Xác định khách hàng & hạng VIP
  - Tính giảm giá: `getDiscountPercent(rank)`
  - Hiển thị chi tiết thanh toán
  - Nhập tiền khách đưa (xác thực)
  - In hóa đơn: `printBillUI()`
  - Xác nhận thanh toán
  - Nếu xác nhận:
    - Cập nhật customer nếu là khách cũ
    - Kiểm tra nâng hạng
    - Lưu khách hàng: `saveAllCustomersToFile()`
    - Lưu hóa đơn toàn bộ: `saveBillToGlobalFile()`
    - Lưu lịch sử khách: `saveBillToHistoryFile()` + `addBillToHistory()`
    - Dọn giỏ hàng

#### Hàm phụ trợ:

**1. `calculateSubtotal(Bill* bill)` - checkout_service.c**
```c
double calculateSubtotal(Bill* bill) {
    double total = 0.0;
    CartNode* node = bill->cart.head;
    while(node) {
        total += node->item.totalPrice;  // price * quantity
        node = node->next;
    }
    return total;
}
```

**2. `getDiscountPercent(char* rank)` - checkout_service.c**
- Bronze (< 1M): 0%
- Silver (1M - 5M): 5%
- Gold (5M - 10M): 10%
- Diamond (≥ 10M): 15%

**3. `updateCustomerRank(Customer* cust)` - checkout_service.c**
```c
void updateCustomerRank(Customer* cust) {
    if (cust->totalSpent >= 10000000) strcpy(cust->rank, "Diamond");
    else if (cust->totalSpent >= 5000000) strcpy(cust->rank, "Gold");
    else if (cust->totalSpent >= 1000000) strcpy(cust->rank, "Silver");
    else strcpy(cust->rank, "Bronze");
}
```

**4. `saveAllCustomersToFile()` - file_service.c**
- Duyệt B-Tree → Lấy tất cả customer
- Ghi vào `app/database/khachhang.txt`

**5. `saveBillToGlobalFile(Bill* bill, int customerId)` - file_service.c**
- Định dạng: `BILL|ID|CustomerID|Name|Phone|DateTime|Total|Discount|FinalPrice|ItemCount`
- Ghi từng item: `ITEM|ID|Name|Price|Qty|Option|Note`
- Lưu vào `app/database/history.txt`

**6. `saveBillToHistoryFile(int customerId, Bill* bill)` - file_service.c**
- Lưu vào file riêng: `app/database/history_khach_X.txt`

**7. `addBillToHistory(int customerId, Bill* bill)` - history_service.c**
- Thêm vào linked list lịch sử (bộ nhớ)

#### Cấu trúc dữ liệu sử dụng:
| Tên | Loại | Chức năng |
|-----|------|----------|
| `currentBill->cart` | Doubly Linked List | Duyệt tính tổng |
| `btreeRoot` | B-Tree | Tìm khách hàng cũ |
| `historyHead` | Singly Linked List | Lưu lịch sử |

#### Luồng chi tiết:
```
1. Kiểm tra:
   - Nếu giỏ hàng rỗng → Báo lỗi & thoát
   
2. Tính toán:
   - subtotal = Σ(price * quantity) từ giỏ hàng
   - Tìm khách hàng: searchBTree(btreeRoot, phone)
   - Nếu tìm thấy:
     * discountRate = getDiscountPercent(rank)
     * discountAmount = subtotal * discountRate
     * finalPrice = subtotal - discountAmount
   - Nếu không (Guest):
     * discountRate = 0
     * discountAmount = 0
     * finalPrice = subtotal
   
3. Hiển thị:
   - Tổng tiền ban đầu: subtotal
   - Hạng khách: rank + % giảm
   - Tiền giảm giá
   - Tổng tiền cần trả: finalPrice
   - Hiển thị chi tiết từng món (printBillUI)
   
4. Nhập tiền:
   - Yêu cầu nhập số tiền khách dua
   - Nếu < finalPrice → Báo lỗi & nhập lại
   - Tính tiền thừa = tiền dưa - finalPrice
   
5. In hóa đơn:
   - printBillUI(currentBill, customerMoney, tienThua)
   
6. Xác nhận:
   - Hiển thị: "Xac nhan thanh toan (1: Dong y / 0: Huy bo): "
   
7. Nếu xác nhận (xacNhan == 1):
   a) Cập nhật khách hàng cũ (nếu khGoc != NULL):
      - Lưu thông tin cũ (oldTotal, oldRank)
      - khGoc->totalSpent += finalPrice
      - updateCustomerRank(khGoc) → Kiểm tra nâng hạng
      - Gán dữ liệu vào currentBill->customer
      - saveAllCustomersToFile() → Ghi file
      - Hiển thị: Tổng chi tiêu cũ/mới, hạng mới
      - Nếu có nâng hạng → Chúc mừng
   
   b) Nếu khách mới (Guest):
      - currentBill->customer.id = 0
      - strcpy(rank, "Guest")
      - totalSpent = finalPrice
   
   c) Lưu hóa đơn:
      - saveBillToGlobalFile() → Lưu tập trung
      - Nếu khách đã đăng ký:
        * addBillToHistory() → Thêm vào linked list
        * saveBillToHistoryFile() → Lưu file riêng
   
   d) Dọn dẹp:
      - currentBill->cart.head = NULL
      - currentBill->cart.tail = NULL
      - currentBill->cart.itemCount = 0
      - Xóa thông tin khách (memset)
   
8. Nếu hủy (xacNhan == 0):
   - Hiển thị "Da huy thanh toan"
   - Quay lại menu
```

#### File liên quan:
- [checkout_controller.c](app/controllers/checkout_controller.c#L1)
- [checkout_service.c](app/services/checkout_service.c)
- [file_service.c](app/services/file_service.c)
- [history_service.c](app/services/history_service.c)
- [bill_ui.c](app/ui/bill_ui.c)

---

### **CHỨC NĂNG 5: XEM LỊCH SỬ** (case 5)

#### Mô tả:
Xem lịch sử bán hàng của quán. Có thể xem:
- Tất cả hóa đơn (mặc định)
- Lịch sử của khách hàng theo số điện thoại
- Chi tiết hóa đơn theo ID

#### Hàm chính:
- **`handleViewHistory()`** - Controller
  - Gọi `printAllHistory()` - Hiển thị tất cả hóa đơn từ file
  - Menu con:
    - 1: Tìm lịch sử khách theo SDT → `getCustomerIdByPhone()` → `getCustomerHistory()` → `printCustomerHistory()`
    - 2: Tìm hóa đơn theo ID → `searchBillById()` → `printBillDetail()`
    - 0: Quay lại

#### Hàm phụ trợ:

**1. `printAllHistory()` - history_service.c**
- Duyệt linked list `historyHead` từ đầu đến cuối
- In từng bill: ID, Khách, SDT, Tổng tiền, Ngày giờ

**2. `getCustomerIdByPhone(char* phone)` - history_service.c**
- Duyệt linked list
- So sánh phone của từng bill
- Trả về customerId nếu tìm thấy, -1 nếu không

**3. `getCustomerHistory(int customerId)` - history_service.c**
- Duyệt linked list
- Lọc các bill có customerId trùng khớp
- Trả về linked list con chỉ chứa bill của khách này

**4. `searchBillById(int billId)` - history_service.c**
- Duyệt linked list
- So sánh billId
- Trả về node nếu tìm thấy, NULL nếu không

**5. `printBillDetail(HistoryNode* billNode)` - history_ui.c**
- In chi tiết hóa đơn: ID, Khách, SDT, Ngày giờ
- Duyệt từng item trong cart
- In: STT, Tên, Giá, Số lượng, Ghi chú, Tổng
- In tổng cộng, giảm giá, tổng tiền

#### Cấu trúc dữ liệu sử dụng:
| Tên | Loại | Chức năng |
|-----|------|----------|
| `historyHead` | Singly Linked List | Lưu tất cả hóa đơn |
| `HistoryNode` | Struct | Chứa bill + con trỏ next |

#### Luồng chi tiết:
```
1. Hiển thị tất cả hóa đơn (mặc định):
   - printAllHistory()
   - Duyệt historyHead từ đầu
   - In bảng: ID | Khách | SDT | Tổng | Ngày giờ
   
2. Menu con:
   a) Chọn 1 - Tìm lịch sử khách:
      - Nhập SDT khách hàng
      - customerId = getCustomerIdByPhone(phone)
      - Nếu không tìm thấy (-1):
        * Báo: "Khong tim thay khach hang"
        * Quay lại menu
      - Nếu tìm thấy:
        * currentHistory = getCustomerHistory(customerId)
        * printCustomerHistory(currentHistory)
        * Hiển thị tất cả hóa đơn của khách
   
   b) Chọn 2 - Tìm hóa đơn theo ID:
      - Nhập billId
      - billNode = searchBillById(billId)
      - Nếu không tìm thấy (NULL):
        * Báo: "Khong tim thay bill"
      - Nếu tìm thấy:
        * printBillDetail(billNode)
        * In chi tiết: Khách, Các món, Tổng tiền, v.v
   
   c) Chọn 0 - Quay lại:
      - Thoát vòng lặp
```

#### File liên quan:
- [history_controller.c](app/controllers/history_controller.c#L1)
- [history_service.c](app/services/history_service.c)
- [history_ui.c](app/ui/history_ui.c)

---

### **CHỨC NĂNG 6: THỐNG KÊ DOANH THU & XUẤT FILE BÁO CÁO** (case 6)

#### Mô tả:
Thống kê doanh thu, số hóa đơn, số món đã bán, phân tích theo từng khách hàng, và xuất file báo cáo.

#### Hàm chính:
- **`handleGenerateReport()`** - report_service.c
  - Gọi `calculateTotalRevenue()` - Tổng doanh thu
  - Gọi `calculateTotalBills()` - Tổng số hóa đơn
  - Gọi `calculateTotalItems()` - Tổng số món đã bán
  - Gọi `analyzeByCustomer()` - Phân tích theo khách
  - Hiển thị báo cáo `printReportUI()`
  - Xuất file báo cáo

#### Hàm phụ trợ:

**1. `calculateTotalRevenue()`**
```
- Mở file "app/database/history.txt"
- Duyệt từng dòng:
  * Nếu bắt đầu bằng "BILL|":
    - Parse: BILL|ID|CustomerID|...|FinalPrice|ItemCount|...
    - Cộng dồn finalPrice vào totalRevenue
- Đóng file, trả về tổng
```

**2. `calculateTotalBills()`**
```
- Mở file "app/database/history.txt"
- Đếm số dòng bắt đầu bằng "BILL|"
- Trả về số lượng
```

**3. `calculateTotalItems()`**
```
- Mở file "app/database/history.txt"
- Duyệt từng dòng:
  * Nếu bắt đầu bằng "BILL|":
    - Parse itemCount từ cuối dòng
    - Cộng dồn vào totalItems
- Trả về tổng
```

**4. `analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount)`**
```
- Khai báo mảng CustomerRevenue
- Mở file "app/database/history.txt"
- Duyệt từng dòng:
  * Nếu bắt đầu bằng "BILL|":
    - Parse: customerId, itemCount, finalPrice
    - Gọi findOrCreateCustomer()
    - Cộng dồn: billCount++, revenue += finalPrice
- Trả về mảng + số lượng khách
```

**5. `findOrCreateCustomer()`**
```
- Tìm customerId trong mảng
- Nếu tồn tại: Trả về con trỏ entry đó
- Nếu chưa:
  * Thêm entry mới với customerId
  * Khởi tạo billCount = 0, revenue = 0.0
  * Trả về con trỏ entry mới
```

#### Cấu trúc dữ liệu sử dụng:
| Tên | Loại | Chức năng |
|-----|------|----------|
| `CustomerRevenue[]` | Array | Lưu doanh thu theo khách |
| File `history.txt` | Text File | Lưu trữ tất cả hóa đơn |

**CustomerRevenue Struct:**
```c
typedef struct {
    int customerId;      // ID khách hàng
    int billCount;       // Số hóa đơn của khách
    double revenue;      // Tổng doanh thu từ khách
} CustomerRevenue;
```

#### Luồng chi tiết:
```
1. Tính toán chỉ số:
   a) Tổng doanh thu:
      - calculateTotalRevenue()
      - Duyệt history.txt
      - Parse và cộng dồn finalPrice từ dòng BILL
      
   b) Tổng số hóa đơn:
      - calculateTotalBills()
      - Đếm dòng BILL
      
   c) Tổng số món:
      - calculateTotalItems()
      - Duyệt BILL, lấy itemCount cộng dồn
      
   d) Phân tích theo khách:
      - analyzeByCustomer()
      - Mỗi bill được gán vào khách tương ứng
      - Cộng: billCount, revenue
      
2. Hiển thị báo cáo:
   - printReportUI() in:
     * Tổng doanh thu
     * Tổng số hóa đơn
     * Tổng số món
     * Top khách hàng (theo doanh thu)
     * Bảng chi tiết từng khách
     
3. Xuất file:
   - Tạo file "app/database/report_doanhthu.txt"
   - Ghi báo cáo theo định dạng có cấu trúc
   - Bao gồm: Ngày giờ tạo, doanh thu, chi tiết
```

#### Định dạng File Báo Cáo (`report_doanhthu.txt`):
```
===========================================
     BAO CAO DOANH THU QUAN COM TAM
===========================================
Thoi gian: [Ngày giờ]

A. TONG HOP
  - Tong doanh thu: X.XXX VND
  - Tong so hoa don: Y
  - Tong so mon da ban: Z

B. PHAN TICH THEO KHACH HANG
  CustomerID | So Hoa Don | Doanh Thu
  1          | 5          | 500.000 VND
  2          | 3          | 300.000 VND
  ...

===========================================
```

#### File liên quan:
- [report_service.c](app/services/report_service.c#L1)
- [report_ui.c](app/ui/report_ui.c)
- [file_service.c](app/services/file_service.c)

---

## V. SỰ LIÊN KẾT GIỮA CÁC CHỨC NĂNG

```
Khởi động
  ↓
┌─────────────────────────────────────────┐
│ Menu Chính - Lựa chọn Chức Năng         │
└─────────────────────────────────────────┘
  ↓
┌─────────────────────────────────────────┐
│ Case 1: Đặt Món (OrderItem → Cart)      │
│ ↓ (sử dụng menuHashTable)               │
├─────────────────────────────────────────┤
│ Case 2: Chỉnh Giỏ Hàng (Sửa Cart)       │
│ ↓ (duyệt Linked List)                   │
├─────────────────────────────────────────┤
│ Case 3: Nhập Khách (Customer → B-Tree)  │
│ ↓ (lưu vào btreeRoot)                   │
├─────────────────────────────────────────┤
│ Case 4: Thanh Toán (Bill → File)        │
│ ↓ (cập nhật B-Tree, Linked List)        │
├─────────────────────────────────────────┤
│ Case 5: Xem Lịch Sử (Duyệt File)        │
│ ↓ (đọc từ history.txt)                  │
├─────────────────────────────────────────┤
│ Case 6: Báo Cáo Doanh Thu (Phân Tích)   │
│ ↓ (tổng hợp từ File)                    │
└─────────────────────────────────────────┘
  ↓
  Quay lại Menu Chính hoặc Thoát
```

---

## VI. TÓÓLQUỐC GIÁ TRỊ (Data Flow)

### Khởi động:
```
main()
  → loadMenuFromFile()        [menu.txt → menu[] → menuHashTable]
  → loadCustomersFromFile()   [khachhang.txt → danhSachKH[] → B-Tree]
  → loadHistoryFromFile()     [history.txt → HistoryNode Linked List]
```

### Luồng Đơn:
```
Case 1: Đặt món
  MenuItem (Hash) → OrderItem → Cart (Linked List)

Case 2: Chỉnh giỏ
  Cart (Linked List) ← Sửa

Case 3: Nhập khách
  Customer → B-Tree (Lưu) + danhSachKH[]

Case 4: Thanh toán
  Cart → Bill (Tính giá) → File
  Customer (B-Tree) → Cập nhật → File
  Bill → HistoryNode (Linked List) + File

Case 5: Xem lịch sử
  File/Linked List → HistoryNode → Display

Case 6: Báo cáo
  File (history.txt) → Phân tích → Report + File
```

---

## VII. BẢNG TÓM TẮT CẤU TRÚC DỮ LIỆU

| Tên | Loại | Mục đích | File |
|-----|------|---------|------|
| `menu[]` | Array (MenuItem) | Lưu 25 món | menu.txt |
| `menuHashTable` | Hash Table (101) | Tìm món O(1) | - |
| `btreeRoot` | B-Tree (cấp 4) | Quản lý khách theo SDT | khachhang.txt |
| `danhSachKH[]` | Array (Customer) | Lưu khách hàng | khachhang.txt |
| `currentBill.cart` | Doubly Linked List | Giỏ hàng hiện tại | - |
| `historyHead` | Singly Linked List | Lịch sử hóa đơn | history.txt |
| `currentBill` | Bill Struct | Hóa đơn hiện tại | - |

---

## VIII. CÁC HÀM TIỆN ÍCH CHÍNH

### Validator (app/utils/validator.c):
- `getValidInt()` - Nhập số nguyên an toàn
- `getValidDouble()` - Nhập số thực an toàn
- `getValidLine()` - Nhập chuỗi an toàn
- `getValidPhoneNumber()` - Nhập SDT với xác thực format

### Helper (app/utils/helper.c):
- `getCurrentDateTime()` - Lấy ngày giờ hiện tại
- `getRankColor()` - Lấy màu hiển thị theo hạng
- `system("cls")` - Xóa màn hình (Windows)
- `system("pause")` - Tạm dừng (Windows)

---

## IX. CẤU TRÚC FILE DỮ LIỆU

### 1. `menu.txt` - Danh sách menu
```
1|Sườn Nướng|35000|1
2|Sườn Kho|32000|0
3|Cút Nướng|28000|1
...
25|Nước Ép|15000|0
```
Format: `ID|Name|Price|HasOptions`

### 2. `khachhang.txt` - Danh sách khách hàng
```
1|0987654321|Nguyen Van A|123 Duong ABC|2500000|Gold
2|0912345678|Tran Thi B|456 Duong XYZ|1200000|Silver
...
```
Format: `ID|Phone|Name|Address|TotalSpent|Rank`

### 3. `history.txt` - Lịch sử hóa đơn chung
```
BILL|123456|1|Nguyen Van A|0987654321|2024-01-15 10:30|150000|15000|135000|3
ITEM|1|Sườn Nướng|40000|1|2|Không cay|40000
ITEM|5|Cơm|5000|2|0||10000
ITEM|20|Nước Ngọt|12000|1|0||12000
BILL|123457|0|Guest|||||60000|0|60000|1
ITEM|3|Thịt Lợn Nướng|60000|1|0||60000
```

### 4. `history_khach_X.txt` - Lịch sử riêng khách X
```
(Cùng định dạng như history.txt)
```

### 5. `report_doanhthu.txt` - Báo cáo doanh thu
```
(Tóm tắt doanh thu, số hóa đơn, phân tích khách)
```

---

## X. LUỒNG CHƯƠNG TRÌNH CHI TIẾT (Pseudocode)

```
PROGRAM QuanComTam

GLOBAL:
  menu[25]: MenuItem
  menuHashTable: HashTable
  btreeRoot: BTreeNode*
  danhSachKH[1000]: Customer
  soLuongKH: int
  historyHead: HistoryNode*
  currentBill: Bill

MAIN():
  loadMenuFromFile()        // Nạp menu
  loadCustomersFromFile()   // Nạp khách hàng
  loadHistoryFromFile()     // Nạp lịch sử
  printWelcomeScreen()
  
  cartInit(&currentBill.cart)  // Khởi tạo giỏ
  
  WHILE TRUE:
    DISPLAY_MENU_CHINH()
    choice = getValidInt()
    
    SWITCH choice:
      CASE 1:  // Đặt món
        handleOrderMenu(&currentBill)
        - LOOP: Nhập ID món
        - Tìm trong menuHashTable
        - Xác thực số lượng
        - Nhập tuỳ chọn (nếu có)
        - Nhập ghi chú
        - addToCart()
        
      CASE 2:  // Chỉnh giỏ
        handleCartMenu(&currentBill)
        - Hiển thị giỏ
        - LOOP: Menu chọn
          * Xóa: removeFromCart()
          * Sửa SL: updateQuantity()
          * Thoát: break
        
      CASE 3:  // Nhập khách
        handleCustomer(&currentBill)
        - Nhập SDT
        - found = searchBTree(btreeRoot, sdt)
        - IF found:
            - showCustomerInfo(found)
            - currentBill.customer = *found
          ELSE:
            - Menu: 1. Đăng ký / 0. Tiếp tục
            - IF 1:
                - Tạo Customer mới
                - insertBTree()
                - saveAllCustomersToFile()
            - ELSE:
                - currentBill.customer.id = 0
        
      CASE 4:  // Thanh toán
        processCheckout(&currentBill)
        - IF cart rỗng: return
        - subtotal = calculateSubtotal()
        - found = searchBTree(btreeRoot, phone)
        - IF found:
            - discount% = getDiscountPercent(rank)
            - discountAmount = subtotal * discount%
            - finalPrice = subtotal - discountAmount
          ELSE:
            - discount% = 0
            - finalPrice = subtotal
        - Hiển thị chi tiết
        - Nhập tiền khách
        - printBillUI()
        - Xác nhận (1/0)
        - IF 1:
            - IF found: Cập nhật customer + B-Tree + File
            - saveBillToGlobalFile()
            - saveBillToHistoryFile()
            - addBillToHistory()
            - Dọn giỏ
        
      CASE 5:  // Xem lịch sử
        handleViewHistory()
        - printAllHistory()  // In tất cả từ file
        - LOOP:
            - Menu con: 1. Khách / 2. Bill ID / 0. Thoát
            - IF 1:
                - Nhập SDT
                - id = getCustomerIdByPhone()
                - history = getCustomerHistory(id)
                - printCustomerHistory()
            - IF 2:
                - Nhập billId
                - bill = searchBillById()
                - printBillDetail()
        
      CASE 6:  // Báo cáo
        handleGenerateReport()
        - totalRevenue = calculateTotalRevenue()
        - totalBills = calculateTotalBills()
        - totalItems = calculateTotalItems()
        - analyzeByCustomer()
        - printReportUI()
        - Xuất file report
        
      CASE 0:
        RETURN 0
        
END MAIN
```

---

## XI. PHÂN TÍCH ĐỘ PHỨC TẠP THUẬT TOÁN

| Hàm | Độ phức tạp | Ghi chú |
|-----|------------|---------|
| `hashTableSearch()` | O(1) avg | Với HASH_TABLE_SIZE=101 |
| `hashTableInsert()` | O(1) avg | Thêm vào đầu bucket |
| `searchBTree()` | O(log n) | B-Tree cấp 4 |
| `insertBTree()` | O(log n) | Với tự động cân bằng |
| `cartAddItem()` | O(1) | Thêm vào tail |
| `cartRemoveItemByIndex()` | O(k) | k = vị trí item |
| `cartUpdateQuantityByIndex()` | O(k) | k = vị trí item |
| `calculateTotalRevenue()` | O(m) | m = số dòng file |
| `analyzeByCustomer()` | O(m) | m = số dòng file |

---

## XII. KẾT LUẬN

Hệ thống này sử dụng nhiều cấu trúc dữ liệu đa dạng:
- **Hash Table**: Tìm kiếm menu nhanh O(1)
- **B-Tree**: Quản lý khách hàng hiệu quả O(log n)
- **Linked List**: Giỏ hàng linh hoạt
- **File I/O**: Lưu trữ dữ liệu bền vững
- **MVC Pattern**: Tách biệt logic, UI, và mô hình

Luồng chương trình tuân theo chu trình: Nạp → Hiển thị → Xử lý → Lưu → Quay lại, tạo nên một hệ thống quản lý quán ăn hoàn chỉnh.

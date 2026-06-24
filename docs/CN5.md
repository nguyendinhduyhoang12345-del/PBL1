# Single Linked List trong Chức Năng 5 — Xem Lịch Sử

---

## 1. Tổng Quan

Chức năng 5 (**Xem Lịch Sử**) sử dụng cấu trúc dữ liệu **Singly Linked List (Danh sách liên kết đơn)** để lưu trữ và duyệt toàn bộ lịch sử hóa đơn của quán.

- Mỗi **nút (node)** trong danh sách đại diện cho một **hóa đơn** đã được thanh toán.
- Các nút được nối với nhau theo **một chiều** (từ đầu → cuối), không có liên kết ngược.
- Con trỏ toàn cục `historyHead` trỏ đến nút đầu tiên của danh sách.

---

## 2. Định Nghĩa Node

```c
// Khai báo trong: app/models/models.h

typedef struct HistoryNode {
    int customerId;          // ID khách hàng (0 = Khách vãng lai / Guest)
    Bill bill;               // Toàn bộ thông tin hóa đơn
    struct HistoryNode* next; // Con trỏ trỏ đến node kế tiếp
} HistoryNode;
```

| Trường        | Kiểu               | Ý nghĩa                            |
|---------------|--------------------|------------------------------------|
| `customerId`  | `int`              | ID khách hàng; `0` nếu là Guest    |
| `bill`        | `Bill`             | Struct chứa đầy đủ dữ liệu hóa đơn |
| `next`        | `HistoryNode*`     | Con trỏ đến node tiếp theo         |

---

## 3. Cấu Trúc Bill Bên Trong Node

```c
typedef struct {
    int id;              // ID hóa đơn (duy nhất)
    Customer customer;   // Thông tin khách hàng
    Cart cart;           // Giỏ hàng (danh sách món đã đặt)
    double total;        // Tổng tiền trước giảm giá
    double discount;     // Số tiền được giảm
    double finalPrice;   // Thành tiền sau khi giảm
    char dateTime[50];   // Ngày giờ tạo hóa đơn
} Bill;
```

---

## 4. Hình Dạng Linked List Trong Bộ Nhớ

```
historyHead
    │
    ▼
┌──────────────────┐     ┌──────────────────┐     ┌──────────────────┐
│ customerId: 1    │     │ customerId: 0    │     │ customerId: 1    │
│ bill.id: 123456  │ ──► │ bill.id: 123457  │ ──► │ bill.id: 123458  │ ──► NULL
│ bill.customer:   │     │ bill.customer:   │     │ bill.customer:   │
│   Nguyen Van A   │     │   Guest          │     │   Nguyen Van A   │
└──────────────────┘     └──────────────────┘     └──────────────────┘
      Node 1                   Node 2                   Node 3
```

> **Chiều duyệt:** Luôn từ `historyHead` → `next` → `next` → ... → `NULL`

---

## 5. Con Trỏ Toàn Cục

```c
// Khai báo trong: app/models/models.h
extern HistoryNode* historyHead;
```

- `historyHead` là **điểm vào duy nhất** của toàn bộ linked list.
- Khi chương trình khởi động, `historyHead` được khởi tạo = `NULL`.
- Sau khi `loadHistoryFromFile()` chạy, `historyHead` trỏ đến node đầu tiên.

---

## 6. Khởi Tạo — Nạp Dữ Liệu Từ File

**Hàm:** `loadHistoryFromFile()` — `app/services/file_service.c`

Khi chương trình khởi động, lịch sử được đọc từ file `app/database/history.txt` và lắp đầy vào linked list:

```
Pseudocode:

loadHistoryFromFile():
  Mở file "app/database/history.txt"

  WHILE đọc từng dòng:
    IF dòng bắt đầu bằng "BILL|":
      → Parse: billId, customerId, name, phone, dateTime, total, discount, finalPrice
      → Cấp phát node mới: malloc(sizeof(HistoryNode))
      → Gán các trường dữ liệu vào node
      → node->next = NULL

      → IF historyHead == NULL:        // Danh sách rỗng
            historyHead = node         // Node đầu tiên
         ELSE:
            Duyệt đến cuối danh sách
            tail->next = node          // Nối vào đuôi

    ELSE IF dòng bắt đầu bằng "ITEM|":
      → Parse item và thêm vào cart của bill hiện tại

  Đóng file
```

**Kết quả:** Một linked list hoàn chỉnh trong RAM, sẵn sàng để duyệt.

---

## 7. Các Thao Tác Trên Linked List

### 7.1. Duyệt Toàn Bộ — `printAllHistory()`

Duyệt từng node từ đầu đến cuối, in thông tin tóm tắt của mỗi hóa đơn.

```c
// app/ui/history_ui.c
void printAllHistory() {
    HistoryNode* current = historyHead;   // Bắt đầu từ đầu danh sách

    while (current != NULL) {             // Lặp đến khi hết danh sách
        printf("%d | %-12s | %-10s | %9.0f | %s\n",
               current->bill.id,
               current->bill.customer.name,
               current->bill.customer.phone,
               current->bill.finalPrice,
               current->bill.dateTime);

        current = current->next;          // Tiến đến node tiếp theo
    }
}
```

**Độ phức tạp:** O(n) — n là tổng số hóa đơn

---

### 7.2. Tìm ID Khách Theo SĐT — `getCustomerIdByPhone()`

Duyệt linked list, so sánh số điện thoại tại mỗi node.

```c
// app/services/history_service.c
int getCustomerIdByPhone(char* phone) {
    HistoryNode* current = historyHead;

    while (current != NULL) {
        if (strcmp(current->bill.customer.phone, phone) == 0) {
            return current->bill.customer.id;   // Tìm thấy → trả về ID
        }
        current = current->next;
    }

    return -1;   // Không tìm thấy
}
```

```
Ví dụ tìm phone = "0987654321":

  historyHead
      │
      ▼
  [Node 1: phone="0987654321"] ──► MATCH! → return customerId = 1
```

**Độ phức tạp:** O(n) — trường hợp tệ nhất duyệt hết danh sách

---

### 7.3. Lọc Lịch Sử Theo Khách — `getCustomerHistory()`

Duyệt toàn bộ danh sách, **copy** các node thuộc về khách hàng cần tìm vào một danh sách mới.

```c
// app/services/history_service.c
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* head = NULL;   // Đầu danh sách kết quả
    HistoryNode* tail = NULL;   // Đuôi danh sách kết quả (để nối nhanh)

    HistoryNode* current = historyHead;

    while (current != NULL) {
        if (current->bill.customer.id == customerId) {
            // Tạo bản copy của node này
            HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
            newNode->customerId = customerId;
            newNode->bill       = current->bill;
            newNode->next       = NULL;

            // Nối vào danh sách kết quả
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

    return head;   // Trả về đầu danh sách kết quả
}
```

```
Ví dụ lọc customerId = 1:

Danh sách gốc:
  [CustID=1, Bill#123456] → [CustID=0, Bill#123457] → [CustID=1, Bill#123458] → NULL

Quá trình duyệt:
  Node 1: CustID==1 ✓ → Copy vào kết quả
  Node 2: CustID==0 ✗ → Bỏ qua
  Node 3: CustID==1 ✓ → Copy vào kết quả

Danh sách kết quả (mới):
  [Copy Bill#123456] → [Copy Bill#123458] → NULL
```

**Độ phức tạp:** O(n)

---

### 7.4. Tìm Hóa Đơn Theo ID — `searchBillById()`

Duyệt từng node, so sánh `bill.id` đến khi tìm thấy hoặc hết danh sách.

```c
// app/services/history_service.c
HistoryNode* searchBillById(int billId) {
    HistoryNode* current = historyHead;

    while (current != NULL) {
        if (current->bill.id == billId) {
            return current;   // Trả về con trỏ trực tiếp đến node
        }
        current = current->next;
    }

    return NULL;   // Không tìm thấy
}
```

**Độ phức tạp:** O(n)

---

## 8. Tại Sao Dùng Singly Linked List?

| Tiêu chí               | Lý do phù hợp                                                              |
|------------------------|----------------------------------------------------------------------------|
| **Số lượng linh hoạt** | Không biết trước số lượng hóa đơn → cấp phát động từng node               |
| **Chỉ duyệt 1 chiều**  | Tất cả thao tác đều duyệt từ đầu → cuối, không cần duyệt ngược            |
| **Tiết kiệm bộ nhớ**   | Chỉ cần 1 con trỏ `next` mỗi node, không cần `prev` như Doubly Linked List |
| **Thêm vào cuối nhanh**| Dùng con trỏ `tail` khi load file → O(1) mỗi lần thêm                    |

---

## 9. Tóm Tắt Các Hàm Sử Dụng Linked List

| Hàm                        | File                        | Thao tác trên List            | Độ phức tạp |
|----------------------------|-----------------------------|-------------------------------|-------------|
| `loadHistoryFromFile()`    | `services/file_service.c`   | Xây dựng danh sách từ file    | O(n)        |
| `printAllHistory()`        | `ui/history_ui.c`           | Duyệt toàn bộ                 | O(n)        |
| `getCustomerIdByPhone()`   | `services/history_service.c`| Tìm kiếm tuần tự theo SĐT     | O(n)        |
| `getCustomerHistory()`     | `services/history_service.c`| Lọc & tạo danh sách con mới   | O(n)        |
| `searchBillById()`         | `services/history_service.c`| Tìm kiếm tuần tự theo Bill ID | O(n)        |

---

*Hệ Thống Quản Lý Quán Cơm Tấm — PBL1*

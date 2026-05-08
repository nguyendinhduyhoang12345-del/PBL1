# KẾ HOẠCH REFACTOR HỆ THỐNG QUẢN LÝ ĐƠN HÀNG VÀ GIỎ HÀNG
## Ngày lập: 08/05/2026

---

## 1. TÌNH TRẠNG HIỆN TẠI
### 1.1 Cấu trúc dữ liệu Order/Cart (Array-based)
- **File liên quan**: `models.h`, `order_controller.c`, `cart_ui.c`
- **Cấu trúc hiện tại**:
  ```c
  typedef struct {
      int id;
      char name[50];
      double price;
      int quantity;
      int option;
      char note[100];
      double totalPrice;
  } OrderItem;

  typedef struct {
      int id;
      Customer customer;
      OrderItem items[MAX_GIO_HANG];  // Array tĩnh, tối đa 30 items
      int itemCount;
      double total;
      double discount;
      double finalPrice;
      char dateTime[50];
  } Bill;
  ```
- **Vấn đề hiện tại**:
  - Giới hạn cứng 30 items trong một đơn hàng
  - Tìm kiếm item trùng lặp: O(n) - phải duyệt toàn bộ array
  - Xóa item: phải dịch chuyển toàn bộ phần tử phía sau (O(n))
  - Không linh hoạt khi thêm/xóa giữa danh sách

### 1.2 Hiện trạng chức năng
- `addToCart()`: Thêm item, check trùng lặp bằng O(n) loop
- `removeFromCart()`: Xóa bằng array shifting
- `updateQuantity()`: Tìm kiếm tuần tự

---

## 2. GIẢI PHÁP ĐỀ XUẤT

### 2.1 Hash Table cho Order Management
**Mục tiêu**: 
- Cải thiện tốc độ tìm kiếm và quản lý items
- Hỗ trợ tự động cộng số lượng khi thêm cùng 1 món
- Xóa/sửa nhanh hơn

**Lợi ích**:
- Tìm kiếm item: O(1) trung bình
- Thêm item trùng: tự động cộng SL thay vì tạo duplicate
- Không giới hạn số lượng items (hết bộ nhớ)
- Cơ cấu giảm duy trì `itemCount`

### 2.2 Doubly Linked List cho Cart Edit/View
**Mục tiêu**:
- Hiệu quả hơn trong việc xem, chỉnh sửa giỏ hàng
- Duyệt tới trước hoặc lùi lại trong danh sách

**Lợi ích**:
- Dễ thêm/xóa giữa danh sách (không cần dịch chuyển)
- Duyệt 2 chiều (forward/backward) cho hiển thị
- Linh hoạt trong thao tác giỏ hàng

---

## 3. KỸ THUẬT TRIỂN KHAI CHI TIẾT

### 3.1 Hash Table Implementation (Để lưu trữ items)

#### 3.1.1 Tạo file mới: `app/services/hash_table_service.h`
```c
#ifndef HASH_TABLE_SERVICE_H
#define HASH_TABLE_SERVICE_H

#include "../models/models.h"

#define HASH_TABLE_SIZE 31  // Số nguyên tố để giảm collision

typedef struct HashNode {
    int itemId;           // Key: ID của OrderItem
    OrderItem item;       // Value: Thông tin OrderItem
    struct HashNode* next; // For chaining (xử lý collision)
} HashNode;

typedef struct {
    HashNode** table;      // Array of pointers
    int size;
    int count;             // Số item hiện tại
} HashTable;

// Hàm khởi tạo hash table
HashTable* hashTableCreate(int size);

// Hàm hash function (dùng module)
int hashFunction(int key, int tableSize);

// Thêm item vào hash table (nếu trùng thì cộng SL)
int hashTableInsert(HashTable* ht, OrderItem* item);

// Lấy item từ hash table
OrderItem* hashTableGet(HashTable* ht, int itemId);

// Xóa item từ hash table
int hashTableDelete(HashTable* ht, int itemId);

// Cập nhật SL item
int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty);

// Lấy tất cả items dưới dạng array (để hiển thị)
OrderItem* hashTableToArray(HashTable* ht, int* count);

// Giải phóng memory
void hashTableFree(HashTable* ht);

#endif
```

#### 3.1.2 Tạo file mới: `app/services/hash_table_service.c`
- Implement hash function: `hash(itemId) = itemId % HASH_TABLE_SIZE`
- Xử lý collision: Chaining (linked list)
- Insert: Tìm vị trí, nếu trùng SL (quantity) += SL nhập
- Delete: Tìm node, xóa khỏi chain
- toArray: Duyệt tất cả node, convert về OrderItem array

### 3.2 Doubly Linked List Implementation (Để hiển thị & edit giỏ hàng)

#### 3.2.1 Tạo file mới: `app/services/doubly_linked_list_service.h`
```c
#ifndef DOUBLY_LINKED_LIST_SERVICE_H
#define DOUBLY_LINKED_LIST_SERVICE_H

#include "../models/models.h"

typedef struct DLLNode {
    OrderItem item;
    int position;              // Vị trí trong danh sách (1, 2, 3, ...)
    struct DLLNode* next;
    struct DLLNode* prev;
} DLLNode;

typedef struct {
    DLLNode* head;
    DLLNode* tail;
    int count;
} DoublyLinkedList;

// Khởi tạo doubly linked list
DoublyLinkedList* dllCreate();

// Thêm item vào cuối danh sách
int dllAppend(DoublyLinkedList* dll, OrderItem* item);

// Xóa item tại vị trí
int dllRemoveAt(DoublyLinkedList* dll, int position);

// Sửa SL item tại vị trí
int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty);

// Lấy item tại vị trí
OrderItem* dllGetAt(DoublyLinkedList* dll, int position);

// Duyệt từ đầu
DLLNode* dllGetFirst(DoublyLinkedList* dll);

// Duyệt từ cuối (ngược lại)
DLLNode* dllGetLast(DoublyLinkedList* dll);

// Cập nhật vị trí cho tất cả node
void dllUpdatePositions(DoublyLinkedList* dll);

// Giải phóng memory
void dllFree(DoublyLinkedList* dll);

#endif
```

#### 3.2.2 Tạo file mới: `app/services/doubly_linked_list_service.c`
- Append: Tạo node, link vào tail
- RemoveAt: Tìm node, unlink 2 chiều
- UpdateQuantity: Tìm node, update `item.quantity` và `item.totalPrice`
- GetAt: Duyệt từ đầu/cuối (tối ưu)

### 3.3 Sửa models.h
```c
// Thêm định nghĩa mới cho Bill (thay vì array)
typedef struct {
    int id;
    Customer customer;
    HashTable* itemsHash;          // Thay thế items[MAX_GIO_HANG]
    DoublyLinkedList* cartDisplay; // Dùng cho hiển thị/edit UI
    int itemCount;
    double total;
    double discount;
    double finalPrice;
    char dateTime[50];
} Bill;
```

---

## 4. CÁC BƯỚC TRIỂN KHAI

### Phase 1: Tạo Hash Table Service
**Files**:
- Tạo `app/services/hash_table_service.h` (header)
- Tạo `app/services/hash_table_service.c` (implement)

**Chi tiết**:
- ✓ Định nghĩa `HashNode` struct (itemId, item, next)
- ✓ Định nghĩa `HashTable` struct
- ✓ Implement hàm create, insert, delete, get, update
- ✓ Implement hash function: `key % HASH_TABLE_SIZE`
- ✓ Handle collision bằng chaining
- ✓ Implement toArray (convert để hiển thị)

**Test**:
- Unit test: Thêm 10 items, search O(1)
- Test trùng lặp: Thêm cùng item 2 lần, check SL cộng
- Test xóa: Xóa item, check count giảm

---

### Phase 2: Tạo Doubly Linked List Service
**Files**:
- Tạo `app/services/doubly_linked_list_service.h` (header)
- Tạo `app/services/doubly_linked_list_service.c` (implement)

**Chi tiết**:
- ✓ Định nghĩa `DLLNode` struct (item, position, next, prev)
- ✓ Định nghĩa `DoublyLinkedList` struct
- ✓ Implement hàm create, append, removeAt, updateQuantity
- ✓ Implement duyệt từ đầu/cuối
- ✓ Implement updatePositions (cập nhật STT)

**Test**:
- Unit test: Append 5 items, duyệt forward/backward
- Test removeAt: Xóa item giữa, check position update
- Test updateQuantity: Sửa SL, check totalPrice

---

### Phase 3: Cập nhật models.h
**Sửa**:
- Thay `OrderItem items[MAX_GIO_HANG]` bằng `HashTable* itemsHash`
- Thêm `DoublyLinkedList* cartDisplay`
- Loại bỏ `MAX_GIO_HANG` constant (nếu không dùng)

---

### Phase 4: Refactor order_controller.c

#### 4.1 Sửa `addToCart()`
**Cũ**:
- Duyệt array tìm duplicate (O(n))
- Nếu trùng → cộng SL
- Nếu không → thêm item mới
- Array shifting nếu cần

**Mới**:
- Tạo `OrderItem` từ input
- `hashTableInsert()` (O(1), tự động handle duplicate)
- `dllAppend()` vào cartDisplay
- Hiển thị "Đã thêm"

#### 4.2 Sửa `removeFromCart()`
**Cũ**:
- Tìm STT
- Array shift
- Cập nhật itemCount

**Mới**:
- Tìm STT trong cartDisplay
- `dllRemoveAt()`
- Tìm item trong hashTable, `hashTableDelete()`
- Cập nhật vị trí

#### 4.3 Sửa `updateQuantity()`
**Cũ**:
- Tìm tuần tự
- Cập nhật array
- Check stock

**Mới**:
- Tìm trong cartDisplay bằng position
- `dllUpdateQuantity()`
- `hashTableUpdateQuantity()`
- Check stock

#### 4.4 Sửa `handleOrderMenu()`
- Khởi tạo HashTable: `currentBill->itemsHash = hashTableCreate(HASH_TABLE_SIZE)`
- Khởi tạo DLL: `currentBill->cartDisplay = dllCreate()`

#### 4.5 Sửa `handleCartMenu()`
- Lấy array từ hash table: `OrderItem* arr = hashTableToArray(ht, &count)`
- Hiển thị dùng DLL positions

---

### Phase 5: Cập nhật cart_ui.c

#### 5.1 Sửa `showCartUI()`
**Cũ**:
```c
for (int i = 0; i < currentBill->itemCount; i++) {
    OrderItem item = currentBill->items[i];
    // Hiển thị
}
```

**Mới**:
```c
// Lấy array từ hash table
OrderItem* arr = hashTableToArray(currentBill->itemsHash, &count);
for (int i = 0; i < count; i++) {
    OrderItem item = arr[i];
    // Hiển thị
}
free(arr);
```

---

### Phase 6: Cập nhật checkout_controller.c & checkout_service.c

#### 6.1 Sửa `calculateSubtotal()`
**Cũ**:
```c
for (int i = 0; i < bill->itemCount; i++) {
    subtotal += bill->items[i].totalPrice;
}
```

**Mới**:
```c
OrderItem* arr = hashTableToArray(bill->itemsHash, &count);
for (int i = 0; i < count; i++) {
    subtotal += arr[i].totalPrice;
}
free(arr);
```

#### 6.2 Sửa hàm xử lý thanh toán
- Duyệt hash table để lấy tất cả items

---

### Phase 7: Cập nhật history_service.c

#### 7.1 Sửa `addBillToHistory()`
```c
// Trước khi lưu, convert hash table -> array
OrderItem* arr = hashTableToArray(bill->itemsHash, &count);
HistoryNode->bill->items = arr;  // hoặc copy array
HistoryNode->bill->itemCount = count;
```

#### 7.2 Sửa `saveBillToHistoryFile()` & `saveBillToGlobalFile()`
- Duyệt array của items (từ hash table)

---

## 5. THAY ĐỔI FILE - CHI TIẾT

| File | Thay đổi | Loại |
|------|---------|------|
| `models.h` | Sửa struct Bill (Hash + DLL) | Modify |
| `order_controller.c` | Refactor addToCart, removeFromCart, updateQuantity | Modify |
| `order_controller.h` | Không thay đổi (interface giữ nguyên) | - |
| `cart_ui.c` | Cập nhật showCartUI (dùng hash table) | Modify |
| `checkout_service.c` | Cập nhật calculateSubtotal | Modify |
| `history_service.c` | Sửa lưu/load lịch sử | Modify |
| `hash_table_service.h` | **TẠO MỚI** | New |
| `hash_table_service.c` | **TẠO MỚI** | New |
| `doubly_linked_list_service.h` | **TẠO MỚI** | New |
| `doubly_linked_list_service.c` | **TẠO MỚI** | New |
| `main.c` | Khởi tạo Hash + DLL cho Bill | Minor |

---

## 6. CHIẾN LƯỢC TRIỂN KHAI

### Giai đoạn 1: Tạo Services (Hash Table + DLL)
- ✓ Viết unit test trước (TDD)
- ✓ Implement hash_table_service
- ✓ Implement doubly_linked_list_service
- ✓ Test riêng lẻ

### Giai đoạn 2: Cập nhật Data Models
- ✓ Sửa `models.h`
- ✓ Cập nhật `main.c` khởi tạo

### Giai đoạn 3: Refactor Controllers & Services
- ✓ `order_controller.c`
- ✓ `cart_ui.c`
- ✓ `checkout_service.c`
- ✓ `history_service.c`

### Giai đoạn 4: Integration Testing
- ✓ Test toàn flow: Đặt → Sửa → Xóa → Thanh toán
- ✓ Test edge case: Rỗng, 1 item, 100 items

### Giai đoạn 5: Optimization & Polish
- ✓ Kiểm tra memory leak
- ✓ Tối ưu hash collision
- ✓ Final testing

---

## 7. LỢI ÍCH KỲ VỌNG

| Khía cạnh | Cũ | Mới | Cải thiện |
|-----------|-----|-----|-----------|
| **Tìm kiếm item** | O(n) | O(1) avg | Nhanh 10-30x |
| **Thêm duplicate** | Copy 2 dòng | Auto cộng | Tự động |
| **Xóa item** | O(n) shift | O(1) | Nhanh 10x |
| **Giới hạn items** | Max 30 | Unlimited | Flexible |
| **Edit giỏ** | Array shifting | DLL move | Mượt hơn |
| **Memory** | Fixed | Dynamic | Tối ưu |

---

## 8. TESTING PLAN

### 8.1 Unit Test Hash Table
```
Test 1: Insert & Get
- Insert item ID 1 → Get → Pass
- Insert item ID 2 → Get → Pass

Test 2: Duplicate handling
- Insert ID 1, SL=2
- Insert ID 1, SL=3 → SL cộng = 5 ✓

Test 3: Delete
- Insert 5 items
- Delete item 3
- Count = 4 ✓

Test 4: Hash collision
- Insert multiple items có hash cùng bucket
- Verify chain link ✓
```

### 8.2 Unit Test Doubly Linked List
```
Test 1: Append & Traverse
- Append 5 items
- Traverse forward: 1→2→3→4→5 ✓
- Traverse backward: 5→4→3→2→1 ✓

Test 2: Remove At
- Append 5 items (Pos 1-5)
- Remove Pos 3
- Count = 4, Pos update: 1,2,3,4 ✓

Test 3: Update Quantity
- Append item SL=2
- Update SL=5
- totalPrice update ✓
```

### 8.3 Integration Test
```
Flow: Add → View → Edit → Delete → Checkout
1. Đặt 5 cái bánh mì
2. Xem giỏ → Hiển thị đúng
3. Sửa 2 cái → SL=7
4. Xóa 1 item
5. Xem lịch sử → Đúng 4 items
6. Thanh toán → Success
```

---

## 9. LƯU Ý QUAN TRỌNG

⚠️ **Memory Management**:
- Phải `free()` Hash Table & DLL trong `main.c` khi kết thúc
- Hoặc sau mỗi Bill (checkout)

⚠️ **Đặc biệt - Cách xử lý 2 cấu trúc**:
- **Hash Table** (`itemsHash`): Lưu trữ duy nhất, lookup nhanh
- **Doubly Linked List** (`cartDisplay`): Mirror của hash table, dùng cho hiển thị & edit theo STT
- Luôn đồng bộ 2 cấu trúc khi insert/delete/update

⚠️ **Backward Compatibility**:
- Interface `handleOrderMenu()` giữ nguyên
- `Bill` struct cần cập nhật nhưng logic ngoài giữ nguyên

---

## 10. MILESTONE & TIMELINE

| Milestone | Task | Thời gian | Status |
|-----------|------|----------|--------|
| M1 | Create Hash Table Service | 1-2 giờ | Pending |
| M2 | Create Doubly Linked List | 1-2 giờ | Pending |
| M3 | Update models.h | 30 phút | Pending |
| M4 | Refactor order_controller.c | 1-2 giờ | Pending |
| M5 | Refactor cart_ui.c | 30 phút | Pending |
| M6 | Refactor checkout & history | 1 giờ | Pending |
| M7 | Integration Testing | 2 giờ | Pending |
| M8 | Bug fixes & Optimization | 1 giờ | Pending |

---

## 11. NOTES & DISCUSSION

- **Hash Table Size**: Chọn số nguyên tố (31, 37, 53) để giảm collision
- **Collision Resolution**: Chaining với linked list (đơn giản, hiệu quả)
- **DLL Sync**: Mỗi lần insert/delete hash table cũng phải update DLL
- **UI Display**: Dùng `hashTableToArray()` để lấy array rồi hiển thị
- **Backward Compat**: Phần khác (checkout, history) sử dụng items array như cũ

---

**Cập nhật lần cuối**: 08/05/2026
**Người lập**: GitHub Copilot
**Trạng thái**: Planning Phase ✓ Ready for Implementation

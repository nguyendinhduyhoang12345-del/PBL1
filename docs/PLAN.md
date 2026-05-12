<<<<<<< HEAD
# Ý tưởng mới cho phần đặt món và giỏ hàng

## 1. Mục tiêu

Tách hai phần rõ ràng:
1. Phần `Đặt món`: nhân viên dùng mã món và tìm nhanh trong Hash Table được nạp từ `menu.txt`.
2. Phần `Giỏ hàng`: dùng Doubly Linked List để quản lý các món đã chọn với 3 chức năng chính là Thêm, Xóa, Chỉnh sửa.

## 2. Luồng dữ liệu tổng quát

menu.txt ──load──► Hash Table (RAM)
                          │
                     nhân viên tìm món
                          │
                          ▼
                   tìm thấy MenuItem
                          │
                     thêm vào giỏ
                          │
                          ▼
              Doubly Linked List (giỏ hàng)

## 3. Phần 1: Đặt món (Hash Table)

1. Ứng dụng đọc dữ liệu từ `menu.txt` vào dãy `MenuItem`.
2. Tạo một `HashTable` RAM để tra cứu món nhanh theo `itemId`.
3. Nhân viên nhập `mã món`.
4. Dùng hàm tìm kiếm trong Hash Table để lấy `MenuItem` tương ứng.
5. Nếu tìm được món và số lượng hợp lệ:
   - kiểm tra tồn kho
   - kiểm tra ràng buộc số lượng món chính nếu cần
   - xây dựng `OrderItem` từ thông tin `MenuItem`
6. Gọi `addToCart()` để chuyển `OrderItem` vào giỏ hàng.

## 4. Phần 2: Giỏ hàng (Doubly Linked List)

Giỏ hàng sẽ dùng `DoublyLinkedList` làm cấu trúc chính, với mỗi node chứa một `OrderItem`.

### Các chức năng chính:
- Thêm món
- Xóa món
- Chỉnh sửa số lượng

### Mục tiêu của Doubly Linked List:
- Hiển thị giỏ hàng theo thứ tự thêm
- Duyệt đến vị trí STT nhanh hơn
- Cập nhật xóa/chỉnh sửa tại vị trí cụ thể

## 5. Luồng chi tiết cho giỏ hàng

1. Khi nhân viên bấm "Thêm vào giỏ":
   - Tạo `OrderItem` từ `MenuItem`
   - Thêm node mới vào cuối `DoublyLinkedList`
   - Cập nhật số lượng món

2. Khi nhân viên bấm "Xóa":
   - Nhập STT món cần xóa
   - Dò theo vị trí trong doubly linked list
   - Xóa node tương ứng
   - Hoàn lại tồn kho nếu cần

3. Khi nhân viên bấm "Chỉnh sửa":
   - Nhập STT món cần sửa
   - Dò node tại vị trí đó
   - Thay đổi `quantity`, có thể thay đổi `totalPrice`
   - Cập nhật lại danh sách và số lượng

## 6. Tương tác giữa Hash Table và Doubly Linked List

- `HashTable` chỉ dùng cho phần tìm menu nhanh khi đặt món.
- `DoublyLinkedList` giữ dữ liệu giỏ hàng sau khi nhân viên xác nhận chọn món.
- Khi thêm món, `OrderItem` lấy từ `MenuItem` rồi push vào Doubly Linked List.
- Khi cần hiển thị/ sửa/ xóa giỏ hàng, chỉ thao tác trên Doubly Linked List.

## 7. Lợi ích của cách thiết kế này

- Tìm món nhanh, không phải duyệt toàn bộ menu mỗi lần nhập mã.
- Giỏ hàng linh hoạt, cho phép thêm/xóa/chỉnh sửa theo STT đơn giản.
- Cấu trúc rõ ràng: menu lookup tách biệt khỏi quản lý giỏ.

## 8. Gợi ý thực hiện

- `menu.txt` load vào `MenuItem[]`.
- Dùng `hashTableInsertMenuItem()` để xây `HashTable` menu.
- Hàm `addToCart()` nhận `OrderItem` và append vào `DoublyLinkedList`.
- Giỏ hàng nên có hàm:
  - `cartAddItem()`
  - `cartRemoveItemByIndex()`
  - `cartUpdateQuantityByIndex()`

---

Phần này là ý tưởng mới theo đúng mô hình bạn muốn: `menu.txt` → `Hash Table` để tìm nhanh, và `Doubly Linked List` để quản lý giỏ hàng.
=======
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

### 2.1 Hash Table cho Order Management (Chức năng 1: Đặt món)
**Mục tiêu**: 
- Cải thiện tốc độ tìm kiếm và quản lý items khi đặt món
- Hỗ trợ tự động cộng số lượng khi thêm cùng 1 món
- Không giới hạn số lượng items (hết bộ nhớ)

**Lợi ích**:
- Tìm kiếm item: O(1) trung bình
- Thêm item trùng: tự động cộng SL thay vì tạo duplicate
- Cơ cấu giảm duy trì `itemCount`

### 2.2 Hash Table + Doubly Linked List cho Cart Edit/View (Chức năng 2: Xem và chỉnh sửa giỏ hàng)
**Mục tiêu**:
- Hash Table: Lưu trữ dữ liệu chính xác, lookup nhanh
- Doubly Linked List: Hiển thị và thao tác giỏ hàng theo STT
- Kết hợp 2 cấu trúc để tối ưu hiệu suất

**Lợi ích**:
- Thêm/xóa/sửa nhanh: Hash Table O(1), DLL O(1) cho thao tác
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
- Get: Tìm node theo key và trả pointer
- UpdateQuantity: Nếu tồn tại, đổi quantity và totalPrice
- toArray: Duyệt tất cả node, copy vào array động, trả về qua `count`
- Free: Giải phóng tất cả node và bảng pointer

#### 3.1.3 Chi tiết hàm trong `hash_table_service`
- `HashTable* hashTableCreate(int size)`
  - Tạo struct, cấp phát `table = malloc(size * sizeof(HashNode*))`
  - Khởi tạo mỗi bucket = NULL
  - Trả về con trỏ `HashTable`

- `int hashFunction(int key, int tableSize)`
  - Trả `key % tableSize`
  - Nếu giá trị âm, chuyển thành dương bằng `+ tableSize`

- `int hashTableInsert(HashTable* ht, OrderItem* item)`
  - Tính bucket bằng hashFunction(item->id, ht->size)
  - Duyệt chain:
    - Nếu node tồn tại với `itemId`, cộng `quantity`, cập nhật `totalPrice`
    - Nếu không tồn tại, tạo node mới, copy item, thêm vào đầu/bucket hoặc cuối chain
  - Tăng `ht->count` nếu thêm node mới
  - Trả 0 nếu thành công, -1 nếu lỗi bộ nhớ

- `OrderItem* hashTableGet(HashTable* ht, int itemId)`
  - Tính bucket, duyệt chain
  - Nếu tìm thấy, trả pointer đến `item`
  - Nếu không, trả NULL

- `int hashTableDelete(HashTable* ht, int itemId)`
  - Tính bucket, duyệt chain để tìm node
  - Nếu tìm thấy, unlink node khỏi chain, free node, giảm `count`
  - Trả 0 nếu xóa thành công, -1 nếu không tìm thấy

- `int hashTableUpdateQuantity(HashTable* ht, int itemId, int newQty)`
  - Lấy node qua `hashTableGet`
  - Nếu tồn tại:
    - Cập nhật `quantity` và `totalPrice = price * quantity`
    - Nếu `newQty <= 0`, gọi `hashTableDelete`
  - Trả 0 nếu thành công, -1 nếu không tìm thấy

- `OrderItem* hashTableToArray(HashTable* ht, int* count)`
  - Tạo array `malloc(ht->count * sizeof(OrderItem))`
  - Duyệt mỗi bucket và node, copy item vào array
  - Gán `*count = ht->count`
  - Trả con trỏ array cho caller

- `void hashTableFree(HashTable* ht)`
  - Duyệt từng bucket
  - Free tất cả node trong chain
  - Free `ht->table` và `ht`

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
DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position);

// Lấy item tại vị trí (pointer)
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
- Append: Tạo node mới, đặt `position = count + 1`, nối vào `tail`
- RemoveAt: Tìm node theo position, unlink `prev->next` và `next->prev`, free node
- UpdateQuantity: Lấy node theo position, cập nhật `quantity`, `totalPrice`
- GetAt: Duyệt từ đầu hoặc từ cuối nếu position gần đuôi
- UpdatePositions: Sau xóa, đọc lại toàn bộ danh sách và đánh lại vị trí
- Free: Free cả danh sách

#### 3.2.3 Chi tiết hàm trong `doubly_linked_list_service`
- `DoublyLinkedList* dllCreate()`
  - Cấp phát struct, head/tail = NULL, count = 0

- `int dllAppend(DoublyLinkedList* dll, OrderItem* item)`
  - Tạo node mới, copy `*item`
  - `position = dll->count + 1`
  - Nếu dll rỗng: head = tail = node
  - Ngược lại: nối vào tail, cập nhật tail
  - Tăng count
  - Trả 0 nếu thành công, -1 nếu lỗi

- `DLLNode* dllGetNodeAt(DoublyLinkedList* dll, int position)`
  - Nếu position <= count/2, duyệt từ `head`
  - Ngược lại duyệt từ `tail`
  - Trả pointer node hoặc NULL

- `OrderItem* dllGetAt(DoublyLinkedList* dll, int position)`
  - Dùng `dllGetNodeAt`, trả `&node->item`

- `int dllRemoveAt(DoublyLinkedList* dll, int position)`
  - Tìm node bằng `dllGetNodeAt`
  - Nếu node tồn tại:
    - Nếu node->prev != NULL, node->prev->next = node->next
    - Nếu node->next != NULL, node->next->prev = node->prev
    - Nếu node là head/tail, cập nhật head/tail
    - Free node
    - Giảm count
    - Gọi `dllUpdatePositions(dll)`
    - Trả 0
  - Nếu không, trả -1

- `int dllUpdateQuantity(DoublyLinkedList* dll, int position, int newQty)`
  - Tìm node
  - Nếu found: node->item.quantity = newQty; node->item.totalPrice = newQty * node->item.price;
  - Trả 0 nếu thành công, -1 nếu không tìm thấy

- `void dllUpdatePositions(DoublyLinkedList* dll)`
  - Duyệt từ `head`, gán position lần lượt 1..count

- `void dllFree(DoublyLinkedList* dll)`
  - Duyệt và free cung toàn bộ node
  - Free struct dll

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

#### 3.3.1 Các struct mới và vị trí file
- `models.h`: chỉ định định nghĩa `HashTable` và `DoublyLinkedList` qua include header service.
- `app/services/hash_table_service.h`: định nghĩa `HashNode`, `HashTable`, prototype hàm hash table.
- `app/services/doubly_linked_list_service.h`: định nghĩa `DLLNode`, `DoublyLinkedList`, prototype hàm list.
- `models.h` cần `#include "../services/hash_table_service.h"` và `#include "../services/doubly_linked_list_service.h"` nếu cần.

#### 3.3.2 Sửa `Bill` struct cụ thể
- `itemsHash`: Lưu trữ dữ liệu chính xác, dùng cho CN1 (đặt món) và CN2 (lưu trữ)
- `cartDisplay`: Mirror của hash table, dùng cho hiển thị & edit giỏ hàng theo STT trong CN2
- `itemCount`: Đồng bộ với `cartDisplay->count` (hoặc `itemsHash->count`)

### 3.4 Chi tiết hàm controller và service cần viết

#### 3.4.1 `app/controllers/order_controller.c`
- `void addToCart(Bill *currentBill)`
  - Input: `currentBill`, người dùng nhập `id`, `quantity`, `option`, `note`
  - Xử lý:
    1. Kiểm tra tồn kho, giới hạn món chính.
    2. Tạo `OrderItem item` từ `menu[id-1]` và ghi chú.
    3. Gọi `hashTableInsert(currentBill->itemsHash, &item)`.
    4. Nếu item mới, gọi `dllAppend(currentBill->cartDisplay, &item)`.
    5. Cập nhật `currentBill->itemCount = currentBill->cartDisplay->count`.
  - Output: cart cập nhật, stock giảm.

- `void removeFromCart(Bill *currentBill)`
  - Input: `currentBill`, STT item muốn xóa.
  - Xử lý:
    1. Tìm `DLLNode* node = dllGetNodeAt(currentBill->cartDisplay, position)`.
    2. Nếu node tồn tại, lưu `itemId` và `quantity`.
    3. Gọi `hashTableDelete(currentBill->itemsHash, itemId)`.
    4. Gọi `dllRemoveAt(currentBill->cartDisplay, position)`.
    5. Trả hàng tồn kho: `menu[itemId-1].stock += quantity`.
    6. Cập nhật `currentBill->itemCount`.
  - Output: giỏ hàng và hash table đồng bộ.

- `void updateQuantity(Bill *currentBill)`
  - Input: `currentBill`, STT item, `newQty`.
  - Xử lý:
    1. Tìm node bằng `dllGetNodeAt`.
    2. Kiểm tra giới hạn món chính và tồn kho.
    3. Gọi `dllUpdateQuantity(currentBill->cartDisplay, position, newQty)`.
    4. Gọi `hashTableUpdateQuantity(currentBill->itemsHash, itemId, newQty)`.
    5. Nếu `newQty == 0`, xóa node và hash table.
    6. Cập nhật `currentBill->itemCount`.

- `void handleOrderMenu(Bill* currentBill)`
  - Input: `currentBill`
  - Xử lý: nếu `currentBill->itemsHash == NULL`, khởi tạo hash table và DLL.
  - Gọi `showMenuUI()` rồi `addToCart(currentBill)`.

- `void handleCartMenu(Bill* currentBill)`
  - Input: `currentBill`
  - Xử lý:
    1. Hiển thị `showCartUI(currentBill)`.
    2. Nhận lựa chọn xóa hoặc sửa.
    3. Gọi `removeFromCart` hoặc `updateQuantity` tương ứng.

#### 3.4.2 `app/ui/cart_ui.c`
- `void showCartUI(Bill *currentBill)`
  - Input: `currentBill`
  - Xử lý:
    1. Nếu `itemCount == 0`, in giỏ trống.
    2. Gọi `hashTableToArray(currentBill->itemsHash, &count)`.
    3. In `STT`, `Ten mon`, `SL`, `Don gia`, `Thanh tien`.
    4. In thêm `option` và `note` nếu có.
    5. Free array sau khi in.
  - Output: hiển thị giỏ hàng từ hash table.

#### 3.4.3 `app/services/checkout_service.c`
- `double calculateSubtotal(Bill* bill)`
  - Input: `bill`
  - Xử lý: lấy array từ `hashTableToArray`, cộng `totalPrice` từng item.
  - Output: subtotal.

- `double getDiscountPercent(char* rank)`
  - Giữ nguyên hàm cũ, không cần thay đổi.

- `void updateCustomerRank(Customer* cust)`
  - Giữ nguyên hàm cũ, nếu có sử dụng `bill->finalPrice`.

#### 3.4.4 `app/services/history_service.c`
- `int addBillToHistory(int customerId, Bill* bill)`
  - Input: `customerId`, `bill`
  - Xử lý:
    1. Tạo node mới.
    2. Copy thông tin bill, nhưng trước đó convert `itemsHash` thành `OrderItem* arr`.
    3. Gán `node->bill.itemCount = count`.
    4. Push node vào `historyHead`.
  - Output: lưu lịch sử trong bộ nhớ.

- `void saveBillToHistoryFile(int customerId, Bill* bill)`
  - Input: `customerId`, `bill`
  - Xử lý: convert hash table thành array, ghi BILL rồi mỗi ITEM.

- `void saveBillToGlobalFile(Bill* bill, int customerId)`
  - Input: `bill`, `customerId`
  - Xử lý: giống `saveBillToHistoryFile`, chỉ khác file lịch sử chung.

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
- **Architecture**: Hash Table cho CN1 (đặt món), Hash Table + DLL cho CN2 (giỏ hàng)

---

**Cập nhật lần cuối**: 09/05/2026
**Người lập**: GitHub Copilot
**Trạng thái**: Planning Phase ✓ Ready for Implementation
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

# 🗂 Hash Table — Giải Thích Chi Tiết Từ Code Dự Án

> Tài liệu này giải thích toàn bộ cách Hash Table được xây dựng và sử dụng trong hệ thống quản lý quán cơm tấm PBL1.  
> Mọi đoạn code đều lấy trực tiếp từ source code thực tế của dự án.

---

## 1. Hash Table Dùng Để Làm Gì?

Trong dự án, Hash Table được dùng để **tra cứu món ăn theo mã ID** từ menu.

| Bài toán | Giải pháp |
|----------|-----------|
| Khách nhập mã món (ví dụ: `5`) → cần lấy thông tin món đó ngay lập tức | Dùng Hash Table để ánh xạ `ID → MenuItem` với tốc độ **O(1)** |

Nếu không có Hash Table, chương trình phải duyệt tuần tự mảng `menu[]` (O(n)) mỗi khi khách nhập mã món.

---

## 2. Cấu Trúc Dữ Liệu

Được định nghĩa trong [`models.h`](../app/models/models.h):

```c
#define HASH_TABLE_SIZE 101

// Một ô trong danh sách liên kết tại mỗi bucket
typedef struct HashNode {
    int key;              // Khóa tra cứu = ID món ăn (1..25)
    MenuItem item;        // Giá trị lưu trữ = thông tin món ăn đầy đủ
    struct HashNode* next; // Con trỏ tới phần tử tiếp theo (Chaining)
} HashNode;

// Bảng băm chính
typedef struct {
    HashNode **table;  // Mảng con trỏ, mỗi phần tử là đầu 1 linked list
    int size;          // Kích thước bảng = HASH_TABLE_SIZE = 101
} HashTable;
```

### Hình minh họa cấu trúc bộ nhớ

```
HashTable
┌──────────────┐
│  table       │──► [ mảng 101 con trỏ HashNode* ]
│  size = 101  │
└──────────────┘

table[0]  ──► NULL
table[1]  ──► HashNode{key=1, item=...} ──► NULL
table[2]  ──► HashNode{key=2, item=...} ──► NULL
  ...
table[5]  ──► HashNode{key=5, item=...} ──► NULL
  ...
table[100] ──► NULL
```

> Vì menu chỉ có **25 món** (ID từ 1 → 25) và bảng có **101 bucket**, mỗi bucket hầu như chỉ chứa **1 phần tử** → tra cứu luôn là **O(1)**.

---

## 3. Hàm Băm (Hash Function)

Được khai báo là `static` trong [`hash_service.c`](../app/services/hash_service.c) — chỉ dùng nội bộ:

```c
static int hash(int key, int size) {
    return key % size;
}
```

### Cách tính toán cụ thể

Với `HASH_TABLE_SIZE = 101` và ID món từ 1 → 25:

| ID món | Phép tính   | Vị trí bucket |
|--------|-------------|---------------|
| 1      | 1 % 101     | **1**         |
| 5      | 5 % 101     | **5**         |
| 12     | 12 % 101    | **12**        |
| 25     | 25 % 101    | **25**        |

> **Nhận xét:** Do ID ≤ 25 < 101, kết quả `key % 101` luôn bằng chính `key`. Không bao giờ xảy ra đụng độ với bộ dữ liệu hiện tại (25 món ≪ 101 bucket).

---

## 4. Tại Sao Chọn `HASH_TABLE_SIZE = 101`?

`101` là **số nguyên tố**. Đây là lựa chọn có chủ đích:

### Lý do dùng số nguyên tố

Khi dùng phép chia lấy dư `key % size`:
- Nếu `size` là số chẵn hoặc có nhiều ước → các key có cùng ước chung sẽ **cùng ánh xạ về một nhóm bucket** → phân phối không đều.
- Nếu `size` là **số nguyên tố** → phân phối đều nhất có thể, giảm thiểu đụng độ.

### So sánh

```
size = 100 (không nguyên tố):
  key=10 → 10 % 100 = 10
  key=110 → 110 % 100 = 10  ← ĐỤNG ĐỘ
  key=210 → 210 % 100 = 10  ← ĐỤNG ĐỘ

size = 101 (số nguyên tố):
  key=10  → 10 % 101 = 10
  key=111 → 111 % 101 = 10  ← chỉ đụng độ khi cách nhau đúng 101
  (với 25 món, điều này không bao giờ xảy ra)
```

### Lý do chọn 101 thay vì số nhỏ hơn (ví dụ 29, 31)

Dù hiện tại chỉ có 25 món, chọn `101` tạo ra **load factor** rất thấp:

```
Load Factor = số phần tử / kích thước bảng
            = 25 / 101
            ≈ 0.25  (rất tốt, lý tưởng là < 0.7)
```

Load factor thấp → xác suất đụng độ gần như bằng 0 → tra cứu luôn O(1).

---

## 5. Xử Lý Đụng Độ — Kỹ Thuật Chaining

Dự án dùng **Separate Chaining** (xử lý đụng độ bằng danh sách liên kết).

### Nguyên lý

Mỗi bucket trong bảng là **đầu của một Singly Linked List**. Khi 2 key băm ra cùng index, chúng được nối vào cùng một danh sách tại bucket đó.

```
Ví dụ nếu có đụng độ tại bucket[5]:

table[5] ──► [key=5, item=...] ──► [key=106, item=...] ──► NULL
                ↑ item thêm sau       ↑ item thêm trước
                (thêm vào đầu)
```

### Code thực tế — `hashTableInsert()`

```c
// File: app/services/hash_service.c, dòng 34-43
void hashTableInsert(HashTable *ht, int key, MenuItem item) {
    if (!ht) return;
    int index = hash(key, ht->size);       // Tính vị trí bucket
    HashNode *node = (HashNode *)malloc(sizeof(HashNode)); // Tạo node mới
    if (!node) return;
    node->key = key;
    node->item = item;
    node->next = ht->table[index];         // ① Trỏ node mới → node cũ đang đứng đầu
    ht->table[index] = node;               // ② Đặt node mới lên đầu bucket
}
```

**Kỹ thuật chèn đầu (Prepend):**
- Node mới luôn được chèn vào **đầu** danh sách tại bucket.
- Ưu điểm: O(1) — không cần duyệt đến cuối.

```
Trước khi insert key=5:
  table[5] ──► NULL

Sau khi insert key=5:
  table[5] ──► [key=5] ──► NULL

Nếu insert key=106 (giả sử 106 % 101 = 5, cũng bucket 5):
  table[5] ──► [key=106] ──► [key=5] ──► NULL
                 ↑ node mới nằm ĐẦU
```

---

## 6. Tìm Kiếm — `hashTableSearch()`

```c
// File: app/services/hash_service.c, dòng 45-56
MenuItem *hashTableSearch(HashTable *ht, int key) {
    if (!ht) return NULL;
    int index = hash(key, ht->size);             // Tính bucket
    HashNode *node = (HashNode *)ht->table[index]; // Lấy đầu linked list
    while (node) {                               // Duyệt linked list
        if (node->key == key) {
            return &node->item;                  // Tìm thấy → trả về con trỏ
        }
        node = node->next;
    }
    return NULL; // Không tìm thấy
}
```

**Luồng tìm kiếm cho `key = 5`:**

```
Bước 1: index = hash(5, 101) = 5
Bước 2: Đến table[5] → lấy đầu linked list
Bước 3: Kiểm tra node đầu: node->key == 5? → Có → return &node->item ✅
```

**Nếu có đụng độ (trường hợp giả định):**

```
table[5] → [key=106] → [key=5] → NULL

Tìm key=5:
  Bước 3a: node->key = 106 ≠ 5 → node = node->next
  Bước 3b: node->key = 5 == 5 → return &node->item ✅
```

---

## 7. Khởi Tạo và Giải Phóng Bộ Nhớ

### Khởi tạo — `createHashTable()`

```c
// File: app/services/hash_service.c, dòng 8-18
HashTable *createHashTable() {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    if (!ht) return NULL;
    ht->size = HASH_TABLE_SIZE;                             // size = 101
    ht->table = (HashNode **)calloc(ht->size, sizeof(HashNode*)); // calloc → tự động NULL hóa
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    return ht;
}
```

> **Lưu ý:** Dùng `calloc` thay vì `malloc` để tự động khởi tạo tất cả 101 bucket về `NULL`. Nếu dùng `malloc`, phải tự `memset` về 0 để tránh con trỏ rác.

### Giải phóng — `freeHashTable()`

```c
// File: app/services/hash_service.c, dòng 20-32
void freeHashTable(HashTable *ht) {
    if (!ht) return;
    for (int i = 0; i < ht->size; i++) {
        HashNode *node = ht->table[i];
        while (node) {
            HashNode *temp = node;
            node = node->next;
            free(temp);         // Giải phóng từng HashNode trong linked list
        }
    }
    free(ht->table);            // Giải phóng mảng con trỏ
    free(ht);                   // Giải phóng struct HashTable
}
```

**Thứ tự giải phóng đúng:**
```
① free từng HashNode trong mỗi bucket
② free(ht->table)   ← mảng 101 con trỏ
③ free(ht)          ← struct HashTable
```
> Nếu đảo ngược thứ tự sẽ gây **memory leak** hoặc **dangling pointer**.

---

## 8. Hash Table Được Dùng Ở Đâu Trong Dự Án?

### Giai đoạn 1 — Nạp dữ liệu (Startup)

Trong [`file_service.c`](../app/services/file_service.c), hàm `loadMenuFromFile()`:

```c
// Dòng 26: Tạo hash table ngay khi chương trình khởi động
menuHashTable = createHashTable();

// Dòng 45: Mỗi món đọc từ file → insert vào hash table
hashTableInsert(menuHashTable, menu[i].id, menu[i]);
```

**Luồng:**
```
main.c → loadMenuFromFile("app/database/menu.txt")
              │
              ├─ Đọc từng dòng file menu.txt
              ├─ Parse: ID | Tên | Giá | HasOptions
              └─ hashTableInsert(menuHashTable, menu[i].id, menu[i])
                      │
                      └─ 25 món được đưa vào 25 bucket khác nhau
```

### Giai đoạn 2 — Khách đặt món (Runtime)

Trong [`order_controller.c`](../app/controllers/order_controller.c):

```c
// Dòng 112 — handleOrderMenu(): Khách nhập mã món
int idNhap = getValidInt(">> Nhap Ma Mon: ");

MenuItem *menuItem = hashTableSearch(menuHashTable, idNhap);
if (!menuItem) {
    printf("  !! Loi: Ma mon ko hop le!\n");
    continue;
}
// Nếu tìm thấy → dùng menuItem->name, menuItem->price, menuItem->hasOptions
```

### Giai đoạn 3 — Cập nhật giỏ hàng

Trong cùng file `order_controller.c`:

```c
// Dòng 41 — removeFromCart(): Xóa món khỏi giỏ
MenuItem *menuItem = hashTableSearch(menuHashTable, node->item.id);

// Dòng 68 — updateQuantity(): Cập nhật số lượng
MenuItem *menuItem = hashTableSearch(menuHashTable, node->item.id);
```

> Khi xóa/sửa món trong giỏ, cần lấy lại thông tin gốc từ menu (ví dụ: `hasOptions`, `price`) → dùng Hash Table để tra cứu nhanh.

---

## 9. Tổng Kết: Sơ Đồ Hoạt Động Đầy Đủ

```
STARTUP
  loadMenuFromFile()
       │
       ├─ [menu.txt] ID=1|Cơm Sườn Cốt Lết|45000|1
       ├─ [menu.txt] ID=2|Cơm Tấm Bì|40000|0
       │         ...25 dòng...
       │
       └─ hashTableInsert(menuHashTable, id, item) × 25
                │
                ▼
          HASH TABLE (101 buckets)
          ┌─────────────────────────────┐
          │ bucket[1]  → {id=1, "Cơm Sườn..."} │
          │ bucket[2]  → {id=2, "Cơm Tấm Bì"}  │
          │ bucket[3]  → {id=3, ...}             │
          │    ...                               │
          │ bucket[25] → {id=25, "Trà đá"}       │
          │ bucket[26] → NULL                    │
          │    ...                               │
          │ bucket[100]→ NULL                    │
          └─────────────────────────────┘

RUNTIME — Khách nhập mã món "12"
  hashTableSearch(menuHashTable, 12)
       │
       ├─ index = 12 % 101 = 12
       ├─ Đến bucket[12]
       ├─ node->key == 12 ✅
       └─ return &node->item  →  O(1) ⚡
```

---

## 10. Độ Phức Tạp

| Thao tác            | Trung bình | Xấu nhất | Ghi chú |
|---------------------|------------|-----------|---------|
| `hashTableInsert()` | **O(1)**   | O(n)      | Chèn vào đầu bucket |
| `hashTableSearch()` | **O(1)**   | O(n)      | Xấu nhất khi mọi key cùng bucket |
| `freeHashTable()`   | O(n)       | O(n)      | Phải duyệt hết |
| Khởi tạo bảng       | O(m)       | O(m)      | m = HASH_TABLE_SIZE = 101 |

> Với 25 món và 101 bucket → **Load Factor = 0.25** → thực tế luôn là **O(1)**.

---

## 11. Điểm Mạnh & Hạn Chế

### ✅ Điểm mạnh

| # | Ưu điểm |
|---|---------|
| 1 | Tra cứu O(1) — cực kỳ nhanh khi khách nhập mã món |
| 2 | Dùng `calloc` → tự động NULL hóa toàn bộ bảng, an toàn |
| 3 | Có `freeHashTable()` → không bị memory leak |
| 4 | Số nguyên tố 101 → phân phối đều, tránh clustering |
| 5 | `static int hash()` → ẩn implementation detail, đóng gói tốt |

### ⚠️ Hạn chế / Có thể cải thiện

| # | Hạn chế | Mức độ | Gợi ý cải thiện |
|---|---------|--------|-----------------|
| 1 | Hash Table chỉ lưu bản sao `MenuItem` (copy by value), không phải con trỏ → tốn bộ nhớ hơn nếu struct lớn | Thấp | Lưu `MenuItem*` thay vì `MenuItem` |
| 2 | Không hỗ trợ xóa một phần tử cụ thể (`hashTableDelete`) | Thấp | Thêm hàm delete nếu menu có thể thay đổi khi chạy |
| 3 | `HASH_TABLE_SIZE` hard-code = 101, không linh hoạt | Thấp | Tính động dựa theo số món thực tế |
| 4 | Nếu menu mở rộng lên > 101 món → tăng đụng độ đáng kể | Trung bình | Dùng dynamic resizing (rehashing) |

---

*📝 Tài liệu phân tích Hash Table — PBL1 Hệ Thống Quản Lý Quán Cơm Tấm*  
*🗓 Ngày tạo: 23/06/2026*

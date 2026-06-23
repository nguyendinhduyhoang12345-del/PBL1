# CHỨC NĂNG 1: ĐẶT MÓN

## I. TỔNG QUÁT

**Tên chức năng:** Đặt Món  
**Lựa chọn:** Case 1 trong menu chính  
**Mô tả:** Cho phép khách hàng xem menu và thêm các món ăn vào giỏ hàng. Có giới hạn số lượng món chính (tối đa 5 cái), trong khi món thêm và nước không giới hạn.

---

## II. GIỚI HẠNÁC ỨNG DỤNG

- **Món chính (ID 1-12):** Tối đa 5 cái
- **Món thêm & Nước (ID 13-25):** Không giới hạn
- **Tuỳ chọn:** Một số món có thể chọn Cot let hoặc Suon cay (+10,000 VND)
- **Ghi chú:** Có thể thêm ghi chú cho từng món

---

## III. CẤU TRÚC DỮ LIỆU CHÍNH

### MenuItem (Mục thực đơn)
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

### OrderItem (Mục hóa đơn)
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

### Cart (Giỏ hàng)
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

---

## IV. HÀM CHÍNH

### `handleOrderMenu(Bill* currentBill)` - Controller

**Vị trí:** `app/controllers/order_controller.c`  
**Mô tả:** Hiển thị menu và xử lý vòng lặp nhập đơn

**Luồng:**
```
1. Gọi showMenuUI() - Hiển thị danh sách menu từ Hash Table
2. Vòng lặp nhập đơn:
   - Hiển thị trạng thái món chính: "Mon chinh: X/5"
   - Nhập ID món
   - Nếu nhập 0: Thoát
   - Tìm món trong Hash Table: hashTableSearch(menuHashTable, idNhap)
   - Xác thực ID và số lượng
   - Kiểm tra số lượng không vượt quá giới hạn
   - Xử lý tuỳ chọn (nếu có)
   - Nhập ghi chú
   - Tạo OrderItem và gọi addToCart()
```

---

## V. HÀM PHỤ TRỢ

### 1. `addToCart(Bill *currentBill, OrderItem item)` - Controller

**Vị trí:** `app/controllers/order_controller.c`  
**Mô tả:** Thêm mục vào giỏ hàng hoặc cộng số lượng nếu món đã tồn tại

**Pseudocode:**
```
addToCart(bill, item):
  node = bill->cart.head
  WHILE node != NULL:
    IF node->item.id == item.id AND
       node->item.option == item.option AND
       strcmp(node->item.note, item.note) == 0:
      node->item.quantity += item.quantity
      node->item.totalPrice = node->item.quantity * node->item.price
      RETURN
    node = node->next
  
  # Nếu chưa tồn tại thì thêm mới
  cartAddItem(&bill->cart, item)
```

**Ý tưởng:** Kiểm tra xem món với cùng ID, tuỳ chọn, và ghi chú đã tồn tại hay chưa:
- Nếu có: Cộng số lượng
- Nếu chưa: Tạo mới qua `cartAddItem()`

### 2. `hashTableSearch(HashTable *ht, int key)` - Service

**Vị trị:** `app/services/hash_service.c`  
**Mô tả:** Tìm kiếm món ăn trong Hash Table

**Pseudocode:**
```
hashTableSearch(ht, key):
  index = hash(key, ht->size)  // key % size
  node = ht->table[index]
  
  WHILE node != NULL:
    IF node->key == key:
      RETURN &node->item
    node = node->next
  
  RETURN NULL
```

**Độ phức tạp:** O(1) trung bình

### 3. `cartAddItem(Cart *cart, OrderItem item)` - Service

**Vị trí:** `app/services/cart_service.c`  
**Mô tả:** Thêm mục vào cuối linked list

**Pseudocode:**
```
cartAddItem(cart, item):
  node = malloc(sizeof(CartNode))
  node->item = item
  node->prev = cart->tail
  node->next = NULL
  
  IF cart->tail != NULL:
    cart->tail->next = node
  
  cart->tail = node
  
  IF cart->head == NULL:
    cart->head = node
  
  cart->itemCount++
```

**Độ phức tạp:** O(1)

### 4. `showMenuUI()` - UI

**Vị trí:** `app/ui/menu_ui.c`  
**Mô tả:** Hiển thị danh sách menu từ array

**In ra:**
```
=========== DANH SACH MENU ===========
1. Sườn Nướng - 35.000 VND (Có tuỳ chọn)
2. Sườn Kho - 32.000 VND
...
25. Nước Ép - 15.000 VND
=====================================
```

---

## VI. LUỒNG HOẠT ĐỘNG CHI TIẾT

```
1. Hiển thị Menu:
   ├─ Gọi showMenuUI()
   ├─ Duyệt menu[] array
   └─ In từng món: ID, Tên, Giá, Tuỳ chọn
   
2. Vòng Lặp Chính:
   ├─ Hiển thị trạng thái: "Mon chinh: X/5"
   ├─ Nhập ID Món:
   │  ├─ getValidInt(">> Nhap Ma Mon: ")
   │  ├─ Nếu nhập 0 → Thoát vòng lặp
   │  └─ Xác thực 1 ≤ ID ≤ 25
   │
   ├─ Tìm Món:
   │  ├─ menuItem = hashTableSearch(menuHashTable, idNhap)
   │  ├─ Nếu NOT FOUND → Báo lỗi, lặp lại
   │  └─ Nếu tìm thấy → Tiếp tục
   │
   ├─ Kiểm Tra Giới Hạn Món Chính:
   │  ├─ Nếu ID ≤ 12 (Món chính):
   │  │  ├─ Đếm số lượng món chính hiện tại: countMain
   │  │  ├─ Nếu countMain ≥ 5 → Báo "Ban da dat du 5 mon chinh"
   │  │  └─ Lặp lại
   │  └─ Nếu ID > 12 → Không hạn chế
   │
   ├─ Nhập Số Lượng:
   │  ├─ getValidInt(">> Nhap so luong: ")
   │  ├─ Xác thực 1 ≤ slNhap ≤ 5
   │  ├─ Nếu ID ≤ 12 AND countMain + slNhap > 5 → Báo lỗi
   │  └─ Nếu hợp lệ → Tiếp tục
   │
   ├─ Xử Lý Tuỳ Chọn (Nếu menuItem->hasOptions == 1):
   │  ├─ In: "1. Cot let | 2. Suon cay (+10k)"
   │  ├─ getValidInt(">> Chon loai: ")
   │  ├─ Nếu opt == 2 → giaThucTe += 10000
   │  └─ Tiếp tục
   │
   ├─ Nhập Ghi Chú:
   │  ├─ printf(">> Ghi chu (Bam Enter bo qua): ")
   │  ├─ fgets(ghiChu, sizeof(ghiChu), stdin)
   │  └─ Xóa ký tự newline
   │
   ├─ Tạo OrderItem:
   │  ├─ newItem.id = menuItem->id
   │  ├─ strcpy(newItem.name, menuItem->name)
   │  ├─ newItem.price = giaThucTe
   │  ├─ newItem.quantity = slNhap
   │  ├─ newItem.option = opt
   │  ├─ strcpy(newItem.note, ghiChu)
   │  └─ newItem.totalPrice = giaThucTe * slNhap
   │
   └─ Thêm Vào Giỏ:
      ├─ addToCart(currentBill, newItem)
      ├─ Hiển thị "=> Da them X 'TenMon' vao gio!"
      └─ Lặp lại bước 2

3. Thoát Vòng Lặp:
   └─ Trả về menu chính (handleOrderMenu() kết thúc)
```

---

## VII. PSEUDOCODE ĐẦY ĐỦ

```c
void handleOrderMenu(Bill* currentBill) {
    showMenuUI();  // Hiển thị danh sách menu
    
    while (1) {
        // Đếm số lượng món chính hiện tại
        int countMain = 0;
        CartNode *tmp = currentBill->cart.head;
        while (tmp) {
            if (tmp->item.id <= 12) countMain += tmp->item.quantity;
            tmp = tmp->next;
        }
        
        printf("\n>> (Mon chinh: %d/5 | Mon them & Nuoc: Khong gioi han, Nhap 0 de thoat) : \n", countMain);
        
        // 1. Nhập ID Món
        int idNhap = getValidInt(">> Nhap Ma Mon: ");
        if (idNhap == 0) break;  // Thoát nếu nhập 0
        
        // 2. Tìm Món
        MenuItem *menuItem = hashTableSearch(menuHashTable, idNhap);
        if (!menuItem || idNhap < 1 || idNhap > TONG_SO_MON) {
            printf("  !! Loi: Ma mon ko hop le!\n");
            continue;
        }
        
        // 3. Kiểm Tra Giới Hạn Món Chính
        if (idNhap <= 12 && countMain >= 5) {
            printf("  !! Thong bao: Ban da dat du 5 mon chinh.\n");
            continue;
        }
        
        // 4. Nhập Số Lượng
        int slNhap = getValidInt(">> Nhap so luong: ");
        if (slNhap <= 0 || slNhap > 5) {
            printf("  !! Loi: So luong khong hop le (Toi da 5).\n");
            continue;
        }
        
        if (idNhap <= 12 && countMain + slNhap > 5) {
            printf("  !! Loi: Tong so mon chinh khong duoc vuot qua 5 (Dang co: %d).\n", countMain);
            continue;
        }
        
        // 5. Xử Lý Tuỳ Chọn
        int opt = 0;
        double giaThucTe = menuItem->price;
        
        if (menuItem->hasOptions) {
            while (1) {
                printf("  1. Cot let | 2. Suon cay (+10k)\n");
                opt = getValidInt("  >> Chon loai: ");
                if (opt == 1 || opt == 2) {
                    if (opt == 2) giaThucTe += 10000;
                    break;
                } else {
                    printf("  !! Loi: Chi chon 1 hoac 2.\n");
                }
            }
        }
        
        // 6. Nhập Ghi Chú
        char ghiChu[100] = "";
        printf("  >> Ghi chu (Bam Enter bo qua): ");
        fgets(ghiChu, sizeof(ghiChu), stdin);
        ghiChu[strcspn(ghiChu, "\n")] = 0;
        
        // 7. Tạo OrderItem
        OrderItem newItem;
        newItem.id = menuItem->id;
        strcpy(newItem.name, menuItem->name);
        newItem.price = giaThucTe;
        newItem.quantity = slNhap;
        newItem.option = opt;
        strcpy(newItem.note, ghiChu);
        newItem.totalPrice = giaThucTe * slNhap;
        
        // 8. Thêm Vào Giỏ
        printf("  => Da them %d '%s'", slNhap, menuItem->name);
        if (menuItem->hasOptions) {
            if (opt == 1) printf(" (Cot let)");
            else printf(" (Suon cay)");
        }
        printf(" vao gio!\n");
        
        addToCart(currentBill, newItem);
    }
}
```

---

## VIII. CẤU TRÚC FILE DỮ LIỆU

### `app/database/menu.txt` - Danh sách menu
```
1|Sườn Nướng|35000|1
2|Sườn Kho|32000|0
3|Cút Nướng|28000|1
4|Cút Kho|26000|0
5|Thịt Lợn Nướng|30000|0
6|Thịt Lợn Kho|28000|0
7|Cá Nướng|32000|0
8|Tôm Nướng|38000|0
9|Tôm Kho Tàu|35000|0
10|Mực Nướng|36000|0
11|Gà Nướng|29000|0
12|Trứng Chiên|12000|0
13|Canh Chua|8000|0
14|Canh Mướp|7000|0
15|Rau Luộc|9000|0
16|Dưa Muối|6000|0
17|Trứng Cuon|10000|0
18|Chả Giò|11000|0
19|Cơm|5000|0
20|Xôi|8000|0
21|Pepsi|15000|0
22|Sprite|15000|0
23|Nước Chanh|12000|0
24|Nước Mía|15000|0
25|Nước Ép|18000|0
```

**Format:** `ID|Name|Price|HasOptions`
- `HasOptions`: 1 = Có tuỳ chọn, 0 = Không có

---

## IX. FILE LIÊN QUAN

| File | Vị trí | Chức năng |
|------|--------|----------|
| order_controller.c | app/controllers/ | Hàm chính `handleOrderMenu()`, `addToCart()` |
| hash_service.h/c | app/services/ | Định nghĩa và xử lý Hash Table |
| cart_service.h/c | app/services/ | Định nghĩa và xử lý Doubly Linked List |
| menu_ui.h/c | app/ui/ | Hiển thị menu |
| models.h | app/models/ | Định nghĩa struct MenuItem, OrderItem, Cart |
| validator.h/c | app/utils/ | Xác thực input |

---

## X. LƯỚI TƯƠNG TÁC

```
handleOrderMenu()
│
├─→ showMenuUI()
│   └─ In danh sách menu từ array
│
├─→ hashTableSearch()
│   └─ Tìm món ăn O(1)
│
├─→ addToCart()
│   └─ Thêm vào linked list
│       ├─→ cartAddItem()
│       └─→ Cập nhật số lượng (nếu trùng)
│
└─→ getValidInt()
    └─ Xác thực input
```

---

## XI. TÓM TẮT

| Khía cạnh | Chi tiết |
|-----------|---------|
| **Cấu trúc dữ liệu** | Hash Table (menu), Doubly Linked List (giỏ) |
| **Độ phức tạp** | O(1) - Hash Table + O(1) - Add to cart |
| **Giới hạn** | 5 món chính, không hạn chế món thêm |
| **Tuỳ chọn** | Cot let / Suon cay (+10k) |
| **Ghi chú** | Có thể thêm ghi chú tùy ý |
| **Trạng thái** | Giỏ hàng lưu trữ trong RAM (Bill struct) |

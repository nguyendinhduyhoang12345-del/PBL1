# CHỨC NĂNG 2: XEM VÀ CHỈNH SỬA GIỎ HÀNG

## I. TỔNG QUÁT

**Tên chức năng:** Xem và Chỉnh Sửa Giỏ Hàng  
**Lựa chọn:** Case 2 trong menu chính  
**Mô tả:** Hiển thị giỏ hàng hiện tại và cho phép xóa hoặc thay đổi số lượng các món đã chọn. Đảm bảo tuân thủ giới hạn số lượng món chính (≤ 5).

---

## II. GIỚI HẠN & ĐIỀU KIỆN

- **Xóa món:** Có thể xóa bất kỳ món nào
- **Sửa số lượng:** 
  - Phải > 0
  - Nếu là món chính (ID ≤ 12), tổng sau sửa không được vượt quá 5
  - Nếu là món thêm (ID > 12), không hạn chế

---

## III. CẤU TRÚC DỮ LIỆU CHÍNH

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

---

## IV. HÀM CHÍNH

### `handleCartMenu(Bill* currentBill)` - Controller

**Vị trí:** `app/controllers/order_controller.c`  
**Mô tả:** Hiển thị giỏ hàng và menu con cho phép xóa hoặc sửa

**Luồng:**
```
1. Vòng lặp con:
   - Gọi showCartUI(currentBill) - Hiển thị giỏ
   - Hiển thị menu: 1. Xóa | 2. Sửa | 0. Quay lại
   - Nhập lựa chọn
   - Xử lý theo lựa chọn
   - Lặp lại đến khi chọn 0
```

---

## V. HÀM PHỤ TRỢ

### 1. `removeFromCart(Bill *currentBill)` - Controller

**Vị trí:** `app/controllers/order_controller.c`  
**Mô tả:** Xóa một mục khỏi giỏ hàng theo STT

**Pseudocode:**
```
removeFromCart(bill):
  IF bill->cart.itemCount == 0:
    RETURN  // Giỏ rỗng
  
  stt = getValidInt(">> Nhap STT mon muon xoa: ")
  
  IF stt < 1 OR stt > bill->cart.itemCount:
    printf("!! STT khong hop le.")
    RETURN
  
  idx = stt - 1
  node = cartGetNodeByIndex(&bill->cart, idx)
  
  IF node == NULL:
    printf("!! Loi: Khong tim thay mon de xoa.")
    RETURN
  
  cartRemoveItemByIndex(&bill->cart, idx)
  printf("=> Da xoa mon!")
```

**Bước chi tiết:**
1. Kiểm tra giỏ rỗng
2. Nhập STT (1 đến itemCount)
3. Xác thực STT
4. Lấy node tại index (stt - 1)
5. Gọi `cartRemoveItemByIndex()` để xóa
6. Hiển thị thành công

### 2. `updateQuantity(Bill *currentBill)` - Controller

**Vị trí:** `app/controllers/order_controller.c`  
**Mô tả:** Cập nhật số lượng của một mục

**Pseudocode:**
```
updateQuantity(bill):
  IF bill->cart.itemCount == 0:
    RETURN  // Giỏ rỗng
  
  stt = getValidInt(">> Nhap STT mon muon sua SL: ")
  
  IF stt < 1 OR stt > bill->cart.itemCount:
    printf("!! STT khong hop le.")
    RETURN
  
  idx = stt - 1
  node = cartGetNodeByIndex(&bill->cart, idx)
  
  IF node == NULL:
    printf("!! Loi: Khong tim thay mon de sua.")
    RETURN
  
  newQty = getValidInt(">> Nhap so luong moi: ")
  
  IF newQty <= 0:
    printf("!! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.")
    RETURN
  
  # Nếu là món chính (ID ≤ 12): Kiểm tra tổng
  IF node->item.id <= 12:
    countMain = 0
    tmp = bill->cart.head
    WHILE tmp != NULL:
      IF tmp->item.id <= 12:
        countMain += tmp->item.quantity
      tmp = tmp->next
    
    countMain = countMain - oldQty + newQty
    
    IF countMain > 5:
      printf("!! Loi: Tong so mon chinh khong duoc vuot qua 5.")
      RETURN
  
  # Cập nhật
  cartUpdateQuantityByIndex(&bill->cart, idx, newQty)
  printf("=> Da cap nhat so luong!")
```

**Bước chi tiết:**
1. Kiểm tra giỏ rỗng
2. Nhập STT
3. Xác thực STT
4. Nhập số lượng mới (> 0)
5. **Kiểm tra ràng buộc:** Nếu là món chính:
   - Đếm tổng số mon chinh hiện tại
   - Trừ đi số lượng cũ, cộng số lượng mới
   - Nếu vượt 5 → Báo lỗi & thoát
6. Cập nhật via `cartUpdateQuantityByIndex()`
7. Hiển thị thành công

### 3. `showCartUI(Bill* currentBill)` - UI

**Vị trí:** `app/ui/cart_ui.c`  
**Mô tả:** Hiển thị danh sách các mục trong giỏ hàng

**In ra:**
```
========== GIO HANG ==========
STT | Ten              | Gia   | SL | Tong
----|------------------|-------|----|---------
1   | Sườn Nướng (Suon) | 45000 | 2  | 90000
2   | Canh Chua        | 8000  | 1  | 8000
3   | Cơm              | 5000  | 2  | 10000
----|------------------|-------|----|---------
    | TONG CONG        |       |    | 108000 VND
==============================
```

### 4. `cartGetNodeByIndex(Cart *cart, int index)` - Service

**Vị trí:** `app/services/cart_service.c`  
**Mô tả:** Lấy node tại vị trí chỉ định

**Pseudocode:**
```c
CartNode *cartGetNodeByIndex(Cart *cart, int index) {
    if (!cart || index < 0 || index >= cart->itemCount) 
        return NULL;
    
    CartNode *node = cart->head;
    for (int i = 0; i < index; i++) 
        node = node->next;
    
    return node;
}
```

**Độ phức tạp:** O(k) - k là vị trí item

### 5. `cartRemoveItemByIndex(Cart *cart, int index)` - Service

**Vị trí:** `app/services/cart_service.c`  
**Mô tả:** Xóa node tại vị trí chỉ định từ linked list

**Pseudocode:**
```c
int cartRemoveItemByIndex(Cart *cart, int index) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (!node) return 0;  // Thất bại
    
    // Cập nhật prev->next
    if (node->prev) 
        node->prev->next = node->next;
    else 
        cart->head = node->next;  // Node là đầu
    
    // Cập nhật next->prev
    if (node->next) 
        node->next->prev = node->prev;
    else 
        cart->tail = node->prev;  // Node là đuôi
    
    free(node);
    cart->itemCount--;
    return 1;  // Thành công
}
```

**Độ phức tạp:** O(k)

### 6. `cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity)` - Service

**Vị trí:** `app/services/cart_service.c`  
**Mô tả:** Cập nhật số lượng của item tại vị trí

**Pseudocode:**
```c
int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (!node) return 0;  // Thất bại
    
    node->item.quantity = newQuantity;
    node->item.totalPrice = node->item.price * newQuantity;
    return 1;  // Thành công
}
```

**Độ phức tạp:** O(k)

---

## VI. LUỒNG HOẠT ĐỘNG CHI TIẾT

```
handleCartMenu()
│
└─→ Vòng Lặp Con:
    │
    ├─→ showCartUI(currentBill)
    │   └─ Hiển thị:
    │       - Danh sách item: STT, Tên, Giá, SL, Tổng
    │       - Tổng cộng (tất cả item)
    │
    ├─→ Menu Con:
    │   "1. Xoa mon khoi gio"
    │   "2. Thay doi so luong"
    │   "0. Quay lai Menu chinh"
    │
    ├─→ Lựa Chọn:
    │   │
    │   ├─ Case 1: removeFromCart()
    │   │   ├─ Nhập STT (1 → itemCount)
    │   │   ├─ Xác thực
    │   │   ├─ cartGetNodeByIndex() → Lấy node
    │   │   ├─ Cập nhật linked list:
    │   │   │   - prev->next = node->next
    │   │   │   - next->prev = node->prev
    │   │   ├─ free(node)
    │   │   ├─ itemCount--
    │   │   └─ In "=> Da xoa mon!"
    │   │
    │   ├─ Case 2: updateQuantity()
    │   │   ├─ Nhập STT
    │   │   ├─ Xác thực
    │   │   ├─ Nhập số lượng mới (> 0)
    │   │   ├─ Nếu ID ≤ 12 (Món chính):
    │   │   │   ├─ Đếm countMain (Duyệt linked list)
    │   │   │   ├─ countMain = countMain - oldQty + newQty
    │   │   │   └─ Nếu countMain > 5 → Báo lỗi & RETURN
    │   │   ├─ cartUpdateQuantityByIndex()
    │   │   │   ├─ node->item.quantity = newQuantity
    │   │   │   └─ node->item.totalPrice = price * quantity
    │   │   └─ In "=> Da cap nhat so luong!"
    │   │
    │   └─ Case 0: break  (Thoát vòng lặp)
    │
    └─→ Lặp Lại Đến Khi Chọn 0
```

---

## VII. PSEUDOCODE ĐẦY ĐỦ

```c
void handleCartMenu(Bill* currentBill) {
    while (1) {
        showCartUI(currentBill);  // Hiển thị giỏ
        
        printf("\n[THAO TAC GIO HANG]\n");
        printf("1. Xoa mon khoi gio\n");
        printf("2. Thay doi so luong\n");
        printf("0. Quay lai Menu chinh\n");
        
        int subChoice = getValidInt("-> Chon: ");
        
        if (subChoice == 1) {
            removeFromCart(currentBill);
        } 
        else if (subChoice == 2) {
            updateQuantity(currentBill);
        } 
        else if (subChoice == 0) {
            break;  // Thoát vòng lặp
        }
        else {
            printf("\n[!] Lua chon khong hop le.\n");
        }
    }
}

void removeFromCart(Bill *currentBill) {
    if (currentBill->cart.itemCount == 0) {
        printf("[!] Gio hang trong!\n");
        return;
    }
    
    int stt = getValidInt(">> Nhap STT mon muon xoa: ");
    if (stt < 1 || stt > currentBill->cart.itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }
    
    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    if (!node) {
        printf("  !! Loi: Khong tim thay mon de xoa.\n");
        return;
    }
    
    cartRemoveItemByIndex(&currentBill->cart, idx);
    printf("  => Da xoa mon!\n");
}

void updateQuantity(Bill *currentBill) {
    if (currentBill->cart.itemCount == 0) {
        printf("[!] Gio hang trong!\n");
        return;
    }
    
    int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
    if (stt < 1 || stt > currentBill->cart.itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }
    
    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    if (!node) {
        printf("  !! Loi: Khong tim thay mon de sua.\n");
        return;
    }
    
    int newQty = getValidInt(">> Nhap so luong moi: ");
    if (newQty <= 0) {
        printf("  !! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.\n");
        return;
    }
    
    // Kiểm tra nếu là món chính
    int oldQty = node->item.quantity;
    if (node->item.id <= 12) {  // Món chính
        int countMain = 0;
        CartNode *tmp = currentBill->cart.head;
        while (tmp) {
            if (tmp->item.id <= 12) 
                countMain += tmp->item.quantity;
            tmp = tmp->next;
        }
        
        countMain = countMain - oldQty + newQty;
        if (countMain > 5) {
            printf("  !! Loi: Tong so mon chinh khong duoc vuot qua 5.\n");
            return;
        }
    }
    
    cartUpdateQuantityByIndex(&currentBill->cart, idx, newQty);
    printf("  => Da cap nhat so luong!\n");
}
```

---

## VIII. LƯỚI CẤU TRÚC LINKED LIST

### Ví dụ Giỏ Hàng:
```
Mục 1                  Mục 2                  Mục 3
┌──────────────┐     ┌──────────────┐      ┌──────────────┐
│ Sườn Nướng   │◄───►│ Canh Chua    │◄────►│ Cơm          │
│ Qty: 2       │     │ Qty: 1       │      │ Qty: 2       │
│ 90.000 VND   │     │ 8.000 VND    │      │ 10.000 VND   │
│ prev→NULL    │     │              │      │ next→NULL    │
└──────────────┘     └──────────────┘      └──────────────┘
      ▲                                            ▲
      │ head                                       │ tail
      └────────────────────────────────────────────┘
                itemCount = 3
```

### Xóa Mục 2:
```
TRƯỚC:
Mục 1 ◄───► Mục 2 (XÓA) ◄───► Mục 3

SAU:
Mục 1 ◄─────────────────────► Mục 3
  - mục1->next = mục3
  - mục3->prev = mục1
  - free(mục2)
  - itemCount-- (3 → 2)
```

---

## IX. FILE LIÊN QUAN

| File | Vị trí | Chức năng |
|------|--------|----------|
| order_controller.c | app/controllers/ | `handleCartMenu()`, `removeFromCart()`, `updateQuantity()` |
| cart_service.h/c | app/services/ | `cartGetNodeByIndex()`, `cartRemoveItemByIndex()`, `cartUpdateQuantityByIndex()` |
| cart_ui.h/c | app/ui/ | `showCartUI()` |
| models.h | app/models/ | Định nghĩa struct Cart, CartNode, OrderItem |
| validator.h/c | app/utils/ | `getValidInt()` |

---

## X. TÓM TẮT

| Khía cạnh | Chi tiết |
|-----------|---------|
| **Cấu trúc dữ liệu** | Doubly Linked List |
| **Độ phức tạp** | O(k) - k là vị trí item |
| **Giới hạn Món Chính** | ≤ 5 khi sửa số lượng |
| **Thao tác** | Xóa, Sửa số lượng |
| **Xác thực** | STT (1 → itemCount), Số lượng > 0 |
| **Trạng thái** | Giỏ hàng lưu trữ trong RAM (Bill struct) |

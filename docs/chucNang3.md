# CHỨC NĂNG 3: NHẬP THÔNG TIN KHÁCH HÀNG

## I. TỔNG QUÁT

**Tên chức năng:** Nhập Thông Tin Khách Hàng  
**Lựa chọn:** Case 3 trong menu chính  
**Mô tả:** Tìm kiếm khách hàng theo số điện thoại. Nếu khách hàng tồn tại: Hiển thị thông tin & hạng VIP. Nếu chưa: Cho phép đăng ký mới với thông tin cơ bản.

---

## II. WORKFLOW CHÍNH

```
Nhập SDT
│
├─ Tìm kiếm B-Tree
│
├─ Khách Cũ
│  ├─ Hiển thị: ID, Tên, SDT, Địa chỉ, Tổng chi tiêu, Hạng VIP
│  └─ Gán vào Bill
│
└─ Khách Mới
   ├─ Menu: 1. Đăng ký / 0. Tiếp tục (Guest)
   │
   └─ Nếu chọn 1:
      ├─ Nhập Tên
      ├─ Nhập Địa chỉ
      ├─ Khởi tạo: ID, Phone, Address, TotalSpent, Rank
      ├─ Thêm vào B-Tree
      ├─ Lưu file
      └─ Gán vào Bill
```

---

## III. CẤU TRÚC DỮ LIỆU CHÍNH

### Customer (Khách hàng)
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

### B-Tree (Cấp 4)
```c
#define BTREE_ORDER 4

typedef struct BTreeNode {
    Customer* customers[BTREE_ORDER - 1];  // Tối đa 3 khóa
    struct BTreeNode* children[BTREE_ORDER];  // Tối đa 4 con
    int numKeys;
    int isLeaf;
} BTreeNode;
```

### Hạng VIP
```
Bronze:   < 1.000.000 VND       → Giảm 0%
Silver:   1.000.000 - 5.000.000 → Giảm 5%
Gold:     5.000.000 - 10.000.000 → Giảm 10%
Diamond:  ≥ 10.000.000 VND      → Giảm 15%
```

---

## IV. HÀM CHÍNH

### `handleCustomer(Bill *currentBill)` - Controller

**Vị trí:** `app/controllers/customer_controller.c`  
**Mô tả:** Xử lý việc nhập thông tin khách hàng (tìm kiếm hoặc đăng ký)

**Luồng:**
```
1. Nhập số điện thoại (xác thực format)
2. Tìm kiếm trong B-Tree: searchBTree(btreeRoot, sdt)
3. Nếu tìm thấy (found != NULL):
   - Hiển thị thông tin
   - Gán vào currentBill->customer
4. Nếu không tìm thấy (found == NULL):
   - Menu: 1. Đăng ký mới | 0. Tiếp tục (Guest)
   - Nếu chọn 1: Đăng ký khách mới
   - Nếu chọn 0: Tiếp tục (Guest)
```

---

## V. HÀM PHỤ TRỢ

### 1. `searchBTree(BTreeNode* root, char* phone)` - Service

**Vị trí:** `app/services/btree_service.c`  
**Mô tả:** Tìm kiếm khách hàng theo số điện thoại trong B-Tree

**Pseudocode:**
```c
Customer* searchBTree(BTreeNode* root, char* phone) {
    if (root == NULL) 
        return NULL;
    
    int i = 0;
    // Tìm vị trí: phone > customers[i]->phone
    while (i < root->numKeys && 
           strcmp(phone, root->customers[i]->phone) > 0) 
        i++;
    
    // Kiểm tra nếu tìm thấy ở vị trí i
    if (i < root->numKeys && 
        strcmp(phone, root->customers[i]->phone) == 0)
        return root->customers[i];
    
    // Nếu là leaf node thì không tìm thấy
    if (root->isLeaf) 
        return NULL;
    
    // Tìm kiếm đệ quy trong child node
    return searchBTree(root->children[i], phone);
}
```

**Độ phức tạp:** O(log n) - Với B-Tree cấp 4

**Ví dụ:**
```
B-Tree:
         [0987]
         /    \
    [0912]   [0989]
    
Tìm 0912:
  - searchBTree(root, "0912")
  - i = 0 ("0912" < "0987")
  - root->children[0] = [0912]
  - Tìm thấy tại index 0
  - Return customers[0] (SDT 0912)
```

### 2. `insertBTree(BTreeNode** root, Customer* cust)` - Service

**Vị trí:** `app/services/btree_service.c`  
**Mô tả:** Thêm khách hàng mới vào B-Tree với tự động cân bằng

**Pseudocode:**
```c
void insertBTree(BTreeNode** root, Customer* cust) {
    if (*root == NULL) {
        // Tạo root mới (leaf node)
        *root = createBTreeNode(1);
        (*root)->customers[0] = cust;
        (*root)->numKeys = 1;
    } else {
        // Nếu root đã đầy (numKeys == BTREE_ORDER - 1)
        if ((*root)->numKeys == BTREE_ORDER - 1) {
            // Tạo node mới làm root
            BTreeNode* s = createBTreeNode(0);
            s->children[0] = *root;
            
            // Chia root
            splitChild(s, 0, *root);
            
            // Chọn child để insert
            int i = 0;
            if (strcmp(cust->phone, s->customers[0]->phone) > 0) 
                i++;
            insertNonFull(s->children[i], cust);
            
            *root = s;
        } else {
            // Root chưa đầy, insert trực tiếp
            insertNonFull(*root, cust);
        }
    }
}
```

**Độ phức tạp:** O(log n)

### 3. `createBTreeNode(int isLeaf)` - Service

**Vị trí:** `app/services/btree_service.c`  
**Mô tả:** Tạo node mới cho B-Tree

**Pseudocode:**
```c
BTreeNode* createBTreeNode(int isLeaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->isLeaf = isLeaf;
    newNode->numKeys = 0;
    
    for (int i = 0; i < BTREE_ORDER; i++) 
        newNode->children[i] = NULL;
    
    return newNode;
}
```

### 4. `showCustomerInfo(Customer* found)` - UI

**Vị trí:** `app/ui/customer_ui.c`  
**Mô tả:** Hiển thị thông tin khách hàng tồn tại

**In ra:**
```
========== KHACH HANG TON TAI ==========
ID:            1
Tên:           Nguyen Van A
Số Điện Thoại: 0987654321
Địa Chỉ:       123 Duong ABC
Tổng Chi Tiêu: 2.500.000 VND
Hạng VIP:      GOLD (Giảm 10%)
========================================
```

### 5. `showCustomerRegistration(Customer* newCust)` - UI

**Vị trí:** `app/ui/customer_ui.c`  
**Mô tả:** Hiển thị thông tin khách hàng đăng ký mới

**In ra:**
```
========== DANG KY THANH CONG ==========
ID:            2
Tên:           Tran Thi B
Số Điện Thoại: 0912345678
Địa Chỉ:       456 Duong XYZ
Tổng Chi Tiêu: 0 VND
Hạng VIP:      BRONZE (Giảm 0%)
=========================================
```

### 6. `saveAllCustomersToFile()` - Service

**Vị trí:** `app/services/file_service.c`  
**Mô tả:** Lưu tất cả khách hàng từ B-Tree vào file

**Pseudocode:**
```
saveAllCustomersToFile():
  file = fopen("app/database/khachhang.txt", "w")
  
  # Duyệt B-Tree theo thứ tự (in-order)
  inOrderTraversal(btreeRoot, file)
  
  fclose(file)

inOrderTraversal(node, file):
  IF node == NULL:
    RETURN
  
  FOR i = 0 TO node->numKeys:
    IF NOT node->isLeaf:
      inOrderTraversal(node->children[i], file)
    
    IF i < node->numKeys:
      # Ghi khách hàng
      fprintf(file, "%d|%s|%s|%s|%.3f|%s\n",
              node->customers[i]->id,
              node->customers[i]->phone,
              node->customers[i]->name,
              node->customers[i]->address,
              node->customers[i]->totalSpent,
              node->customers[i]->rank)
  
  IF NOT node->isLeaf:
    inOrderTraversal(node->children[node->numKeys], file)
```

### 7. `getValidPhoneNumber()` - Utility

**Vị trí:** `app/utils/validator.c`  
**Mô tả:** Xác thực và nhập số điện thoại

**Xác thực:**
- Độ dài: 10-15 ký tự
- Ký tự: Chỉ số (0-9)
- Bắt đầu bằng 0

---

## VI. LUỒNG HOẠT ĐỘNG CHI TIẾT

```
handleCustomer()
│
├─→ 1. Nhập SDT:
│   ├─ printf(">> Nhap So Dien Thoai: ")
│   ├─ getValidPhoneNumber(sdt, sizeof(sdt), prompt)
│   │  ├─ Kiểm tra độ dài: 10-15 ký tự
│   │  ├─ Kiểm tra chỉ số: 0-9
│   │  ├─ Kiểm tra bắt đầu: 0
│   │  └─ Nếu hợp lệ → RETURN
│   └─ sdt đã được xác thực
│
├─→ 2. Tìm Kiếm:
│   ├─ found = searchBTree(btreeRoot, sdt)
│   └─ Tìm kiếm O(log n) trong B-Tree
│
├─→ 3A. Nếu Tìm Thấy (found != NULL):
│   │
│   ├─ showCustomerInfo(found)
│   │  └─ In: ID, Tên, SDT, Địa chỉ, Tổng chi, Hạng
│   │
│   └─ currentBill->customer = *found
│       └─ Gán khách hàng vào hóa đơn hiện tại
│
└─→ 3B. Nếu Không Tìm Thấy (found == NULL):
    │
    ├─ printf("KHACH HANG CHUA CO TRONG HE THONG")
    ├─ Menu: "1. Dang ky moi | 0. Bo qua (Guest)"
    │
    ├─ choice = getValidInt("-> Chon: ")
    │
    └─ IF choice == 1:
       │
       ├─ Tạo Customer Mới:
       │  ├─ newCust.id = soLuongKH + 1
       │  ├─ strcpy(newCust.phone, sdt)
       │  │
       │  ├─ Nhập Tên:
       │  │  ├─ printf(">> Nhap Ten: ")
       │  │  └─ getValidLine(newCust.name, sizeof(newCust.name), prompt)
       │  │
       │  ├─ Nhập Địa Chỉ:
       │  │  ├─ printf(">> Nhap Dia chi: ")
       │  │  └─ getValidLine(newCust.address, sizeof(newCust.address), prompt)
       │  │
       │  ├─ Khởi Tạo:
       │  │  ├─ newCust.totalSpent = 0.0
       │  │  └─ strcpy(newCust.rank, "Bronze")
       │  │
       │  ├─ Lưu vào Array:
       │  │  ├─ danhSachKH[soLuongKH] = newCust
       │  │  └─ soLuongKH++
       │  │
       │  ├─ Thêm vào B-Tree:
       │  │  └─ insertBTree(&btreeRoot, &danhSachKH[soLuongKH])
       │  │
       │  ├─ Lưu File:
       │  │  └─ saveAllCustomersToFile()
       │  │
       │  └─ Gán vào Bill:
       │     └─ currentBill->customer = newCust
       │
       └─ showCustomerRegistration()
          └─ In: Đăng ký thành công + Thông tin
    
    ELSE:
       └─ Tiếp tục (Guest) - currentBill->customer.id = 0
```

---

## VII. PSEUDOCODE ĐẦY ĐỦ

```c
void handleCustomer(Bill *currentBill) {
    char sdt[15];
    printf("\n=== QUAN LY THONG TIN KHACH HANG ===\n");
    
    // 1. Nhập SDT
    getValidPhoneNumber(sdt, sizeof(sdt), ">> Nhap So Dien Thoai: ");
    
    // 2. Tìm kiếm trong B-Tree
    Customer* found = searchBTree(btreeRoot, sdt);
    
    // 3A. Khách hàng cũ
    if (found != NULL) {
        showCustomerInfo(found);
        currentBill->customer = *found;  // Gán vào bill
    } 
    // 3B. Khách hàng mới
    else {
        printf("\n=== KHACH HANG CHUA CO TRONG HE THONG ===\n");
        printf("1. Dang ky moi | 0. Bo qua (Guest)\n");
        int choice = getValidInt("-> Chon: ");
        
        if (choice == 1) {
            // Tạo Customer mới
            Customer newCust;
            newCust.id = soLuongKH + 1;
            strcpy(newCust.phone, sdt);
            
            // Nhập thông tin
            getValidLine(newCust.name, sizeof(newCust.name), ">> Nhap Ten: ");
            getValidLine(newCust.address, sizeof(newCust.address), ">> Nhap Dia chi: ");
            
            // Khởi tạo
            newCust.totalSpent = 0.0;
            strcpy(newCust.rank, "Bronze");
            
            // Lưu vào array
            danhSachKH[soLuongKH] = newCust;
            soLuongKH++;
            
            // Thêm vào B-Tree
            insertBTree(&btreeRoot, &danhSachKH[soLuongKH]);
            
            // Gán vào bill
            currentBill->customer = danhSachKH[soLuongKH - 1];
            
            // Lưu file
            saveAllCustomersToFile();
            
            // Hiển thị xác nhận
            showCustomerRegistration(&danhSachKH[soLuongKH - 1]);
        }
        // Nếu chọn 0: Tiếp tục (Guest) - currentBill->customer sẽ có id = 0
    }
}
```

---

## VIII. VÍ DỤ B-TREE

### Khởi Tạo B-Tree (Trống)
```
btreeRoot = NULL
```

### Sau Khi Thêm 3 Khách (0912, 0987, 0901)
```
                    [0987]
                    /    \
        [0901, 0912]      [0989]
        (isLeaf=1)        (isLeaf=1)
```

### Tìm 0912
```
searchBTree(root, "0912"):
  - i = 0: "0912" < "0987" → không tăng
  - Kiểm tra children[0]
  - children[0] là leaf: [0901, 0912]
  - Tìm thấy 0912 tại index 1
  - Return customers[1]
```

---

## IX. CẤU TRÚC FILE DỮ LIỆU

### `app/database/khachhang.txt`
```
1|0987654321|Nguyen Van A|123 Duong ABC|2500000|Gold
2|0912345678|Tran Thi B|456 Duong XYZ|1200000|Silver
3|0901234567|Le Minh C|789 Duong PQR|500000|Bronze
4|0898765432|Pham Hoang D|321 Duong LMN|12000000|Diamond
```

**Format:** `ID|Phone|Name|Address|TotalSpent|Rank`

---

## X. FILE LIÊN QUAN

| File | Vị trí | Chức năng |
|------|--------|----------|
| customer_controller.c | app/controllers/ | `handleCustomer()` |
| btree_service.h/c | app/services/ | `searchBTree()`, `insertBTree()`, `createBTreeNode()` |
| file_service.c | app/services/ | `saveAllCustomersToFile()` |
| customer_ui.c | app/ui/ | `showCustomerInfo()`, `showCustomerRegistration()` |
| models.h | app/models/ | Định nghĩa struct Customer, BTreeNode |
| validator.h/c | app/utils/ | `getValidPhoneNumber()`, `getValidLine()` |

---

## XI. PHÂN TÍCH B-TREE

### Tính Chất B-Tree Cấp 4:
- **Mỗi node (trừ root):** 1-3 khóa
- **Mỗi node nội bộ:** 2-4 con
- **Root:** 1-3 khóa
- **Tất cả leaf ở cùng độ sâu**
- **Các khóa được sắp xếp** theo thứ tự (SDT)

### Vận Hành:
| Thao tác | Độ phức tạp |
|---------|-----------|
| Tìm kiếm | O(log n) |
| Thêm | O(log n) |
| Xóa | O(log n) |

---

## XII. TÓM TẮT

| Khía cạnh | Chi tiết |
|-----------|---------|
| **Cấu trúc dữ liệu** | B-Tree (cấp 4) + Array |
| **Khóa tìm kiếm** | Số điện thoại (SDT) |
| **Độ phức tạp** | O(log n) - Tìm/Thêm |
| **Hạng VIP** | 4 loại (Bronze/Silver/Gold/Diamond) |
| **Lưu trữ** | B-Tree (RAM) + File (Persistent) |
| **Xác thực** | SDT format, Tên, Địa chỉ |
| **Trạng thái Khách** | Có ID → Registered, ID=0 → Guest |

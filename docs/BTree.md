# 🌳 B-Tree — Giải Thích Thuật Toán & Review Code

---

## 1. B-Tree là gì?

**B-Tree bậc `t`** (order `t`) là cây cân bằng nhiều nhánh, mỗi node có thể chứa **nhiều khóa** và **nhiều con**. Khác với BST (mỗi node chỉ có 1 khóa, 2 con), B-Tree được thiết kế để tối ưu tìm kiếm khi dữ liệu lớn.

> Trong project này dùng **`BTREE_ORDER = 4`** → mỗi node chứa tối đa **3 khóa** và **4 con**.

### Ràng buộc của B-Tree bậc 4 (`ORDER = 4`)

| Thuộc tính | Giá trị |
|---|---|
| Số khóa tối đa / node | `ORDER - 1 = 3` |
| Số khóa tối thiểu / node (trừ root) | `⌈ORDER/2⌉ - 1 = 1` |
| Số con tối đa / node | `ORDER = 4` |
| Số con tối thiểu / node (trừ root) | `⌈ORDER/2⌉ = 2` |

---

## 2. Cấu trúc Node trong Project

```c
#define BTREE_ORDER 4

typedef struct BTreeNode {
    Customer* customers[BTREE_ORDER - 1]; // Mảng 3 con trỏ khóa
    struct BTreeNode* children[BTREE_ORDER]; // Mảng 4 con trỏ con
    int numKeys;   // Số khóa hiện có
    int isLeaf;    // 1 = lá, 0 = node trong
} BTreeNode;
```

**Minh họa một node đầy (3 khóa):**

```
         customers[0]   customers[1]   customers[2]
        ┌────────────┬────────────┬────────────┐
        │  "0901"    │  "0912"    │  "0933"    │
        └─────┬──────┴──────┬─────┴──────┬─────┘
              │             │            │
          child[0]      child[1]     child[2]    child[3]
         (< "0901")  ("0901"~"0912") ("0912"~"0933") (> "0933")
```

> **Quy tắc vàng:** Các khóa trong `customers[]` luôn được sắp xếp tăng dần.
> Mọi khóa trong `child[i]` đều **nằm giữa** `customers[i-1]` và `customers[i]`.

---

## 3. Tổng Quan 3 Hàm Insert Phối Hợp Nhau

Khi chèn một khách hàng mới, 3 hàm này được gọi theo thứ tự:

```
insertBTree()          ← Hàm ngoài cùng, gọi đầu tiên
    │
    ├─ Nếu root đầy → splitChild() trước
    │
    └─ insertNonFull() ← Hàm thực sự đi xuống cây để chèn
            │
            └─ Nếu gặp node con đầy → splitChild() trước khi xuống
```

---

## 4. Hàm `insertBTree()` — Cổng Vào

```c
void insertBTree(BTreeNode** root, Customer* cust) {
    if (*root == NULL) {
        // ① Cây rỗng: tạo node gốc luôn
        *root = createBTreeNode(1);       // isLeaf = 1
        (*root)->customers[0] = cust;
        (*root)->numKeys = 1;
    } else {
        if ((*root)->numKeys == BTREE_ORDER - 1) {
            // ② Root đầy (numKeys = 3): phải split trước
            BTreeNode* s = createBTreeNode(0); // Tạo root mới, isLeaf = 0
            s->children[0] = *root;            // Root cũ trở thành con đầu tiên
            splitChild(s, 0, *root);           // Split root cũ
            int i = 0;
            if (strcmp(cust->phone, s->customers[0]->phone) > 0) i++;
            insertNonFull(s->children[i], cust);
            *root = s; // Root mới là s
        } else {
            // ③ Root chưa đầy: xuống thẳng
            insertNonFull(*root, cust);
        }
    }
}
```

### Khi nào dùng `insertBTree`?
> **Luôn luôn** — Đây là hàm duy nhất bạn gọi từ bên ngoài để chèn.
> Nó lo phần "kiểm tra root có đầy không" trước khi làm gì khác.

---

## 5. Hàm `splitChild()` — Chia Đôi Node Đầy

### Khi nào gọi `splitChild`?
> **Khi một node con đã có đủ 3 khóa (đầy)** mà ta cần chèn thêm vào vùng đó.
> Thay vì nhét thêm vào node đầy (vi phạm quy tắc), ta **tách nó ra thành 2 node** và **đẩy khóa giữa lên node cha**.

### Code:
```c
void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    // child là node đang đầy [K0, K1, K2], cần tách

    // Bước 1: Tạo node mới z để chứa phần phải
    BTreeNode* z = createBTreeNode(child->isLeaf);
    z->numKeys = 1;
    z->customers[0] = child->customers[2]; // K2 sang z

    // Bước 2: Nếu child không phải lá, con của phần phải cũng di chuyển sang z
    if (!child->isLeaf) {
        z->children[0] = child->children[2];
        z->children[1] = child->children[3];
    }

    // Bước 3: Cắt bớt child, chỉ còn lại K0
    child->numKeys = 1;
    // (K1 sẽ lên parent, K2 đã sang z — child chỉ còn K0)

    // Bước 4: Gắn z vào parent tại vị trí i+1
    for (int j = parent->numKeys; j >= i + 1; j--)
        parent->children[j+1] = parent->children[j]; // Dịch phải
    parent->children[i+1] = z;

    // Bước 5: Đẩy K1 (khóa giữa) lên parent
    for (int j = parent->numKeys - 1; j >= i; j--)
        parent->customers[j+1] = parent->customers[j]; // Dịch phải
    parent->customers[i] = child->customers[1]; // K1 lên parent
    parent->numKeys++;
}
```

### Ví dụ minh họa từng bước:

**Tình huống:** Parent còn chỗ, child (ở vị trí `children[1]`) đã đầy `["0910", "0920", "0930"]`. Ta cần chèn thêm `"0925"` vào vùng đó.

```
TRƯỚC KHI splitChild(parent, 1, child):

         parent
    ┌────────────────┐
    │    "0900"      │  (numKeys = 1)
    └──────┬─────────┘
           │
       children[0]    children[1]
       [< "0900"]     [đầy!]
                   ┌────────────────────────┐
                   │ "0910" │ "0920" │ "0930"│
                   └────────────────────────┘
                    ^K0      ^K1      ^K2
```

**Bước 1–2:** Tạo node `z`, đưa K2 = `"0930"` sang `z`:
```
z = ["0930"]
child = ["0910", "0920", "0930"]  ← chưa cắt
```

**Bước 3:** Cắt `child` còn 1 khóa (K0):
```
child = ["0910"]   (K1 = "0920" sẽ lên parent)
z     = ["0930"]
```

**Bước 4–5:** Đưa K1 = `"0920"` lên parent, gắn `z` vào `children[2]`:
```
SAU KHI splitChild:

              parent
    ┌──────────────────────┐
    │  "0900"  │  "0920"  │  (numKeys = 2)
    └────┬─────────┬───────┘
         │         │
     child[0]   child[1]    child[2]
    [< "0900"] ["0910"]    ["0930"]
```

Bây giờ ta có thể chèn `"0925"` vào `child[1]` (khoảng `"0920"`~`"0930"`) một cách bình thường!

---

## 6. Hàm `insertNonFull()` — Chèn Vào Node Chưa Đầy

### Khi nào gọi `insertNonFull`?
> Sau khi `insertBTree` đảm bảo root chưa đầy, hàm này đi **từ trên xuống** để tìm đúng **node lá** để chèn.
> Trên đường đi, nếu gặp **node con đầy**, nó **split con đó trước** rồi mới xuống.

### Code:
```c
void insertNonFull(BTreeNode* node, Customer* cust) {
    int i = node->numKeys - 1; // Bắt đầu từ khóa cuối cùng

    if (node->isLeaf) {
        // ── TRƯỜNG HỢP 1: Đang ở node lá ──
        // Dịch các khóa lớn hơn sang phải để chừa chỗ trống
        while (i >= 0 && strcmp(cust->phone, node->customers[i]->phone) < 0) {
            node->customers[i+1] = node->customers[i];
            i--;
        }
        // Chèn khóa mới vào đúng vị trí
        node->customers[i+1] = cust;
        node->numKeys++;

    } else {
        // ── TRƯỜNG HỢP 2: Đang ở node trong ──
        // Tìm vị trí con (i+1) cần đi xuống
        while (i >= 0 && strcmp(cust->phone, node->customers[i]->phone) < 0) i--;

        // Nếu con đó đầy → split trước
        if (node->children[i+1]->numKeys == BTREE_ORDER - 1) {
            splitChild(node, i + 1, node->children[i+1]);
            // Sau split, khóa giữa của con đã lên node hiện tại
            // Xác định lại: xuống trái hay phải của khóa vừa lên?
            if (strcmp(cust->phone, node->customers[i+1]->phone) > 0) i++;
        }
        // Đệ quy xuống con
        insertNonFull(node->children[i+1], cust);
    }
}
```

### Trường hợp 1: Node lá — Chèn trực tiếp

**Chèn `"0908"` vào lá `["0901", "0933"]`** (còn 1 chỗ trống):

```
Bắt đầu: i = 1 (numKeys-1 = 1)

Vòng while:
  i=1: "0908" < "0933" → dịch phải → ["0901", _, "0933"], i=0
  i=0: "0908" > "0901" → dừng

Chèn vào i+1 = 1:
  ["0901", "0908", "0933"] ✅
```

### Trường hợp 2: Node trong — Tìm con để xuống

**Chèn `"0925"` vào cây dưới:**

```
         node hiện tại
    ┌──────────────────────┐
    │  "0900"  │  "0950"  │
    └────┬─────────┬───────┘
         │         │
     child[0]  child[1]          child[2]
    [< "0900"] ["0910","0930"]   [> "0950"]
                  ↑ đầy!
```

```
Bắt đầu: i = 1 (numKeys-1)

Vòng while:
  i=1: "0925" < "0950" → i=0
  i=0: "0925" > "0900" → dừng
→ Cần xuống children[i+1] = children[1]

Kiểm tra: children[1]->numKeys = 2 ≠ 3 → chưa đầy → xuống thẳng
→ Đệ quy: insertNonFull(children[1], "0925")
```

**Nhưng nếu `children[1]` đầy `["0910", "0920", "0930"]`?**

```
Kiểm tra: children[1]->numKeys == 3 → ĐẦY!
→ Gọi: splitChild(node, 1, children[1])

Sau split: "0920" lên node, children[1]=["0910"], children[2]=["0930"]
Node trở thành:
    ┌──────────────────────────────┐
    │  "0900"  │  "0920"  │  "0950"│
    └───┬──────────┬──────────┬────┘
        │          │          │         │
    child[0]   child[1]   child[2]   child[3]
               ["0910"]   ["0930"]

Xác định lại: "0925" > "0920" → i++ → i=1
→ Xuống children[2] = ["0930"]
→ Chèn "0925" vào ["0930"]:  ["0925", "0930"] ✅
```

---

## 7. Sơ Đồ Gọi Hàm Khi Insert

```
insertBTree("0925")
│
├─ Root chưa đầy?
│   ├─ Có → insertNonFull(root, "0925")
│   │           │
│   │           ├─ Node lá? → Chèn trực tiếp (dịch phải, nhét vào)
│   │           │
│   │           └─ Node trong?
│   │               ├─ Tìm children[i+1] để xuống
│   │               ├─ children[i+1] đầy? → splitChild() trước
│   │               └─ Đệ quy insertNonFull(children[i+1], "0925")
│   │
│   └─ Không (root đầy) → splitChild(newRoot, 0, root)
│                          rồi insertNonFull(...)
```

---

## 8. Ví Dụ Toàn Bộ: Chèn 7 Khách Hàng

Chèn lần lượt: `"A"`, `"B"`, `"C"`, `"D"`, `"E"`, `"F"`, `"G"`

**Bước 1:** Chèn `"A"` → cây rỗng → tạo root lá
```
["A"]
```

**Bước 2:** Chèn `"B"` → root lá chưa đầy → insertNonFull → chèn thẳng
```
["A", "B"]
```

**Bước 3:** Chèn `"C"` → root lá chưa đầy → chèn thẳng
```
["A", "B", "C"]   ← ROOT ĐẦY (numKeys = 3)
```

**Bước 4:** Chèn `"D"` → **`insertBTree` thấy root đầy!**
```
① Tạo root mới s (isLeaf=0)
② s->children[0] = root cũ ["A","B","C"]
③ splitChild(s, 0, rootCũ):
     - z = ["C"]
     - rootCũ = ["A"]
     - "B" lên s
④ s = ["B"], children[0]=["A"], children[1]=["C"]

"D" > "B" → i=1 → insertNonFull(children[1]=["C"], "D")
→ Chèn D vào lá ["C"]: ["C", "D"] ✅

KẾT QUẢ:
         ["B"]
        /     \
     ["A"]   ["C","D"]
```

**Bước 5:** Chèn `"E"` → root `["B"]` chưa đầy → insertNonFull
```
i=0: "E" > "B" → i=0 (dừng vì hết)
xuống children[1] = ["C","D"] chưa đầy → chèn "E"
→ ["C","D","E"] ← ĐẦY

         ["B"]
        /     \
     ["A"]  ["C","D","E"]
```

**Bước 6:** Chèn `"F"` → insertNonFull(root=["B"])
```
xuống children[1] = ["C","D","E"] → ĐẦY!
→ splitChild(root, 1, ["C","D","E"]):
    z = ["E"], child = ["C"], "D" lên root

root = ["B","D"], children[0]=["A"], children[1]=["C"], children[2]=["E"]

"F" > "D" → i=2 → xuống children[2]=["E"]
→ Chèn "F": ["E","F"] ✅

         ["B","D"]
        /    |    \
     ["A"] ["C"]  ["E","F"]
```

**Bước 7:** Chèn `"G"`:
```
xuống children[2]=["E","F"] chưa đầy → chèn "G"

         ["B","D"]
        /    |    \
     ["A"] ["C"]  ["E","F","G"]   ← ĐẦY nhưng chưa cần split
```

---

## 9. Đánh Giá Tổng Thể Code

### ✅ Điểm tốt

| # | Điểm mạnh |
|---|---|
| 1 | Logic tìm kiếm `searchBTree` chuẩn xác, dùng `strcmp` đúng cách |
| 2 | Xử lý đúng trường hợp root đầy trong `insertBTree` |
| 3 | Phân tách rõ ràng: `insertBTree` → `insertNonFull` → `splitChild` |
| 4 | Dùng con trỏ `Customer*` thay vì copy struct → tiết kiệm bộ nhớ |

### ⚠️ Điểm cần lưu ý

| # | Vấn đề | Mức độ | Gợi ý |
|---|---|---|---|
| 1 | `splitChild` hardcode index `[1]`, `[2]` | Trung bình | Ổn vì `ORDER` cố định = 4 |
| 2 | `createBTreeNode` không init `customers[]` = NULL | Thấp | Nên thêm init để tránh garbage |
| 3 | Không có hàm `freeBTree()` | Thấp | Nên có để tránh memory leak |
| 4 | Không kiểm tra `malloc` trả về NULL | Thấp | Nên add khi build production |

---

## 10. So Sánh B-Tree với BST

| Tiêu chí | BST | B-Tree (ORDER=4) |
|---|---|---|
| Chiều cao cây | `O(n)` xấu nhất | `O(log n)` luôn đảm bảo |
| Tìm kiếm | `O(n)` xấu nhất | `O(log n)` |
| Cân bằng | Không tự cân bằng | Luôn cân bằng |

---

*📝 Tài liệu phân tích code BTree — PBL1 Hệ Thống Quản Lý Quán Cơm Tấm*

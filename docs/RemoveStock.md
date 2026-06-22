# Hướng dẫn Xóa Tồn Kho (Stock) Trong Dự Án

## Tổng quan
Tồn kho (stock) được sử dụng ở nhiều nơi trong dự án. Để xóa hoàn toàn, bạn cần sửa 4 file chính:

---

## 1. Sửa file `app/models/models.h`

**Vị trí:** Dòng ~18 trong struct `MenuItem`

**Xóa dòng:**
```c
int stock;
```

**Kết quả:** Struct MenuItem sẽ có dạng:
```c
typedef struct {
    int id;
    char name[50];
    double price;
    // int stock;  ← XÓA DÒNG NÀY
    int hasOptions;
} MenuItem;
```

---

## 2. Sửa file `app/ui/menu_ui.c`

### Bước 2.1: Sửa hàm `printItemCell`

**Vị trí:** Dòng ~17

**Thay đổi từ:**
```c
static void printItemCell(int id, const char *name, double price, int stock, int nameWidth) {
    printf(" %2d.", id);
    printf(WHITE "%-*.*s" RESET, nameWidth, nameWidth, name);
    printf(" " GREEN "%9.0lf" RESET, price);
    printf(" (K:%-3d)", stock);  // ← XÓA PHẦN NÀY
}
```

**Thành:**
```c
static void printItemCell(int id, const char *name, double price, int nameWidth) {
    printf(" %2d.", id);
    printf(WHITE "%-*.*s" RESET, nameWidth, nameWidth, name);
    printf(" " GREEN "%9.0lf" RESET, price);
}
```

### Bước 2.2: Cập nhật tất cả lệnh gọi `printItemCell`

**Tìm và sửa** tất cả dòng gọi hàm `printItemCell` bằng cách **xóa tham số `menu[i].stock`**

**Ví dụ:**
- **Từ:** `printItemCell(menu[i].id, menu[i].name, menu[i].price, menu[i].stock, 20);`
- **Thành:** `printItemCell(menu[i].id, menu[i].name, menu[i].price, 20);`

**Các dòng cần sửa:**
- Dòng ~47, 55, 58, 74, 80, 84 (hoặc xung quanh)

---

## 3. Sửa file `app/controllers/order_controller.c`

### Bước 3.1: Xóa kiểm tra stock khi thêm món

**Tìm dòng ~92:**
```c
if (diff > 0 && diff > menuItem->stock) {
    printf("  !! Loi: Kho khong du hang (Con: %d).\n", menuItem->stock);
    return;
}
```

**Xóa hoàn toàn 3 dòng trên** hoặc thay thành:
```c
if (diff <= 0) {
    printf("  !! Loi: So luong khong hop le.\n");
    return;
}
```

### Bước 3.2: Xóa cập nhật stock khi thêm món

**Tìm dòng ~98:**
```c
if (menuItem) menuItem->stock -= diff;  // ← XÓA DÒNG NÀY
```

### Bước 3.3: Xóa cập nhật stock khi hủy món

**Tìm dòng ~41:**
```c
if (menuItem) menuItem->stock += node->item.quantity;  // ← XÓA DÒNG NÀY
```

### Bước 3.4: Xóa kiểm tra stock khi đặt hàng

**Tìm dòng ~136:**
```c
if (slNhap <= 0 || slNhap > menuItem->stock || slNhap > 5) {
    printf("  !! Loi: Kho ko du hoac SL ko hop le (Con: %d)\n", menuItem->stock);
    return;
}
```

**Thay thành:**
```c
if (slNhap <= 0 || slNhap > 5) {
    printf("  !! Loi: So luong khong hop le (Toi da 5)\n");
    return;
}
```

### Bước 3.5: Xóa cập nhật stock khi đặt hàng

**Tìm dòng ~185:**
```c
menuItem->stock -= slNhap;  // ← XÓA DÒNG NÀY
```

---

## 4. Sửa file `app/services/file_service.c`

**Vị trí:** Dòng ~45-51 trong hàm load menu

**Tìm và xóa tất cả dòng gán stock:**
```c
menu[i].stock = 500 + rand() % 501;  // ← XÓA
if (gioHienTai >= 6 && gioHienTai < 11) menu[i].stock = 400 + rand() % 101;  // ← XÓA
else if (gioHienTai >= 11 && gioHienTai < 14) menu[i].stock = 300 + rand() % 101;  // ← XÓA
else if (gioHienTai >= 14 && gioHienTai < 17) menu[i].stock = 270 + rand() % 31;  // ← XÓA
else if (gioHienTai >= 17 && gioHienTai < 21) menu[i].stock = 50 + rand() % 221;  // ← XÓA
else menu[i].stock = rand() % 51;  // ← XÓA
```

---

## Bước cuối cùng: Kiểm tra Compile

**Chạy lệnh compile:**
```bash
gcc -o run main.c app/**/*.c -lm
```

Nếu có lỗi, tìm dòng lỗi và sửa lại theo hướng dẫn trên.

---

## Tóm tắt các file cần sửa
1. ✅ `app/models/models.h` - Xóa `int stock;` từ MenuItem
2. ✅ `app/ui/menu_ui.c` - Sửa hàm in và xóa tham số stock
3. ✅ `app/controllers/order_controller.c` - Xóa tất cả xử lý stock
4. ✅ `app/services/file_service.c` - Xóa gán giá trị stock

---

**Lưu ý:** Nếu có chỗ nào khác sử dụng stock, compiler sẽ báo lỗi. Khi đó hãy tìm dòng lỗi và xóa/sửa tương tự.

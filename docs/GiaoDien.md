# Hướng dẫn tạo giao diện khung hộp PBL1

## Ý tưởng chung
Vẽ khung hộp bằng các ký tự đặc biệt (border) và text, sử dụng `printf()` để in từng dòng theo thứ tự:
1. Viền trên
2. Tiêu đề chính
3. Tiêu đề phụ
4. Thông tin
5. Viền dưới

---

## Các ký tự dùng để vẽ khung

Lựa chọn giữa 2 cách:

### Cách 1: Dùng ký tự Unicode (đẹp hơn)
```
╔═══════════════════════════════════════════╗
║                 Nội dung                   ║
╚═══════════════════════════════════════════╝
```
- Top left: `╔`
- Top/bottom: `═`
- Top right: `╗`
- Left/right: `║`
- Bottom left: `╚`
- Bottom right: `╝`

### Cách 2: Dùng ký tự ASCII (tương thích hơn)
```
+=====================================+
|             Nội dung                |
+=====================================+
```
- Corners: `+`
- Horizontal: `=` hoặc `-`
- Vertical: `|`

---

## Ví dụ code

```c
void printMainMenu() {
    int width = 80;  // Chiều rộng của khung
    
    // Viền trên
    printf("╔");
    for (int i = 0; i < width - 2; i++) printf("═");
    printf("╗\n");
    
    // Tiêu đề chính (căn giữa)
    printf("║");
    printf("%-*s", width - 2, "");  // Khoảng trắng căn lề
    printf("║\n");
    
    // In tiêu đề với căn giữa
    char title[] = "PBL1: ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN";
    int padding = (width - 2 - strlen(title)) / 2;
    printf("║");
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s", title);
    for (int i = 0; i < (width - 2 - strlen(title)) - padding; i++) printf(" ");
    printf("║\n");
    
    // Viền giữa
    printf("║");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("║\n");
    
    // Nội dung khác (giáo viên, sinh viên, ...)
    // ...
    
    // Viền dưới
    printf("╚");
    for (int i = 0; i < width - 2; i++) printf("═");
    printf("╝\n");
}
```

---

## Cách tính toán căn lề giữa

Công thức căn giữa một string:
```c
int totalWidth = 80;        // Chiều rộng khung (trừ viền)
int contentWidth = totalWidth - 2;  // Trừ 2 ký tự viền trái/phải
int stringLength = strlen(text);
int padding = (contentWidth - stringLength) / 2;

printf("║");
for (int i = 0; i < padding; i++) printf(" ");
printf("%s", text);
for (int i = 0; i < (contentWidth - stringLength) - padding; i++) printf(" ");
printf("║\n");
```

---

## Chi tiết cấu trúc giao diện (theo hình)

```
1. Viền trên: ╔════════════════════════════════════════════╗

2. Tiêu đề chính: ║   PBL1: ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN   ║

3. Phụ đề: ║   TÊN ĐỀ TÀI: ĐẬT MÓN THÔNG MINH, ...   ║
          ║   SỬ DỤNG HỎI QUY TUYẾN TÍNH, ...        ║

4. Viền ngăn: ║────────────────────────────────────────────║

5. Thông tin giáo viên:
   ║ Giáo viên hướng dẫn: PGS. TS. PHẠM CÔNG THẮNG  ║

6. Thông tin sinh viên:
   ║ Sinh viên thực hiện:                           ║
   ║ LÊ QUYNH TRĂNG    2HT_Nhất 1    MSSV: 1022M330 ║
   ║ THIỆU BẢO THƯ     2HT_Nhất 1    MSSV: 1022M334 ║

7. Viền dưới: ╚════════════════════════════════════════════╝
```

---

## Cách thực hiện từng bước

### Bước 1: Tạo hàm vẽ viền trên
```c
void printTopBorder(int width) {
    printf("╔");
    for (int i = 0; i < width - 2; i++) printf("═");
    printf("╗\n");
}
```

### Bước 2: Tạo hàm in dòng text bình thường (trái)
```c
void printLine(char* text, int width) {
    printf("║ %-*s ║\n", width - 4, text);
}
```

### Bước 3: Tạo hàm in dòng text căn giữa
```c
void printCenterLine(char* text, int width) {
    int textLen = strlen(text);
    int totalSpace = width - 4;  // Trừ 2 ký tự viền
    int leftPad = (totalSpace - textLen) / 2;
    int rightPad = totalSpace - textLen - leftPad;
    
    printf("║");
    for (int i = 0; i < leftPad; i++) printf(" ");
    printf("%s", text);
    for (int i = 0; i < rightPad; i++) printf(" ");
    printf("║\n");
}
```

### Bước 4: Tạo hàm viền dưới
```c
void printBottomBorder(int width) {
    printf("╚");
    for (int i = 0; i < width - 2; i++) printf("═");
    printf("╝\n");
}
```

### Bước 5: Tạo hàm viền giữa (chia ngăn)
```c
void printMiddleBorder(int width) {
    printf("║");
    for (int i = 0; i < width - 2; i++) printf("─");
    printf("║\n");
}
```

### Bước 6: Tạo hàm chính để in toàn bộ giao diện
```c
void printWelcomeScreen() {
    int width = 80;
    
    printTopBorder(width);
    printCenterLine("PBL1: ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN", width);
    printCenterLine("TÊN ĐỀ TÀI: ĐẬT MÓN THÔNG MINH, TỐI ƯU THANH TOÁN,", width);
    printCenterLine("SỬ DỤNG HỎI QUY TUYẾN TÍNH, THUẬT TOÁN QUY HOẠCH ĐỘNG", width);
    printMiddleBorder(width);
    printLine("Giáo viên hướng dẫn:", width);
    printCenterLine("PGS. TS. PHẠM CÔNG THẮNG", width);
    printMiddleBorder(width);
    printLine("Sinh viên thực hiện:", width);
    printLine("LÊ QUYNH TRĂNG        2HT_Nhất 1        MSSV: 1022M3306", width);
    printLine("THIỆU BẢO THƯ         2HT_Nhất 1        MSSV: 1022M3343", width);
    printBottomBorder(width);
}
```

---

## File cần tạo/sửa
1. Tạo file mới: `app/ui/welcome_ui.h` - Khai báo các hàm
2. Tạo file mới: `app/ui/welcome_ui.c` - Implement các hàm trên
3. Sửa: `main.c` - Gọi `printWelcomeScreen()` ở đầu chương trình

---

## Lưu ý
- Chú ý encoding UTF-8 để các ký tự đặc biệt hiển thị đúng
- **Windows Console UTF-8:** Để dùng ký tự Unicode trên Windows, cần enable UTF-8 bằng cách:
  - Thêm vào `run.bat` dòng: `chcp 65001` trước khi chạy `.exe`
  - Hoặc thêm vào `main.c` trước `main()`:
    ```c
    #include <windows.h>
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    ```
- Kiểm tra terminal hỗ trợ UTF-8 (Console Windows cần bật UTF-8 như trên)
- Nếu vẫn không hiển thị đúng, dùng cách ASCII (`+`, `=`, `|`) thay thế
- Tính toán chiều rộng cẩn thận (include cả ký tự space, border)

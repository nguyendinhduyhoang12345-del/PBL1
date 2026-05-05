# Kế hoạch kiểm tra và sửa PBL1

## 1. Tình trạng hiện tại đã kiểm tra
- `app/utils/validator.c` đã có:
  - `getValidInt(const char* prompt)`
  - `getValidDouble(const char* prompt)`
  - `isDigits(const char* s)`
- `app/utils/validator.h` chỉ khai báo 3 hàm trên.
- `app/controllers/customer_controller.c` đang dùng `scanf("%14s", sdt)` và trim thủ công.
- `app/controllers/history_controller.c` đang dùng `scanf("%14s", phone)` nhưng chưa trim và chưa kiểm tra `isDigits()`.
- `app/controllers/checkout_controller.c` đã dùng `getValidDouble()` cho tiền và `getValidInt()` cho xác nhận, đúng hướng.
- `app/controllers/order_controller.c` đã dùng `getValidInt()` cho mã món, số lượng, và tùy chọn.

## 2. Mục tiêu cần đạt
1. Tạo helper chung trong `validator.c` để gọi dễ dàng.
2. Kiểm tra đầy đủ:
   - nhập số điện thoại đúng 10 chữ số
   - trim khoảng trắng đầu/cuối
   - không dùng `scanf("%s", ...)` trực tiếp cho tất cả input quan trọng
3. Áp dụng helper vào tất cả phần cần kiểm tra:
   - quản lý khách hàng
   - tìm lịch sử theo số điện thoại
   - nhập tiền thanh toán
   - nhập menu / số lượng / chọn option
4. Ghi chi tiết từng bước vào `plan.md`.

## 3. Sửa `validator.h`
1. Mở `app/utils/validator.h`.
2. Thêm bốn hàm sau:

```
void clearBuffer();
int getValidInt(const char* prompt);
double getValidDouble(const char* prompt);
int isDigits(const char* s);
void trimString(char* s);
void getValidPhoneNumber(char* phone, int maxLen, const char* prompt);
void getValidLine(char* buffer, int maxLen, const char* prompt);
```

3. `getValidLine()` là helper để đọc một dòng chuỗi bằng `fgets()` và trim luôn.

## 4. Sửa `validator.c`
1. Mở `app/utils/validator.c`.
2. Thêm include `string.h` nếu chưa có.
3. Giữ lại `clearBuffer()`, `getValidInt()`, `getValidDouble()`, `isDigits()`.
4. Thêm hàm:

```c
void trimString(char* s) {
    int len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || s[len-1] == ' ' || s[len-1] == '\t')) {
        s[--len] = '\0';
    }
    int start = 0;
    while (s[start] == ' ' || s[start] == '\t') {
        start++;
    }
    if (start > 0) {
        memmove(s, s + start, strlen(s + start) + 1);
    }
}

void getValidPhoneNumber(char* phone, int maxLen, const char* prompt) {
    while (1) {
        printf("%s", prompt);
        scanf("%s", phone);
        clearBuffer();
        trimString(phone);
        if (strlen(phone) != 10) {
            printf("  !! Loi: SDT phai co 10 chu so.\n");
            continue;
        }
        if (!isDigits(phone)) {
            printf("  !! Loi: SDT chi duoc chua chu so.\n");
            continue;
        }
        break;
    }
}

void getValidLine(char* buffer, int maxLen, const char* prompt) {
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    trimString(buffer);
}
```

5. Nếu muốn, đổi `scanf("%s", phone)` thành `scanf("%14s", phone)` trong `getValidPhoneNumber()` để an toàn hơn.

## 5. Áp dụng helper vào `customer_controller.c`
### 5.1. Thay nhập số điện thoại
1. Xóa toàn bộ phần:
   - `printf("..." ); scanf("%14s", sdt); clearBuffer();`
   - đoạn trim thủ công bằng `strlen`, `memmove`, `while`.
2. Thay bằng:

```c
char sdt[15];
getValidPhoneNumber(sdt, sizeof(sdt), ">> Nhap So Dien Thoai khach hang: ");
```

3. Khi nhập lại nếu sai, `getValidPhoneNumber()` sẽ tự lặp.

### 5.2. Dùng `trimString()` cho tên và địa chỉ
1. Thay:

```c
fgets(newCust.name, 50, stdin);
newCust.name[strcspn(newCust.name, "\n")] = 0;
```

bằng:

```c
getValidLine(newCust.name, sizeof(newCust.name), " >> Nhap Ten: ");
```

2. Thay tương tự cho `newCust.address`:

```c
getValidLine(newCust.address, sizeof(newCust.address), " >> Nhap Dia chi: ");
```
```

3. Điều này giúp name/address luôn được trim đầu/cuối và loại bỏ newline.

## 6. Áp dụng helper vào `history_controller.c`
1. Thay phần nhập phone bằng:

```c
char phone[15];
getValidPhoneNumber(phone, sizeof(phone), ">> Nhap so dien thoai khach hang: ");
```

2. Không cần trim thêm.
3. Đây là nơi cần kiểm tra để tìm lịch sử khách hàng.

## 7. Các chỗ khác cần validate
### 7.1. `main.c`
- Đã dùng `getValidInt()` cho lựa chọn menu đúng rồi.

### 7.2. `order_controller.c`
- Giữ `getValidInt()` cho mã món, số lượng và chọn option.
- Kiểm tra thêm:
  - `idNhap` phải nằm trong `1..TONG_SO_MON`
  - `slNhap` phải > 0 và <= `menu[idx].stock`
  - với món chính, tổng số lượng không vượt 5

### 7.3. `checkout_controller.c`
- Dùng `customerMoney = getValidDouble("-> Nhap so tien khach dua : ");`
- Dùng `xacNhan = getValidInt("Xac nhan thanh toan (1: Dong y / 0: Huy bo): ");`
- Nếu muốn, thêm vòng lặp bắt `xacNhan` chỉ nhận `0` hoặc `1`.

### 7.4. `history_controller.c`
- Sử dụng `getValidPhoneNumber()` cho nhập số điện thoại.
- Có thể nâng cấp `getCustomerIdByPhone()` để trim trước khi tìm.

### 7.5. `customer_controller.c` phần đăng ký
- Nếu nhập tên/địa chỉ trống sau trim, có thể yêu cầu nhập lại.
- Điều này giúp dữ liệu khách hàng sạch hơn.

## 8. Những hàm helper nên thêm vào `validator.c`
- `trimString(char* s)`
- `getValidPhoneNumber(char* phone, int maxLen, const char* prompt)`
- `getValidLine(char* buffer, int maxLen, const char* prompt)`

## 9. Check list cho `plan.md`
1. `validator.h` có đủ các prototype: `trimString`, `getValidPhoneNumber`, `getValidLine`.
2. `validator.c` có định nghĩa các helper đó.
3. `customer_controller.c` không còn trim manual cho `sdt`.
4. `customer_controller.c` dùng `getValidLine()` cho tên và địa chỉ.
5. `history_controller.c` dùng `getValidPhoneNumber()`.
6. `checkout_controller.c` dùng `getValidDouble()` và `getValidInt()`.
7. `order_controller.c` và `main.c` dùng `getValidInt()`.
8. Không còn `scanf("%s", ...)` chưa giới hạn ở các input quan trọng.
9. Các file dịch vụ kiểm tra định dạng file khi đọc dữ liệu.

## 10. Ghi chi tiết từng bước vào `plan.md`
- Với mỗi mục trên, ghi rõ:
  - file cần sửa
  - hàm cần sửa
  - dòng hoặc đoạn thay đổi cụ thể
  - nếu cần, nội dung thay thế code

## 11. Hướng dẫn test sau sửa
- chạy biên dịch với `gcc -Wall -Wextra -Wshadow`.
- mở chương trình và kiểm tra:
  1. nhập menu sai chữ/người
  2. nhập số điện thoại không đủ 10 chữ số
  3. nhập số điện thoại có chữ
  4. nhập tiền thanh toán không phải số
  5. tìm lịch sử theo số điện thoại
  6. thêm khách hàng mới và lưu file

# Kế hoạch Thay đổi Format Lưu trữ Lịch sử Bill Chi tiết

## Tổng quan
Hiện tại, file lịch sử chỉ lưu thông tin tóm tắt của bill (id, customerId, dateTime, finalPrice, discount). 
Cần thay đổi để lưu **toàn bộ chi tiết items** (tên món, id, số lượng, ghi chú, option, giá...) 
để khi xem chi tiết bill, người dùng có thể thấy "đã mua món gì" một cách đầy đủ.

---

## 1. Hiểu tình hình hiện tại

### 1.1 Format file hiện tại
**File**: `app/database/history_khach_X.txt` và `app/database/history.txt`

```
billId|customerId|dateTime|finalPrice|discount
100001|1|01/05/2026 10:30:00|850500.000|0.000
100003|1|02/05/2026 14:20:00|1250000.000|62500.000
```

### 1.2 Cấu trúc dữ liệu Bill hiện tại
Từ `app/models/models.h`:
```c
typedef struct {
    int id;           // ID hóa đơn
    Customer customer;// Thông tin khách hàng
    OrderItem items[MAX_GIO_HANG]; // Mảng các món hàng (CHỈ CÓ TRONG BỘ NHỚ)
    int itemCount;    // Số lượng items
    double total;     // Tổng trước giảm giá
    double discount;  // Tiền giảm giá
    double finalPrice;// Tổng cuối cùng
    char dateTime[50];// Thời gian
} Bill;

typedef struct {
    int id;        // ID món
    char name[50]; // Tên món
    double price;  // Giá
    int quantity;  // Số lượng
    int option;    // Tùy chọn (0, 1, 2...)
    char note[100];// Ghi chú
    double totalPrice; // Thành tiền
} OrderItem;
```

### 1.3 Vấn đề hiện tại
- **Hàm `saveBillToHistoryFile()`** chỉ lưu 5 thông tin: billId, customerId, dateTime, finalPrice, discount
- **Hàm `loadHistoryFromFile()`** chỉ đọc 5 thông tin trên, `itemCount = 0`, mảng items rỗng
- Khi gọi `printBillDetail()`, hiển thị items rỗng vì `billNode->bill.itemCount = 0`

---

## 2. Ý tưởng giải pháp

### 2.1 Chiến lược lưu trữ
Sử dụng **cấu trúc multi-line**:
- **Dòng 1**: Thông tin chung của bill
- **Dòng 2 - N**: Chi tiết từng item trong bill
- **Dòng N+1**: Dấu kết thúc (marker)

### 2.2 Format mới chi tiết

#### Format dòng thông tin bill:
```
BILL|billId|customerId|nameKhach|phoneKhach|dateTime|total|discount|finalPrice|itemCount
```

Ví dụ:
```
BILL|100001|1|Nguyen Van A|0123456789|01/05/2026 10:30:00|850500.000|0.000|850500.000|2
```

#### Format dòng item:
```
ITEM|itemId|itemName|unitPrice|quantity|option|note|totalPrice
```

Ví dụ:
```
ITEM|1|Com tam sườn cay|45000.000|1|2|Them hanh|45000.000
ITEM|2|Nuoc chanh|5500.000|3|0|Khong duong|16500.000
```

#### Dấu kết thúc:
```
END
```

#### Ví dụ bill hoàn chỉnh trong file:
```
BILL|100001|1|Nguyen Van A|0123456789|01/05/2026 10:30:00|850500.000|0.000|850500.000|2
ITEM|1|Com tam sườn cay|45000.000|1|2|Them hanh|45000.000
ITEM|2|Nuoc chanh|5500.000|3|0|Khong duong|16500.000
END
BILL|100003|1|Nguyen Van A|0123456789|02/05/2026 14:20:00|1250000.000|62500.000|1187500.000|3
ITEM|1|Com tam sườn cot|42000.000|2|1|Bo ghi chu|84000.000
ITEM|3|Ca kho to|52000.000|2|0|Them gung|104000.000
ITEM|12|Bia|15000.000|5|0||75000.000
END
```

**Ưu điểm:**
- Dễ phân tích và parse
- Rõ ràng từng item trong bill
- Dễ mở rộng thêm thông tin
- Dễ debug và kiểm tra file

---

## 3. Từng bước thực hiện

### 3.1 Bước 1: Sửa hàm `saveBillToHistoryFile()` trong `app/services/history_service.c`

**Mục tiêu**: Lưu toàn bộ chi tiết bill + items vào file cá nhân khách

**Thay đổi**:
```c
void saveBillToHistoryFile(int customerId, Bill* bill) {
    char filename[100];
    sprintf(filename, "app/database/history_khach_%d.txt", customerId);

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        file = fopen(filename, "w");
        if (file == NULL) return;
    }

    // Lưu dòng thông tin bill
    fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d\n",
            bill->id, customerId, bill->customer.name, bill->customer.phone,
            bill->dateTime, bill->total, bill->discount, bill->finalPrice, bill->itemCount);

    // Lưu từng item
    for (int i = 0; i < bill->itemCount; i++) {
        fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%s|%.3f\n",
                bill->items[i].id,
                bill->items[i].name,
                bill->items[i].price,
                bill->items[i].quantity,
                bill->items[i].option,
                bill->items[i].note,
                bill->items[i].totalPrice);
    }

    // Lưu dấu kết thúc
    fprintf(file, "END\n");
    fclose(file);
}
```

**Chi tiết thay đổi**:
- Thêm dòng header `BILL|...` với đủ thông tin
- Vòng lặp để lưu từng `ITEM|...`
- Dòng `END` để đánh dấu kết thúc bill

---

### 3.2 Bước 2: Sửa hàm `saveBillToGlobalFile()` trong `app/services/history_service.c`

**Mục tiêu**: Lưu toàn bộ chi tiết bill + items vào file chung `history.txt`

**Thay đổi**: Sử dụng cùng logic như `saveBillToHistoryFile()`
```c
void saveBillToGlobalFile(Bill* bill, int customerId) {
    FILE* file = fopen("app/database/history.txt", "a");
    if (file == NULL) {
        file = fopen("app/database/history.txt", "w");
        if (file == NULL) return;
    }

    // Lưu dòng thông tin bill
    fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d\n",
            bill->id, customerId, bill->customer.name, bill->customer.phone,
            bill->dateTime, bill->total, bill->discount, bill->finalPrice, bill->itemCount);

    // Lưu từng item
    for (int i = 0; i < bill->itemCount; i++) {
        fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%s|%.3f\n",
                bill->items[i].id,
                bill->items[i].name,
                bill->items[i].price,
                bill->items[i].quantity,
                bill->items[i].option,
                bill->items[i].note,
                bill->items[i].totalPrice);
    }

    // Lưu dấu kết thúc
    fprintf(file, "END\n");
    fclose(file);
}
```

---

### 3.3 Bước 3: Sửa hàm `loadHistoryFromFile()` trong `app/services/history_service.c`

**Mục tiêu**: Load lại toàn bộ thông tin bill + items từ file

**Logic mới**:
1. Đọc từng dòng trong file
2. Nếu dòng bắt đầu với `BILL|`, parse và tạo Bill mới
3. Nếu dòng bắt đầu với `ITEM|`, parse và thêm vào mảng items của Bill hiện tại
4. Nếu gặp `END`, hoàn tất bill hiện tại

**Code**:
```c
void loadHistoryFromFile() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return;
    }

    char line[500];
    Bill tempBill = {0};
    int itemCount = 0;
    int billStarted = 0;

    while (fgets(line, sizeof(line), file)) {
        // Xóa ký tự newline
        line[strcspn(line, "\n")] = 0;

        if (strncmp(line, "BILL|", 5) == 0) {
            // Parse dòng BILL
            int billId, customerId, itemCountInFile;
            char nameKhach[50], phoneKhach[15], dateTime[50];
            double total, discount, finalPrice;

            sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phoneKhach, dateTime,
                   &total, &discount, &finalPrice, &itemCountInFile);

            tempBill.id = billId;
            tempBill.total = total;
            tempBill.discount = discount;
            tempBill.finalPrice = finalPrice;
            tempBill.itemCount = itemCountInFile;
            strcpy(tempBill.dateTime, dateTime);
            
            // Lưu thông tin khách hàng (nếu cần)
            strcpy(tempBill.customer.name, nameKhach);
            strcpy(tempBill.customer.phone, phoneKhach);

            itemCount = 0;
            billStarted = 1;
        }
        else if (strncmp(line, "ITEM|", 5) == 0 && billStarted) {
            // Parse dòng ITEM
            if (itemCount < MAX_GIO_HANG) {
                int id, quantity, option;
                char name[50], note[100];
                double price, totalPrice;

                sscanf(line, "ITEM|%d|%[^|]|%lf|%d|%d|%[^|]|%lf",
                       &id, name, &price, &quantity, &option, note, &totalPrice);

                tempBill.items[itemCount].id = id;
                strcpy(tempBill.items[itemCount].name, name);
                tempBill.items[itemCount].price = price;
                tempBill.items[itemCount].quantity = quantity;
                tempBill.items[itemCount].option = option;
                strcpy(tempBill.items[itemCount].note, note);
                tempBill.items[itemCount].totalPrice = totalPrice;

                itemCount++;
            }
        }
        else if (strcmp(line, "END") == 0 && billStarted) {
            // Kết thúc bill, thêm vào linked list
            // Tìm customerId từ bill đã load
            addBillToHistory(tempBill.id / 1000, &tempBill); // (hoặc tìm từ btree)
            
            // Reset để đọc bill tiếp theo
            tempBill = {0};
            itemCount = 0;
            billStarted = 0;
        }
    }

    fclose(file);
}
```

**Lưu ý**:
- Sử dụng `strncmp()` để so sánh prefix
- Phân tích từng field của dòng BILL và ITEM
- Lưu trữ items vào mảng của Bill
- Dùng flag `billStarted` để biết là đang load bill nào

---

### 3.4 Bước 4: Xử lý file lịch sử cũ (tuỳ chọn)

**Vấn đề**: File lịch sử cũ dùng format cũ, sẽ không parse được

**Giải pháp**:
1. **Xóa file cũ**: Xóa toàn bộ `app/database/history_*.txt` và `app/database/history.txt`
2. **Hoặc migrate**: Viết script để đọc file cũ, chuyển sang format mới
   - Vì không có items trong file cũ, format mới sẽ là:
   ```
   BILL|billId|customerId|name|phone|dateTime|finalPrice|0|finalPrice|0
   END
   ```
   - (itemCount = 0, không có ITEM|...)

**Khuyến nghị**: Xóa file cũ vì chúng không chứa thông tin items

---

### 3.5 Bước 5: Cập nhật UI (nếu cần)

**File**: `app/ui/history_ui.c`

Đã cần sửa UI để xử lý format mới của file `history.txt` và `history_khach_X.txt`.

**Sửa `printAllHistory()`**
- Chỉ đọc dòng `BILL|...` từ file.
- Bỏ qua các dòng `ITEM|...` và `END`.
- Hiển thị thêm số lượng item của mỗi hóa đơn.

**Sửa `printCustomerHistory()`**
- Hiển thị chi tiết từng hóa đơn của khách.
- In ra `STT`, `ID món`, `Tên món`, `SL`, `Option`, `Ghi chú`, `Thành tiền`.
- Hiển thị thông tin khách hàng (tên, số điện thoại) khi xem lịch sử của khách.

**Sửa `printBillDetail()`**
- In chi tiết từng item với `ID`, `Tên món`, `SL`, `Option`, `Ghi chú`, `Thành tiền`.
- Hiển thị thông tin khách hàng và thời gian rõ ràng.

**Ví dụ sửa UI**:
```c
if (strncmp(line, "BILL|", 5) == 0) {
    // parse và hiển thị summary
}

for (int i = 0; i < current->bill.itemCount; i++) {
    printf("%-4d | %-4d | %-25.25s | %-3d | %-8s | %-20.20s | %12.3f\n",
           i + 1,
           current->bill.items[i].id,
           current->bill.items[i].name,
           current->bill.items[i].quantity,
           optLabel,
           current->bill.items[i].note,
           current->bill.items[i].totalPrice);
}
```

---

## 4. Thứ tự thực hiện

1. ✅ **Bước 1**: Sửa `saveBillToHistoryFile()`
2. ✅ **Bước 2**: Sửa `saveBillToGlobalFile()`
3. ✅ **Bước 3**: Sửa `loadHistoryFromFile()`
4. ✅ **Bước 4**: Xóa file lịch sử cũ hoặc migrate
5. ✅ **Bước 5**: Test chức năng xem chi tiết bill
6. ✅ **Bước 6**: Cập nhật UI nếu cần
7. ✅ **Bước 7**: Biên dịch và test toàn bộ

---

## 5. Kiểm tra kết quả

**Test case**:
1. Đặt một bill với 3 items
2. Thanh toán
3. Xem lịch sử → xem chi tiết bill
4. Kiểm tra file `app/database/history_khach_X.txt`
5. Xác nhận toàn bộ items hiển thị đúng

**Kỳ vọng**:
- File chứa đầy đủ thông tin items
- Khi xem chi tiết bill, hiển thị "Đã mua món gì" đầy đủ
- Các hóa đơn cũ vẫn hoạt động (hoặc cần xóa file cũ)

---

## 6. Ghi chú quan trọng

- Kiểm tra **buffer overflow** khi parse các trường có dấu '|'
- Sử dụng `%[^|]` trong sscanf để đọc đến dấu '|' tiếp theo
- Đảm bảo **độ dài buffer** đủ để chứa thông tin (ví dụ: `char line[500]`)
- **Nhất quán** giữa hàm save và hàm load
- Test với các **tên món dài**, **ghi chú dài** để chắc chắn không bị mất dữ liệu

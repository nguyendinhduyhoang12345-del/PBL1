# Fix Chức năng 5: Cập nhật file lịch sử để có Hạng và Tổng tiền tích lũy

## 1. Mục tiêu sửa

Muốn tất cả lịch sử hóa đơn đều chứa thêm:
- `rank` của khách hàng
- `totalSpent` (tổng tiền tích lũy của khách)

Và khi xem lịch sử hoặc in bill, hệ thống có thể hiển thị chính xác hai giá trị này.

## 2. File cần chỉnh sửa và vai trò

1. `app/models/models.h` - đảm bảo `Customer` có 2 trường `rank` và `totalSpent`
2. `app/controllers/checkout_controller.c` - gán dữ liệu rank/tổng tích lũy vào `currentBill` trước khi lưu
3. `app/services/history_service.c` - mở rộng format dòng `BILL` khi lưu và đọc
4. `app/ui/history_ui.c` - hiển thị `rank` và `totalSpent` khi in lịch sử
5. `app/services/file_service.c` - chỉ cần sửa nếu bạn muốn đồng bộ lại file `khachhang.txt`

## 3. Bước 1: Xác nhận model `Customer`

Mở `app/models/models.h` và đảm bảo `Customer` có nội dung như sau:

```c
typedef struct {
    int id;
    char phone[15];
    char name[50];
    char address[100];
    double totalSpent; // Tổng tiền khách đã chi tiêu
    char rank[20];     // Hạng khách hàng
} Customer;
```

Nếu bạn đã có `totalSpent` và `rank`, bước này đã xong.

## 4. Bước 2: Sửa `checkout_controller.c`

Đây là bước quan trọng nhất: trước khi gọi `saveBillToHistoryFile()` hoặc `saveBillToGlobalFile()`, bạn phải gán giá trị `rank` và `totalSpent` vào `currentBill->customer`.

Tìm đoạn mã trong `processCheckout()` sau khi tính `finalPrice` và trước khi lưu file.

### 4.1 Thêm đoạn này vào ngay trước `addBillToHistory(...)` hoặc `saveBillTo...`:

```c
if (khGoc != NULL) {
    strcpy(currentBill->customer.rank, khGoc->rank);
    currentBill->customer.totalSpent = khGoc->totalSpent + finalPrice;
}
else {
    strcpy(currentBill->customer.rank, "Khach vang lai");
    currentBill->customer.totalSpent = finalPrice;
}
```

### 4.2 Nếu bạn dùng `currentBill->customer.name` và `currentBill->customer.phone` để lưu khách hàng, hãy sửa thêm:

```c
currentBill->customer.id = khGoc->id;
strcpy(currentBill->customer.name, khGoc->name);
strcpy(currentBill->customer.phone, khGoc->phone);
```

Trong code hiện tại, phần `khGoc->totalSpent` đã được cập nhật, nhưng `currentBill->customer.totalSpent` vẫn cần gán riêng để file lịch sử lưu đúng giá trị.

## 5. Bước 3: Sửa `app/services/history_service.c`

### 5.1 Sửa `saveBillToHistoryFile()`

Mở `saveBillToHistoryFile()` và thay đoạn `fprintf` dòng BILL cũ bằng:

```c
fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d|%s|%.3f\n",
        bill->id,
        customerId,
        bill->customer.name,
        bill->customer.phone,
        bill->dateTime,
        bill->total,
        bill->discount,
        bill->finalPrice,
        bill->itemCount,
        bill->customer.rank,
        bill->customer.totalSpent);
```

### 5.2 Sửa `saveBillToGlobalFile()`

Mở `saveBillToGlobalFile()` và sửa tương tự:

```c
fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d|%s|%.3f\n",
        bill->id,
        customerId,
        bill->customer.name,
        bill->customer.phone,
        bill->dateTime,
        bill->total,
        bill->discount,
        bill->finalPrice,
        bill->itemCount,
        bill->customer.rank,
        bill->customer.totalSpent);
```

### 5.3 Sửa `loadHistoryFromFile()`

Mở hàm `loadHistoryFromFile()` và tìm đoạn parse `BILL|...`.

Thay bằng đoạn sau:

```c
int billId, customerId, itemCountInFile;
char nameKhach[50], phoneKhach[15], dateTime[50], rankKhach[20];
double total, discount, finalPrice, totalSpent;

sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d|%[^|]|%lf",
       &billId,
       &customerId,
       nameKhach,
       phoneKhach,
       dateTime,
       &total,
       &discount,
       &finalPrice,
       &itemCountInFile,
       rankKhach,
       &totalSpent);

strcpy(tempBill.customer.rank, rankKhach);
tempBill.customer.totalSpent = totalSpent;
```

Và giữ nguyên các dòng gán khác của `tempBill`.

### 5.4 Xác nhận `tempBill` đã nhận đủ dữ liệu

Sau khi parse xong, bạn phải gán:

```c
tempBill.customer.id = customerId;
strcpy(tempBill.customer.name, nameKhach);
strcpy(tempBill.customer.phone, phoneKhach);
strcpy(tempBill.dateTime, dateTime);
tempBill.total = total;
tempBill.discount = discount;
tempBill.finalPrice = finalPrice;
tempBill.itemCount = itemCountInFile;
strcpy(tempBill.customer.rank, rankKhach);
tempBill.customer.totalSpent = totalSpent;
```

## 6. Bước 4: Sửa `app/ui/history_ui.c`

Nếu file lịch sử đã mở rộng format, bạn có thể in thêm `rank` và `totalSpent`.

### 6.1 Với `printAllHistory()`

Thay header và printf hiện tại bằng:

```c
printf("%-8s | %-12s | %-20s | %-10s | %-10s | %-12s | %-10s | %-6s\n",
       "Bill ID", "Customer ID", "Thoi gian", "Rank", "Tich luy", "So tien", "Giam gia", "Items");
```

Và khi parse dòng BILL, thêm biến nhận `rank` và `totalSpent`.

Nếu hiển thị từ `Bill` đã load, chỉ cần in:

```c
printf("Rank: %s | Tong tich luy: %.3f VND\n",
       current->bill.customer.rank,
       current->bill.customer.totalSpent);
```
```

### 6.2 Với `printCustomerHistory()`

Ở đầu mỗi hóa đơn, thêm dòng:

```c
printf("Rank: %s | Tong tich luy: %.3f VND\n",
       current->bill.customer.rank,
       current->bill.customer.totalSpent);
```

### 6.3 Với `printBillDetail()`

Thêm trên hoặc dưới dòng thông tin khách hàng:

```c
printf("Rank: %s | Tong tich luy: %.3f VND\n",
       billNode->bill.customer.rank,
       billNode->bill.customer.totalSpent);
```

## 7. Fix chi tiết theo code hiện tại

### 7.1 `history_service.c` của bạn hiện đã mở rộng format

Đoạn `saveBillToHistoryFile()` và `saveBillToGlobalFile()` trong file hiện đang sử dụng đúng format

```c
BILL|...|%s|%.3f
```

nên bạn chỉ cần đảm bảo `currentBill` trước khi lưu chứa đủ dữ liệu.

### 7.2 `checkout_controller.c` cần sửa ngay tại đây

Hiện tại bạn cập nhật `khGoc->totalSpent += finalPrice;` nhưng chưa gán lại cho `currentBill->customer.totalSpent`.

Sửa đoạn sau:

```c
if (khGoc != NULL) {
    double oldTotal = khGoc->totalSpent;
    char oldRank[20];
    strcpy(oldRank, khGoc->rank);

    khGoc->totalSpent += finalPrice;
    updateCustomerRank(khGoc);

    strcpy(currentBill->customer.rank, khGoc->rank);
    currentBill->customer.totalSpent = khGoc->totalSpent;
    currentBill->customer.id = khGoc->id;
    strcpy(currentBill->customer.name, khGoc->name);
    strcpy(currentBill->customer.phone, khGoc->phone);

    addBillToHistory(currentBill->customer.id, currentBill);
    saveBillToHistoryFile(currentBill->customer.id, currentBill);
    saveBillToGlobalFile(currentBill, currentBill->customer.id);

    saveAllCustomersToFile();
    ...
}
```

Nếu khách là khách vãng lai, bạn vẫn nên gán `currentBill->customer.rank` và `totalSpent` để file lưu đầy đủ.

## 8. Cần làm rõ thêm nếu bạn muốn đồng bộ `khachhang.txt`

Nếu bạn dùng `totalSpent` và `rank` trong `Customer` để cập nhật hạng khách, thì sau khi checkout xong phải gọi `saveAllCustomersToFile()` như bạn đã có. Điều đó đảm bảo dữ liệu khách hàng và file lịch sử đều nhất quán.

## 9. Kết luận: sửa cụ thể

- Nếu chỉ muốn sửa `history_service.c`, bạn đã xong phần lưu/đọc.
- Nếu muốn sửa hoàn chỉnh, hãy sửa cả `checkout_controller.c` để `currentBill` mang đủ `rank` và `totalSpent`.
- Nếu không thấy `rank`/`totalSpent` trong lịch sử khi xem, nguyên nhân là bạn chưa gán vào `currentBill` trước khi lưu.

## 10. Đề xuất kiểm tra

1. Chạy chức năng thanh toán với khách hàng có rank.
2. Mở `app/database/history.txt` và kiểm tra dòng `BILL|...` có 11 phần hay không.
3. Chạy chức năng xem lịch sử, kiểm tra `Rank` và `Tong tich luy` hiển thị đúng.

Nếu bạn muốn, tôi có thể tiếp tục tạo thêm bản patch cụ thể cho `checkout_controller.c` và `history_ui.c`.
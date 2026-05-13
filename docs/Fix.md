# Fix chức năng Lịch sử theo SĐT

## Vấn đề cần sửa
Ở chức năng 1 của phần Lịch sử ("Xem lịch sử của khách hàng theo số điện thoại"), hiện tại `printCustomerHistory()` đang in chi tiết từng món trong mỗi hóa đơn.

Yêu cầu mong muốn: chỉ in bản tóm tắt lịch sử của khách theo SĐT, bao gồm những thông tin chính như:
- ID bill
- ID khách hàng
- ngày giờ
- tổng số món
- tổng tiền
- giảm giá
- tổng thanh toán

Không cần in chi tiết từng dòng món, chỉ cần một bảng tóm tắt mỗi hóa đơn.

## Cách sửa chi tiết
### 1. File cần sửa
- `app/ui/history_ui.c`

### 2. Thay đổi hàm `printCustomerHistory()`
- Bỏ phần in chi tiết các món (`CartNode *node = current->bill.cart.head; ...`) trong `printCustomerHistory()`.
- Thay bằng một bảng tóm tắt mỗi hóa đơn với các cột:
  - `Bill ID`
  - `Customer ID`
  - `Ngay`
  - `So mon`
  - `Tong tien`
  - `Giam gia`
  - `Tong` / `Thanh toan`

### 3. Nội dung in mẫu
Ví dụ, trong vòng lặp `while (current != NULL)` của `printCustomerHistory()` có thể thành như sau:
```c
printf("%-8s | %-12s | %-20s | %-8s | %-12s | %-10s | %-12s\n",
       "Bill ID", "Cust ID", "Ngay", "So mon", "Tong tien", "Giam gia", "Thanh toan");
printf("-------------------------------------------------------------------------------------\n");

int itemCount = 0;
CartNode *node = current->bill.cart.head;
while (node != NULL) {
    itemCount += node->item.quantity;
    node = node->next;
}

printf("%-8d | %-12d | %-20s | %-8d | %12.0f | %10.0f | %12.0f\n",
       current->bill.id,
       current->customerId,
       current->bill.dateTime,
       itemCount,
       current->bill.total,
       current->bill.discount,
       current->bill.finalPrice);
```

### 4. Giữ lại thông tin khách hàng phía trên
Vẫn giữ các dòng in:
- `Khach hang: ... | SDT: ...`
- `Rank: ... | Tong tich luy: ...`

### 5. Kết quả mong muốn
Sau sửa, khi chọn xem lịch sử theo SĐT, chương trình sẽ in ra danh sách hóa đơn của khách hàng chỉ với thông tin tóm tắt, không in danh sách món từng hóa đơn.

## Gợi ý mở rộng
Nếu muốn giữ cả hai chế độ:
- `printCustomerHistory()` hiển thị tóm tắt lịch sử
- `printBillDetail()` vẫn dùng để xem chi tiết từng hóa đơn theo ID bill

Thì chỉ cần chỉnh `printCustomerHistory()` như trên, còn `printBillDetail()` giữ nguyên.

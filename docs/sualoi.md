# Sửa lỗi PBL1

## 1. Chức năng 6 chưa triển khai
- `main.c` hiển thị menu `6. Thong ke doanh thu` nhưng không có `case 6` trong `switch`.
- Kết quả: chọn 6 sẽ rơi vào `default` và không thực hiện được chức năng.

## 2. Lỗi ghi file lịch sử khách hàng
- Trong `app/services/history_service.c`, `saveBillToHistoryFile()` ghi bằng `fprintf(file, "%d|%d|%s|%.3f|%3f\n", ...)`.
- `"%3f"` không nhất quán và nên sửa thành `"%.3f"`.
- Tên file được tạo bằng `history_khach_%d.txt` nhưng dữ liệu thực tế trong thư mục sử dụng tên `history_khach_1.txt`, `history_khach_10.txt`.
- Cần đảm bảo tên file đồng nhất với dữ liệu hoặc đổi tên file hiện có.

## 3. Lịch sử chỉ load file `history.txt`
- `loadHistoryFromFile()` chỉ đọc `app/database/history.txt`.
- Các file riêng `history_khach_*.txt` hiện không được load lại khi khởi động.
- Do vậy dữ liệu riêng của khách không được phục hồi từ file.

## 4. Vấn đề validate/nhập liệu chưa đầy đủ
- `customer_controller.c` dùng `scanf("%s", sdt);` chưa giới hạn độ dài chuỗi.
- `history_controller.c` dùng `scanf("%s", phone);` cũng tương tự.
- `checkout_controller.c` dùng `scanf("%lf", &customerMoney)` và `scanf("%d", &xacNhan)` mà không validate nếu người dùng nhập chữ hoặc ký tự không hợp lệ.
- Chưa có `getValidDouble()`; chỉ có `getValidInt()` cho số nguyên.
- SDT chỉ kiểm tra `strlen(sdt) == 10`, chưa kiểm tra ký tự có phải số hay không.

## 5. Mã nguồn cần dọn warning
- `app/controllers/checkout_controller.c`: tham số `BTreeNode* btreeRoot` shadow global `btreeRoot`.
- `app/ui/bill_ui.c`: tham số `subtotal`, `discount`, `finalPrice` không dùng, gây warning `-Wunused-parameter`.
- `app/ui/history_ui.c`: tham số `historyHead` trong `printCustomerHistory()` shadow global `historyHead`.

## 6. Các hàm/biến cần xem lại
- `getCustomerHistory(int customerId)` trong `app/services/history_service.c` có thể dùng tốt nhưng hiện tại chỉ dùng khi tìm lịch sử khách.
- `HistoryNode* searchBillById(int billId)` và `getCustomerIdByPhone()` đều sử dụng được.
- `loadMenuFromFile()` và `loadCustomersFromFile()` chưa kiểm tra đầy đủ các dòng file bị hỏng hoặc thiếu trường.

## 7. Đề xuất sửa nhanh
1. Thêm `case 6` vào `main.c` và gọi hàm thống kê/ghi file báo cáo.
2. Sửa `fprintf()` trong `saveBillToHistoryFile()` và chuẩn hóa tên file `history_khach_%d.txt`.
3. Thêm validate an toàn cho `double` và `int`, thay `scanf` bằng hàm kiểm tra đầu vào.
4. Giới hạn độ dài khi dùng `scanf("%s", ...)` thành `scanf("%14s", sdt)` hoặc dùng `fgets()`.
5. Sửa các tên biến tránh shadow và xóa tham số thừa để dọn warnings.

## 8. Gợi ý chức năng 6
- Có thể xuất báo cáo vào `app/database/baocao.txt`.
- Nội dung báo cáo nên gồm: tổng số hóa đơn, tổng doanh thu, tổng giảm giá, số hóa đơn theo khách hàng, hoặc chi tiết hóa đơn.
- Đây là phần bổ sung cần làm để menu chức năng 6 thực sự hoàn chỉnh.

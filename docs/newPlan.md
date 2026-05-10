# Ý tưởng mới cho phần đặt món và giỏ hàng

## 1. Mục tiêu

Tách hai phần rõ ràng:
1. Phần `Đặt món`: nhân viên dùng mã món và tìm nhanh trong Hash Table được nạp từ `menu.txt`.
2. Phần `Giỏ hàng`: dùng Doubly Linked List để quản lý các món đã chọn với 3 chức năng chính là Thêm, Xóa, Chỉnh sửa.

## 2. Luồng dữ liệu tổng quát

menu.txt ──load──► Hash Table (RAM)
                          │
                     nhân viên tìm món
                          │
                          ▼
                   tìm thấy MenuItem
                          │
                     thêm vào giỏ
                          │
                          ▼
              Doubly Linked List (giỏ hàng)

## 3. Phần 1: Đặt món (Hash Table)

1. Ứng dụng đọc dữ liệu từ `menu.txt` vào dãy `MenuItem`.
2. Tạo một `HashTable` RAM để tra cứu món nhanh theo `itemId`.
3. Nhân viên nhập `mã món`.
4. Dùng hàm tìm kiếm trong Hash Table để lấy `MenuItem` tương ứng.
5. Nếu tìm được món và số lượng hợp lệ:
   - kiểm tra tồn kho
   - kiểm tra ràng buộc số lượng món chính nếu cần
   - xây dựng `OrderItem` từ thông tin `MenuItem`
6. Gọi `addToCart()` để chuyển `OrderItem` vào giỏ hàng.

## 4. Phần 2: Giỏ hàng (Doubly Linked List)

Giỏ hàng sẽ dùng `DoublyLinkedList` làm cấu trúc chính, với mỗi node chứa một `OrderItem`.

### Các chức năng chính:
- Thêm món
- Xóa món
- Chỉnh sửa số lượng

### Mục tiêu của Doubly Linked List:
- Hiển thị giỏ hàng theo thứ tự thêm
- Duyệt đến vị trí STT nhanh hơn
- Cập nhật xóa/chỉnh sửa tại vị trí cụ thể

## 5. Luồng chi tiết cho giỏ hàng

1. Khi nhân viên bấm "Thêm vào giỏ":
   - Tạo `OrderItem` từ `MenuItem`
   - Thêm node mới vào cuối `DoublyLinkedList`
   - Cập nhật số lượng món

2. Khi nhân viên bấm "Xóa":
   - Nhập STT món cần xóa
   - Dò theo vị trí trong doubly linked list
   - Xóa node tương ứng
   - Hoàn lại tồn kho nếu cần

3. Khi nhân viên bấm "Chỉnh sửa":
   - Nhập STT món cần sửa
   - Dò node tại vị trí đó
   - Thay đổi `quantity`, có thể thay đổi `totalPrice`
   - Cập nhật lại danh sách và số lượng

## 6. Tương tác giữa Hash Table và Doubly Linked List

- `HashTable` chỉ dùng cho phần tìm menu nhanh khi đặt món.
- `DoublyLinkedList` giữ dữ liệu giỏ hàng sau khi nhân viên xác nhận chọn món.
- Khi thêm món, `OrderItem` lấy từ `MenuItem` rồi push vào Doubly Linked List.
- Khi cần hiển thị/ sửa/ xóa giỏ hàng, chỉ thao tác trên Doubly Linked List.

## 7. Lợi ích của cách thiết kế này

- Tìm món nhanh, không phải duyệt toàn bộ menu mỗi lần nhập mã.
- Giỏ hàng linh hoạt, cho phép thêm/xóa/chỉnh sửa theo STT đơn giản.
- Cấu trúc rõ ràng: menu lookup tách biệt khỏi quản lý giỏ.

## 8. Gợi ý thực hiện

- `menu.txt` load vào `MenuItem[]`.
- Dùng `hashTableInsertMenuItem()` để xây `HashTable` menu.
- Hàm `addToCart()` nhận `OrderItem` và append vào `DoublyLinkedList`.
- Giỏ hàng nên có hàm:
  - `cartAddItem()`
  - `cartRemoveItemByIndex()`
  - `cartUpdateQuantityByIndex()`

---

Phần này là ý tưởng mới theo đúng mô hình bạn muốn: `menu.txt` → `Hash Table` để tìm nhanh, và `Doubly Linked List` để quản lý giỏ hàng.
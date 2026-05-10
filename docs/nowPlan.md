# Luồng hoạt động phần đặt món và giỏ hàng

## 1. Tổng quan

Luồng đặt món và giỏ hàng hiện tại vận hành theo cả hai thành phần:
- `HashTable` để lưu trữ dữ liệu món đã đặt theo `itemId`.
- `DoublyLinkedList` để hiển thị và thao tác trực quan trên giỏ hàng.

`Bill` dùng `itemsHash` để lưu trạng thái chính, `cartDisplay` để quản lý vị trí và hiển thị trong UI.

## 2. Khi người dùng chọn đặt món (`handleOrderMenu`)

1. `handleOrderMenu(Bill *currentBill)` được gọi.
2. Nếu `currentBill->itemsHash` chưa khởi tạo, hàm tạo hash table mới.
3. Nếu `currentBill->cartDisplay` chưa khởi tạo, hàm tạo doubly linked list mới.
4. Gọi `showMenuUI()` để hiển thị menu món ăn.
5. Gọi `addToCart(currentBill)` để cho người dùng nhập món.

## 3. Cách `addToCart()` hoạt động

1. Hàm hiển thị số lượng món chính đã đặt và yêu cầu người dùng nhập mã món (`idNhap`).
2. Nếu nhập `0`, thoát vòng lặp.
3. Kiểm tra mã món hợp lệ và điều kiện giới hạn:
   - Món chính (`id <= 12`) chỉ được đặt tối đa 5 món.
   - Số lượng nhập phải lớn hơn 0 và không vượt quá kho.
4. Nếu món có tùy chọn (`hasOptions`), yêu cầu người dùng chọn kiểu và cộng giá nếu cần.
5. Ghi chú cũng được nhập nếu người dùng muốn.
6. Tạo `OrderItem item` từ dữ liệu nhập:
   - `id`, `name`, `price`, `quantity`, `option`, `note`, `totalPrice`.
7. Gọi `hashTableInsert(currentBill->itemsHash, &item)`:
   - Lưu món vào hash table theo `item.id`.
   - Nếu món đã tồn tại cùng `item.id`, hàm cập nhật số lượng hoặc ghi đè tùy thiết kế nội bộ của hash table.
8. Nếu chèn thành công:
   - Cập nhật `currentBill->itemCount = currentBill->itemsHash->count`.
   - Giảm tồn kho `menu[idx].stock`.
   - Nếu cần, thêm mục vào `currentBill->cartDisplay` bằng `dllAppend()` để phản ánh danh sách hiển thị.

## 4. Hiển thị giỏ hàng (`showCartUI`)

1. `handleCartMenu()` gọi `showCartUI(currentBill)` mỗi lần vào menu giỏ hàng.
2. `showCartUI()` kiểm tra `currentBill->itemCount`.
   - Nếu rỗng, hiển thị thông báo giỏ trống.
3. Nếu có món, hàm gọi `hashTableToArray(currentBill->itemsHash, &count)` để lấy tất cả `OrderItem` từ hash table.
4. Duyệt mảng trả về và in ra:
   - STT, tên món, số lượng, đơn giá, thành tiền.
   - Hiển thị thêm `option` và `note` nếu có.
5. Tính tổng tạm và in ra.
6. Cuối cùng, giải phóng mảng trả về bằng `free(arr)`.

## 5. Thao tác giỏ hàng

### Xóa món (`removeFromCart`)

1. Người dùng nhập STT món cần xóa.
2. Dùng `dllGetNodeAt(currentBill->cartDisplay, stt)` để lấy node trong danh sách hiển thị.
3. Lấy `itemId` và `quantity` từ node.
4. Xóa trong hash table bằng `hashTableDelete(currentBill->itemsHash, itemId)`.
5. Xóa node trong doubly linked list bằng `dllRemoveAt(currentBill->cartDisplay, stt)`.
6. Hoàn lại tồn kho `menu[menuIdx].stock += qty`.
7. Cập nhật `currentBill->itemCount = currentBill->itemsHash->count`.

### Sửa số lượng (`updateQuantity`)

1. Người dùng chọn STT món muốn sửa.
2. Dùng `dllGetNodeAt(currentBill->cartDisplay, stt)` để lấy node.
3. Lấy `itemId`, vị trí menu và số lượng cũ.
4. Nhập số lượng mới (`newQty`).
5. Kiểm tra điều kiện:
   - `newQty` phải > 0.
   - Nếu là món chính (`itemId <= 12`), tổng món chính sau khi sửa không vượt quá 5.
   - Kho còn đủ để tăng thêm số lượng.
6. Cập nhật cả hai cấu trúc:
   - `dllUpdateQuantity(currentBill->cartDisplay, stt, newQty)`.
   - `hashTableUpdateQuantity(currentBill->itemsHash, itemId, newQty)`.
7. Điều chỉnh tồn kho `menu[menuIdx].stock -= diff`.

## 6. Tại sao dùng cả Hash Table và Doubly Linked List

- `HashTable` giúp tra cứu, thêm, xóa món nhanh theo `itemId`.
- `DoublyLinkedList` giúp lấy nhanh theo STT để hiển thị và sửa/xóa theo vị trí trong giỏ.
- `itemCount` và `hashTableToArray()` dùng để tổng hợp số món và tính toán đơn giản.

## 7. Tóm tắt luồng chính

1. Người dùng vào `Đặt món` → `handleOrderMenu()`
2. Khởi tạo cấu trúc dữ liệu nếu cần
3. Nhập món, kiểm tra giới hạn, chèn vào `itemsHash`
4. Cập nhật `cartDisplay` để hiển thị giỏ hàng
5. Người dùng vào `Giỏ hàng` → `handleCartMenu()`
6. Hiển thị danh sách từ `hashTableToArray()`
7. Xóa hoặc sửa số lượng bằng cách đồng bộ `itemsHash` và `cartDisplay`

---

Nội dung này là mô tả luồng hiện tại của dự án theo các hàm đang có trong mã nguồn.
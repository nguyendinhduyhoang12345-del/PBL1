# Knowledge Base - PBL1 Quản Lý Quán Cơm TAM

## Tổng Quan Dự Án
Dự án PBL1 là hệ thống quản lý quán cơm TAM được phát triển bằng ngôn ngữ C, sử dụng cấu trúc dữ liệu động và lưu trữ file để quản lý menu, khách hàng, đơn hàng và thanh toán.

## Mục Tiêu
- Quản lý menu món ăn với tồn kho động
- Xử lý đặt món với giới hạn 5 món chính
- Quản lý thông tin khách hàng thành viên với hạng và giảm giá
- Tính toán thanh toán với ưu đãi thành viên
- Lưu trữ lịch sử đơn hàng và thống kê doanh thu

## Cấu Trúc Dự Án
```
PBL1/
├── main.c                 # File chính, menu chính
├── app/
│   ├── controllers/       # Xử lý logic nghiệp vụ
│   │   ├── order_controller.c/h     # Đặt món, giỏ hàng
│   │   ├── customer_controller.c/h  # Quản lý khách hàng
│   │   └── checkout_controller.c/h  # Thanh toán
│   ├── models/
│   │   └── models.h        # Định nghĩa struct dữ liệu
│   ├── services/           # Logic xử lý dữ liệu
│   │   ├── pricing_service.c/h     # Tính tiền, giảm giá
│   │   ├── ranking_service.c/h     # Cập nhật hạng KH
│   │   ├── file_service.c/h        # Đọc/ghi file
│   │   └── btree_service.c/h       # Cấu trúc B-Tree cho KH
│   ├── ui/                # Giao diện người dùng
│   │   ├── menu_ui.c/h    # Hiển thị menu
│   │   ├── cart_ui.c/h    # Hiển thị giỏ hàng
│   │   └── bill_ui.c/h    # In hóa đơn
│   ├── utils/             # Tiện ích
│   │   ├── helper.c/h     # Hàm hỗ trợ
│   │   └── validator.c/h  # Validate input
│   └── database/          # File dữ liệu
│       ├── menu.txt       # Danh sách món ăn
│       ├── khachhang.txt  # Thông tin khách hàng
│       └── history.txt    # Lịch sử đơn hàng
```

## Cấu Trúc Dữ Liệu Chính

### MenuItem
- id: Mã món
- name: Tên món
- price: Giá
- stock: Tồn kho (tự động tạo theo giờ)
- hasOptions: Có tùy chọn (sườn cốt lết/cay)

### OrderItem
- id: Mã món
- name: Tên
- price: Giá thực tế (có thể +10k cho sườn cay)
- quantity: Số lượng
- option: Tùy chọn (1: cốt lết, 2: cay)
- note: Ghi chú
- totalPrice: Tổng tiền item

### Customer
- id: Mã KH
- phone: SĐT (10 số)
- name: Tên
- address: Địa chỉ
- totalSpent: Tổng chi tiêu
- rank: Hạng (Bronze/Silver/Gold/Diamond)

### Bill
- id: Mã hóa đơn
- customer: Thông tin KH
- items[]: Danh sách món
- itemCount: Số món
- total: Tổng tiền
- discount: Giảm giá
- finalPrice: Thành tiền
- dateTime: Thời gian

### CTDL Động
- HistoryNode: Linked list cho lịch sử hóa đơn
- BTreeNode: B-Tree order 4 cho tìm kiếm KH theo SĐT

## Quy Trình Hoạt Động

### 1. Khởi Động
- Load menu từ file menu.txt
- Load KH từ file khachhang.txt vào B-Tree
- Khởi tạo giỏ hàng rỗng

### 2. Menu Chính
1. **Đặt món**: Chọn món, số lượng, tùy chọn, ghi chú
   - Giới hạn 5 món chính (id 1-12)
   - Món thêm và nước không giới hạn
   - Kiểm tra tồn kho

2. **Xem/chỉnh sửa giỏ hàng**: Thêm/bớt/sửa số lượng

3. **Nhập thông tin KH**: Tìm theo SĐT, đăng ký mới nếu chưa có

4. **Thanh toán**:
   - Tính tổng tiền
   - Áp dụng giảm giá theo hạng KH
   - Nhận tiền, tính tiền thừa
   - In hóa đơn
   - Cập nhật tổng chi tiêu và hạng KH
   - Lưu KH và lịch sử

5. **Xem lịch sử**: Hiển thị các hóa đơn đã thanh toán

6. **Thống kê doanh thu**: (Chưa triển khai)

## Hệ Thống Hạng Thành Viên
- **Bronze**: < 1tr VND
- **Silver**: 1tr - 5tr VND (giảm 10%)
- **Gold**: 5tr - 10tr VND (giảm 20%)
- **Diamond**: >= 10tr VND (giảm 30%)

## Quản Lý Tồn Kho
Tồn kho tự động tạo ngẫu nhiên theo khung giờ:
- 6-11h: 400-500
- 11-14h: 300-400
- 14-17h: 270-300
- 17-21h: 50-270
- Ngoài giờ: 0-50
- Nước uống: 500-1000

## Công Nghệ Sử Dụng
- **Ngôn ngữ**: C
- **CTDL**: Array, Linked List, B-Tree
- **Lưu trữ**: File text (menu.txt, khachhang.txt, history.txt)
- **UI**: Console-based với ASCII art
- **Build**: GCC compiler

## Điểm Mạnh
- Tách biệt rõ ràng MVC (Models, Controllers, Services)
- Sử dụng CTDL phù hợp (B-Tree cho tìm kiếm nhanh)
- Validate input chặt chẽ
- Tính năng thành viên với ưu đãi
- Lưu trữ persistent

## Điểm Cần Cải Thiện
- Chưa có thống kê doanh thu chi tiết
- UI console đơn giản, chưa thân thiện
- Không có multi-threading
- Lịch sử chưa được lưu file đầy đủ
- Thiếu backup và recovery
# Sơ Đồ Tổ Chức Chương Trình

## 1. Cấu Trúc Thư Mục

```
PBL1/
├── main.c                          # Điểm vào chính
├── run.bat                         # Script chạy chương trình
├── README.md
│
├── app/                            # Thư mục chính của ứng dụng
│   ├── test.c
│   ├── controllers/                # Điều khiển luồng chương trình
│   │   ├── checkout_controller.c/h
│   │   ├── customer_controller.c/h
│   │   ├── history_controller.c/h
│   │   ├── order_controller.c/h
│   │   └── report_controller.c/h
│   │
│   ├── services/                   # Logic xử lý dữ liệu
│   │   ├── btree_service.c/h       # Cây nhị phân tìm kiếm
│   │   ├── cart_service.c/h        # Quản lý giỏ hàng
│   │   ├── checkout_service.c/h    # Xử lý thanh toán
│   │   ├── file_service.c/h        # Đọc/ghi file
│   │   ├── hash_service.c/h        # Hash table
│   │   ├── history_service.c/h     # Quản lý lịch sử
│   │   └── report_service.c/h      # Tính toán báo cáo
│   │
│   ├── ui/                         # Giao diện người dùng
│   │   ├── bill_ui.c/h             # Hiển thị hóa đơn
│   │   ├── cart_ui.c/h             # Hiển thị giỏ hàng
│   │   ├── customer_ui.c/h         # Hiển thị khách hàng
│   │   ├── history_ui.c/h          # Hiển thị lịch sử
│   │   ├── menu_ui.c/h             # Hiển thị menu món ăn
│   │   ├── report_ui.c/h           # Hiển thị báo cáo
│   │   └── welcome_ui.c/h          # Giao diện chào mừng
│   │
│   ├── models/                     # Định nghĩa cấu trúc dữ liệu
│   │   └── models.h
│   │
│   ├── utils/                      # Hàm tiện ích
│   │   ├── helper.c/h              # Hàm hỗ trợ chung
│   │   └── validator.c/h           # Kiểm tra đầu vào
│   │
│   └── database/                   # Lưu trữ dữ liệu
│       ├── menu.txt                # Dữ liệu menu
│       ├── khachhang.txt           # Dữ liệu khách hàng
│       ├── history.txt             # Lịch sử hóa đơn chung
│       ├── history_khach_*.txt     # Lịch sử từng khách hàng
│       ├── report_doanhthu.txt     # Báo cáo doanh thu
│       └── ...
│
└── docs/                           # Tài liệu dự án
    ├── ARCHITECTURE.md             # File này
    ├── BTree.md
    ├── chucNang1.md - chucNang6.md
    ├── flowchart.md
    └── LUONG_HOAT_DONG_DU_AN.md
```

## 2. Sơ Đồ Kiến Trúc Tầng

```mermaid
graph TB
    subgraph UI["Tầng Giao Diện - UI Layer"]
        WelcomeUI["welcome_ui"]
        MenuUI["menu_ui"]
        CartUI["cart_ui"]
        BillUI["bill_ui"]
        HistoryUI["history_ui"]
        CustomerUI["customer_ui"]
        ReportUI["report_ui"]
    end
    
    subgraph Controller["Tầng Điều Khiển - Controller Layer"]
        OrderCtrl["order_controller"]
        CheckoutCtrl["checkout_controller"]
        HistoryCtrl["history_controller"]
        CustomerCtrl["customer_controller"]
        ReportCtrl["report_controller"]
    end
    
    subgraph Service["Tầng Logic - Service Layer"]
        CartSvc["cart_service"]
        CheckoutSvc["checkout_service"]
        HistorySvc["history_service"]
        FileSvc["file_service"]
        ReportSvc["report_service"]
        BTreeSvc["btree_service"]
        HashSvc["hash_service"]
    end
    
    subgraph Utils["Tầng Tiện Ích - Utils Layer"]
        Helper["helper.c"]
        Validator["validator.c"]
    end
    
    subgraph Data["Tầng Dữ Liệu - Data Layer"]
        Menu["menu.txt"]
        Customer["khachhang.txt"]
        History["history.txt"]
        Database[(Database Files)]
    end
    
    WelcomeUI --> OrderCtrl
    MenuUI --> OrderCtrl
    CartUI --> OrderCtrl
    BillUI --> CheckoutCtrl
    HistoryUI --> HistoryCtrl
    CustomerUI --> CustomerCtrl
    ReportUI --> ReportCtrl
    
    OrderCtrl --> CartSvc
    CheckoutCtrl --> CheckoutSvc
    HistoryCtrl --> HistorySvc
    ReportCtrl --> ReportSvc
    
    CartSvc --> FileSvc
    CheckoutSvc --> FileSvc
    HistorySvc --> FileSvc
    ReportSvc --> BTreeSvc
    ReportSvc --> HashSvc
    
    CartSvc --> Helper
    OrderCtrl --> Validator
    CheckoutCtrl --> Validator
    HistoryCtrl --> Validator
    
    FileSvc --> Database
    FileSvc --> Menu
    FileSvc --> Customer
    FileSvc --> History
```

## 3. Luồng Chính của Chương Trình

```mermaid
graph LR
    Start["Start<br/>main.c"] --> Welcome["Welcome Screen<br/>welcome_ui"]
    Welcome --> MainMenu{"Main Menu"}
    
    MainMenu -->|1| Order["Order Menu<br/>order_controller"]
    MainMenu -->|2| Checkout["Checkout<br/>checkout_controller"]
    MainMenu -->|3| History["View History<br/>history_controller"]
    MainMenu -->|4| Customer["Customer Info<br/>customer_controller"]
    MainMenu -->|5| Report["Generate Report<br/>report_controller"]
    MainMenu -->|0| Exit["Exit"]
    
    Order --> OrderCart["Add/Edit Cart"]
    OrderCart --> MainMenu
    
    Checkout --> Pay["Process Payment"]
    Pay --> MainMenu
    
    History --> HistoryInfo["View Bills"]
    HistoryInfo --> MainMenu
    
    Customer --> CustomerData["View Customers"]
    CustomerData --> MainMenu
    
    Report --> ReportData["Generate Report"]
    ReportData --> MainMenu
    
    Exit --> End["End"]
```

## 4. Quan Hệ Giữa Các Module

| Module | Chức Năng | Phụ Thuộc |
|--------|-----------|----------|
| **order_controller** | Quản lý đặt hàng | cart_service, menu_ui, validator |
| **checkout_controller** | Xử lý thanh toán | checkout_service, bill_ui |
| **history_controller** | Xem lịch sử | history_service, history_ui |
| **customer_controller** | Quản lý khách hàng | btree_service, customer_ui |
| **report_controller** | Báo cáo doanh thu | report_service, report_ui |
| **cart_service** | Logic giỏ hàng | file_service, helper |
| **checkout_service** | Logic thanh toán | file_service, helper |
| **history_service** | Lưu/lấy lịch sử | file_service |
| **report_service** | Tính toán báo cáo | btree_service, hash_service |
| **file_service** | Đọc/ghi file | helper |
| **btree_service** | Cấu trúc dữ liệu B-Tree | - |
| **hash_service** | Hash table | - |

## 5. Cấu Trúc Dữ Liệu Chính

```mermaid
erDiagram
    CUSTOMER ||--o{ BILL : has
    BILL ||--o{ BILL_ITEM : contains
    MENU ||--o{ BILL_ITEM : references
    
    CUSTOMER {
        int customerId
        string name
        string phone
        date createdAt
    }
    
    BILL {
        int billId
        int customerId
        string customerName
        string customerPhone
        string dateTime
        double totalPrice
        double discount
        double finalPrice
        int itemCount
    }
    
    BILL_ITEM {
        int itemId
        string itemName
        string itemOption
        int quantity
        double price
        double totalPrice
        string note
    }
    
    MENU {
        int itemId
        string itemName
        double price
        string category
        string description
    }
```

## 6. Quy Trình Xử Lý Dữ Liệu

### Quy Trình Đặt Hàng
```
order_controller
    ↓
menu_ui (hiển thị menu)
    ↓
validator (kiểm tra input)
    ↓
cart_service (thêm vào giỏ)
    ↓
Giỏ hàng (Bill.cart)
```

### Quy Trình Thanh Toán
```
checkout_controller
    ↓
checkout_service (tính toán)
    ↓
file_service (lưu vào history.txt)
    ↓
file_service (lưu vào history_khach_N.txt)
    ↓
bill_ui (hiển thị hóa đơn)
```

### Quy Trình Báo Cáo
```
report_controller
    ↓
report_service (tính toán từ file)
    ↓
btree_service / hash_service (sắp xếp dữ liệu)
    ↓
report_ui (hiển thị + xuất file)
    ↓
report_doanhthu.txt
```

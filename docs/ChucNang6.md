# Chức năng 6: Báo cáo Doanh thu và Thống kê

## 1. Tổng quan Chức năng

Chức năng 6 cho phép người dùng xem báo cáo tổng hợp doanh thu của quán cơm Tấm, bao gồm:
- **Tổng quan**: Tổng số hóa đơn, tổng doanh thu, tổng số món đã bán, doanh thu trung bình/bill
- **Phân tích theo khách hàng**: Doanh thu và số lượng hóa đơn của từng khách
- **Xuất báo cáo**: Lưu báo cáo ra file văn bản có định dạng đẹp

Chức năng đọc dữ liệu từ file `app/database/history.txt` (format BILL mới với chi tiết items).

## 2. Kiến trúc Tổng quan

Chức năng được tổ chức theo kiến trúc MVC rõ ràng:

```
app/
├── models/models.h          # Struct CustomerRevenue
├── services/
│   ├── report_service.h     # Khai báo hàm service
│   └── report_service.c     # Logic tính toán báo cáo
├── ui/
│   ├── report_ui.h          # Khai báo hàm UI
│   └── report_ui.c          # Hiển thị và xuất báo cáo
└── main.c                   # Menu chính + case 6
```

**Nguyên tắc thiết kế:**
- **Models**: Chứa struct dữ liệu
- **Services**: Xử lý logic nghiệp vụ (tính toán)
- **UI**: Xử lý giao diện (hiển thị, xuất file)
- **Main**: Điều phối và gọi hàm

## 3. Chi tiết Từng File

### 3.1 `app/models/models.h` - Cấu trúc Dữ liệu

Thêm struct sau vào cuối file:

```c
// Struct cho báo cáo doanh thu theo khách hàng
typedef struct {
    int customerId;        // ID khách hàng
    int billCount;         // Số lượng hóa đơn của khách
    double revenue;        // Tổng doanh thu của khách
} CustomerRevenue;
```

**Giải thích:**
- `customerId`: Liên kết với ID trong file khách hàng
- `billCount`: Đếm số hóa đơn để tính tần suất mua
- `revenue`: Tổng tiền khách đã chi tiêu

### 3.2 `app/services/report_service.h` - Header Service

Tạo file mới với nội dung:

```c
#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include "../models/models.h"

// ============ HÀM CHÍNH ============

// Hàm chính xử lý báo cáo doanh thu
void handleGenerateReport();

// ============ HÀM TÍNH TOÁN ============

// Tính tổng doanh thu từ tất cả hóa đơn
double calculateTotalRevenue();

// Tính tổng số hóa đơn
int calculateTotalBills();

// Tính tổng số món đã bán
int calculateTotalItems();

// Phân tích doanh thu theo từng khách hàng
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount);

#endif
```

**Giải thích:**
- Header guard để tránh include trùng lặp
- Tách biệt hàm chính và hàm helper
- Sử dụng struct từ models.h

### 3.3 `app/services/report_service.c` - Logic Service

Tạo file mới với nội dung:

```c
#include "report_service.h"
#include "../ui/report_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100  // Giới hạn số khách hàng tối đa

// ============ HÀM HELPER NỘI BỘ ============

// Tìm hoặc tạo entry mới cho khách hàng trong mảng thống kê
CustomerRevenue* findOrCreateCustomer(CustomerRevenue* customerStats, int* customerCount, int customerId) {
    // Duyệt mảng để tìm khách đã tồn tại
    for (int i = 0; i < *customerCount; i++) {
        if (customerStats[i].customerId == customerId) {
            return &customerStats[i];
        }
    }

    // Tạo entry mới nếu chưa có
    if (*customerCount < MAX_CUSTOMERS) {
        customerStats[*customerCount].customerId = customerId;
        customerStats[*customerCount].billCount = 0;
        customerStats[*customerCount].revenue = 0.0;
        return &customerStats[(*customerCount)++];
    }

    return NULL;  // Quá giới hạn
}

// ============ HÀM TÍNH TOÁN ============

// Tính tổng doanh thu từ file history.txt
double calculateTotalRevenue() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        printf("[!] Khong the mo file history.txt\n");
        return 0.0;
    }

    double totalRevenue = 0.0;
    char line[500];

    // Đọc từng dòng trong file
    while (fgets(line, sizeof(line), file)) {
        // Loại bỏ ký tự xuống dòng
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        // Chỉ xử lý dòng BILL
        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemCount;
            char nameKhach[50], phone[15], dateTime[50];
            double total, discount, finalPrice;

            // Parse thông tin từ dòng BILL
            sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount);

            // Cộng dồn doanh thu
            totalRevenue += finalPrice;
        }
    }

    fclose(file);
    return totalRevenue;
}

// Đếm tổng số hóa đơn
int calculateTotalBills() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return 0;
    }

    int billCount = 0;
    char line[500];

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "BILL|", 5) == 0) {
            billCount++;
        }
    }

    fclose(file);
    return billCount;
}

// Tính tổng số món đã bán
int calculateTotalItems() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return 0;
    }

    int itemCount = 0;
    char line[500];

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemsInBill;
            char nameKhach[50], phone[15], dateTime[50];
            double total, discount, finalPrice;

            sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemsInBill);

            itemCount += itemsInBill;
        }
    }

    fclose(file);
    return itemCount;
}

// Phân tích doanh thu theo khách hàng
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount) {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return;
    }

    *customerCount = 0;
    char line[500];

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemCount;
            char nameKhach[50], phone[15], dateTime[50];
            double total, discount, finalPrice;

            sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount);

            // Tìm hoặc tạo entry cho khách hàng
            CustomerRevenue* cust = findOrCreateCustomer(customerStats, customerCount, customerId);
            if (cust != NULL) {
                cust->billCount++;
                cust->revenue += finalPrice;
            }
        }
    }

    fclose(file);
}

// ============ HÀM CHÍNH ============

void handleGenerateReport() {
    printf("\nDang thuc hien thong ke doanh thu...\n\n");

    // Bước 1: Tính toán các chỉ số tổng quan
    double totalRevenue = calculateTotalRevenue();
    int totalBills = calculateTotalBills();
    int totalItems = calculateTotalItems();

    // Bước 2: Phân tích theo khách hàng
    CustomerRevenue customerStats[MAX_CUSTOMERS];
    int customerCount = 0;
    analyzeByCustomer(customerStats, &customerCount);

    // Bước 3: Gọi UI để hiển thị và xuất báo cáo
    printReportToScreen(totalRevenue, totalBills, totalItems, customerStats, customerCount);
    exportReportToFile(totalRevenue, totalBills, totalItems, customerStats, customerCount);
}
```

**Giải thích code:**
- **findOrCreateCustomer**: Quản lý mảng thống kê khách hàng
- **calculateTotalRevenue**: Duyệt file và cộng dồn finalPrice
- **analyzeByCustomer**: Tạo thống kê cho từng khách
- **handleGenerateReport**: Điều phối toàn bộ quy trình

### 3.4 `app/ui/report_ui.h` - Header UI

Tạo file mới:

```c
#ifndef REPORT_UI_H
#define REPORT_UI_H

#include "../models/models.h"

// ============ HÀM HIỂN THỊ ============

// In báo cáo ra màn hình console
void printReportToScreen(double totalRevenue, int totalBills, int totalItems,
                        CustomerRevenue* customerStats, int customerCount);

// Xuất báo cáo ra file văn bản
void exportReportToFile(double totalRevenue, int totalBills, int totalItems,
                       CustomerRevenue* customerStats, int customerCount);

#endif
```

### 3.5 `app/ui/report_ui.c` - Logic UI

Tạo file mới:

```c
#include "report_ui.h"
#include <stdio.h>
#include <time.h>

// ============ HÀM HELPER ============

// Lấy thời gian hiện tại định dạng dd/mm/yyyy hh:mm:ss
void getCurrentDateTime(char* buffer, int size) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, size, "%d/%m/%Y %H:%M:%S", timeinfo);
}

// ============ HÀM HIỂN THỊ ============

// In báo cáo ra màn hình với định dạng đẹp
void printReportToScreen(double totalRevenue, int totalBills, int totalItems,
                        CustomerRevenue* customerStats, int customerCount) {
    char dateTime[50];
    getCurrentDateTime(dateTime, sizeof(dateTime));

    printf("\n");
    printf("====================================================\n");
    printf("         BAO CAO DOANH THU QUAN COM TAM\n");
    printf("====================================================\n");
    printf("Ngay tao: %s\n", dateTime);
    printf("====================================================\n\n");

    printf("TONG QUAN:\n");
    printf("  Tong so hoa don: %d\n", totalBills);
    printf("  Tong doanh thu: %.3f VND\n", totalRevenue);
    printf("  Tong so mon da ban: %d\n", totalItems);
    printf("  Doanh thu trung binh/bill: %.3f VND\n",
           totalBills > 0 ? totalRevenue / totalBills : 0.0);
    printf("\n");

    printf("PHAN TICH THEO KHACH HANG:\n");
    printf("%-12s | %-12s | %-15s\n", "Khach ID", "So HD", "Doanh thu");
    printf("--------------------------------------------\n");

    for (int i = 0; i < customerCount; i++) {
        printf("%-12d | %-12d | %15.3f\n",
               customerStats[i].customerId,
               customerStats[i].billCount,
               customerStats[i].revenue);
    }

    printf("\n");
    printf("====================================================\n");
}

// Xuất báo cáo ra file report_doanhthu.txt
void exportReportToFile(double totalRevenue, int totalBills, int totalItems,
                       CustomerRevenue* customerStats, int customerCount) {
    FILE* file = fopen("app/database/report_doanhthu.txt", "w");
    if (file == NULL) {
        printf("[!] Khong the tao file report_doanhthu.txt\n");
        return;
    }

    char dateTime[50];
    getCurrentDateTime(dateTime, sizeof(dateTime));

    // Header báo cáo
    fprintf(file, "====================================================\n");
    fprintf(file, "         BAO CAO DOANH THU QUAN COM TAM\n");
    fprintf(file, "====================================================\n");
    fprintf(file, "Ngay tao: %s\n", dateTime);
    fprintf(file, "====================================================\n\n");

    // Phần tổng quan
    fprintf(file, "TONG QUAN:\n");
    fprintf(file, "  Tong so hoa don: %d\n", totalBills);
    fprintf(file, "  Tong doanh thu: %.3f VND\n", totalRevenue);
    fprintf(file, "  Tong so mon da ban: %d\n", totalItems);
    fprintf(file, "  Doanh thu trung binh/bill: %.3f VND\n",
            totalBills > 0 ? totalRevenue / totalBills : 0.0);
    fprintf(file, "\n");

    // Phần phân tích khách hàng
    fprintf(file, "PHAN TICH THEO KHACH HANG:\n");
    fprintf(file, "%-12s | %-12s | %-15s\n", "Khach ID", "So HD", "Doanh thu");
    fprintf(file, "--------------------------------------------\n");

    for (int i = 0; i < customerCount; i++) {
        fprintf(file, "%-12d | %-12d | %15.3f\n",
                customerStats[i].customerId,
                customerStats[i].billCount,
                customerStats[i].revenue);
    }

    fprintf(file, "\n");
    fprintf(file, "====================================================\n");

    fclose(file);
    printf("[OK] Bao cao da duoc luu vao app/database/report_doanhthu.txt\n");
}
```

**Giải thích:**
- **getCurrentDateTime**: Lấy thời gian hiện tại cho báo cáo
- **printReportToScreen**: In ra console với format table đẹp
- **exportReportToFile**: Ghi ra file với cùng format

### 3.6 `main.c` - Tích hợp Menu

Sửa file `main.c`:

**Bước 1: Thêm include**
```c
#include "app/services/report_service.h"
```

**Bước 2: Thêm case 6 trong switch**
```c
case 6:
    system("cls");
    handleGenerateReport();
    system("pause");
    break;
```

## 4. Hướng dẫn Implement Từng Bước

### Bước 1: Chuẩn bị Models
1. Mở `app/models/models.h`
2. Thêm struct `CustomerRevenue` vào cuối file
3. Lưu file

### Bước 2: Tạo Service Files
1. Tạo `app/services/report_service.h` với nội dung trên
2. Tạo `app/services/report_service.c` với nội dung trên
3. Kiểm tra compile: `gcc -c app/services/report_service.c -Iapp/models`

### Bước 3: Tạo UI Files
1. Tạo `app/ui/report_ui.h` với nội dung trên
2. Tạo `app/ui/report_ui.c` với nội dung trên
3. Kiểm tra compile: `gcc -c app/ui/report_ui.c -Iapp/models`

### Bước 4: Tích hợp Main
1. Mở `main.c`
2. Thêm include `report_service.h`
3. Thêm case 6 trong switch menu
4. Compile toàn bộ project

### Bước 5: Test Chức năng
1. Chạy chương trình
2. Chọn chức năng 6
3. Kiểm tra báo cáo hiển thị trên màn hình
4. Kiểm tra file `app/database/report_doanhthu.txt` được tạo

## 5. Xử lý Lỗi Thường gặp

### Lỗi 1: File history.txt không tồn tại
- **Nguyên nhân**: Chưa có hóa đơn nào được lưu
- **Giải pháp**: Tạo file trống hoặc thêm dữ liệu test

### Lỗi 2: Không thể tạo file report_doanhthu.txt
- **Nguyên nhân**: Thư mục database không tồn tại hoặc không có quyền ghi
- **Giải pháp**: Tạo thư mục `app/database/` thủ công

### Lỗi 3: Số liệu không chính xác
- **Nguyên nhân**: Format file history.txt không đúng
- **Giải pháp**: Kiểm tra format BILL|... khớp với sscanf

## 6. Mở rộng Tương lai

### 6.1 Lọc theo Thời gian
- Thêm tham số ngày bắt đầu/kết thúc
- Parse dateTime từ BILL để lọc

### 6.2 Xuất CSV
- Thêm hàm `exportReportToCSV()`
- Format dữ liệu theo chuẩn CSV

### 6.3 Báo cáo Chi tiết Món ăn
- Thống kê món bán chạy nhất
- Phân tích theo danh mục món

### 6.4 Giao diện Đồ họa
- Chuyển từ console sang GUI (nếu cần)

## 7. Kết luận

Chức năng 6 được thiết kế với kiến trúc rõ ràng, dễ bảo trì và mở rộng. Code được tách biệt theo từng layer (models/services/ui) giúp dễ debug và phát triển thêm tính năng mới.
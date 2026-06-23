#include "report_service.h"
#include "../ui/report_ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100

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
            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount);

            // Cộng dồn doanh thu
            totalRevenue += finalPrice;
        }
    }

    fclose(file);
    return totalRevenue;
}

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

            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, nameKhach, phone, dateTime,
                   &total, &discount, &finalPrice, &itemsInBill);

            itemCount += itemsInBill;
        }
    }

    fclose(file);
    return itemCount;
}

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

            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d",
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


#include "report_controller.h"
#include "../services/report_service.h"
#include "../ui/report_ui.h"
#include <stdio.h>

#define MAX_CUSTOMERS 100

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

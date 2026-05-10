#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include "../models/models.h"

// Hàm chính xử lý báo cáo doanh thu
void handleGenerateReport();


// Các hàm phụ trợ để tính toán số liệu cho báo cáo
// Tính tổng doanh thu từ tất cả hóa đơn
double calculateTotalRevenue();

// Tính tổng số hóa đơn
int calculateTotalBills();

// Tính tổng số món đã bán
int calculateTotalItems();

// Phân tích doanh thu theo từng khách hàng
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount);

#endif
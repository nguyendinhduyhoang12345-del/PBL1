#ifndef REPORT_SERVICE_H
#define REPORT_SERVICE_H

#include "../models/models.h"

// Tính tổng doanh thu
double calculateTotalRevenue();

// Tính tổng số hóa đơn
int calculateTotalBills();

// Tính tổng số món đã bán
int calculateTotalItems();

// Phân tích doanh thu theo từng khách hàng
void analyzeByCustomer(CustomerRevenue* customerStats, int* customerCount);

#endif
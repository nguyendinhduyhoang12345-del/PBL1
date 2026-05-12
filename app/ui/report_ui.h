#ifndef REPORT_UI_H
#define REPORT_UI_H

#include "../models/models.h"

// In báo cáo ra màn hình console
void printReportToScreen(double totalRevenue, int totalBills, int totalItems, CustomerRevenue* customerStats, int customerCount);

// Xuất báo cáo ra file văn bản
void exportReportToFile(double totalRevenue, int totalBills, int totalItems, CustomerRevenue* customerStats, int customerCount);

#endif
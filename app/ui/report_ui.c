#include "report_ui.h"
#include <stdio.h>
#include <time.h>
#include "../utils/helper.h"

void printReportToScreen(double totalRevenue, int totalBills, int totalItems, CustomerRevenue* customerStats, int customerCount) {
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
void exportReportToFile(double totalRevenue, int totalBills, int totalItems, CustomerRevenue* customerStats, int customerCount) {
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
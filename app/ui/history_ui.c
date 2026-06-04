#include "history_ui.h"
#include <stdio.h>
#include <string.h>

#include "../utils/helper.h"
#include "../services/cart_service.h"

// in toàn bộ lịch sử (tóm tắt)
void printAllHistory() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        printf("[!] Chua co hoa don nao.\n");
        return;
    }
    
    printf("\n" CYAN "=================================================== TOAN BO LICH SU HOA DON ===================================================\n" RESET);
    printf(CYAN "%-8s | %-12s | %-12s | %-20s | %-10s | %-16s | %-10s | %-10s | %-6s\n" RESET,
       "Bill ID", "Customer ID", "SDT", "Thoi gian", "Rank", "Tich luy", "So tien", "Giam gia", "Items");
    printf(CYAN "===============================================================================================================================\n" RESET);
    
    double totalAmount = 0;
    int count = 0;
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemCount;
            char customerName[50], phone[15], dateTime[50], rank[20];
            double total, discount, finalPrice, totalSpent;

            sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d|%[^|]|%lf",
                   &billId, &customerId, customerName, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount,
                   rank, &totalSpent);

            printf("%s%-8d%s | %s%-12d%s | %-12s | %-20s | %s%-10s%s | " GREEN "%16.3f" RESET " | " GREEN "%10.3f" RESET " | " GREEN "%10.3f" RESET " | %-6d\n",
                   YELLOW_BOLD, billId, RESET,
                   YELLOW_BOLD, customerId, RESET,
                   phone, dateTime,
                   getRankColor(rank), rank, RESET,
                   totalSpent, finalPrice, discount, itemCount);
            totalAmount += finalPrice;
            count++;
        }
    }
    
    fclose(file);
    
    printf(CYAN "===============================================================================================================================\n" RESET);
    printf("Tong so hoa don: %d | Tong doanh thu: " GREEN "%.3f VND" RESET "\n", count, totalAmount);
    printf(CYAN "===============================================================================================================================\n" RESET);
}

// In lịch sử hóa đơn chi tiết của 1 khách hàng (từ linked list)
void printCustomerHistory(HistoryNode* customerHistory) {
    if (customerHistory == NULL) {
        printf("[!] Khach hang nay chua co lich su giao dich.\n");
        return;
    }

    int customerId = customerHistory->customerId;
    printf("\n" CYAN "================================= LICH SU HOA DON KHACH HANG ID: " YELLOW_BOLD "%d" RESET CYAN " =================================\n" RESET,
           customerId);
    
    double totalAmount = 0;
    int count = 0;
    HistoryNode* current = customerHistory;
    
    printf(CYAN "%-8s | %-12s | %-20s | %-8s | %-12s | %-10s | %-12s\n" RESET,
           "Bill ID", "Cust ID", "Ngay", "So mon", "Tong tien", "Giam gia", "Thanh toan");
    printf(CYAN "====================================================================================================\n" RESET);

    while (current != NULL) {
        int itemCount = 0;
        CartNode *node = current->bill.cart.head;
        while (node != NULL) {
            itemCount += node->item.quantity; // Tính tổng số món bằng cách cộng dồn số lượng của từng item
            node = node->next;
        }

        printf("%s%-8d%s | %s%-12d%s | %-20s | %-8d | " GREEN "%12.0f" RESET " | " GREEN "%10.0f" RESET " | " GREEN "%12.0f" RESET "\n",
               YELLOW_BOLD, current->bill.id, RESET,
               YELLOW_BOLD, current->customerId, RESET,
               current->bill.dateTime,
               itemCount,
               current->bill.total,
               current->bill.discount,
               current->bill.finalPrice);

        totalAmount += current->bill.finalPrice;
        count++;
        current = current->next;
    }
    
    if (count == 0) {
        printf("[!] Khach hang nay chua co lich su giao dich.\n");
        return;
    }

    freeHistoryList(customerHistory); // Giải phóng bộ nhớ sau khi in xong
    
    printf("\n" CYAN "====================================================================================================\n" RESET);
    printf("Tong so hoa don: %d | Tong doanh thu: " GREEN "%.3f VND" RESET "\n", count, totalAmount);
    printf(CYAN "====================================================================================================\n" RESET);
}

// In chi tiết hóa đơn theo ID bill
void printBillDetail(HistoryNode* billNode ) { // nhận vào node lịch sử đã tìm được theo ID bill
    if (billNode == NULL) { 
        printf("[!] Khong tim thay hoa don.\n");
        return;
    }
    
    printf("\n" CYAN "=================================== CHI TIET HOA DON ID: " YELLOW_BOLD "%d" RESET CYAN " ===================================\n" RESET,
           billNode->bill.id);
    printf("Khach hang ID: " YELLOW_BOLD "%d" RESET "\n", billNode->customerId);
    printf("Khach hang: %s | SDT: %s\n", billNode->bill.customer.name, billNode->bill.customer.phone);
    printf("Rank: %s%s%s | Tong tich luy: " GREEN "%.3f VND" RESET "\n",
           getRankColor(billNode->bill.customer.rank), billNode->bill.customer.rank, RESET,
           billNode->bill.customer.totalSpent);
    printf("Ngay: %s\n", billNode->bill.dateTime);
    
    printf(CYAN "%-4s | %-4s | %-25s | %-3s | %-8s | %-20s | " GREEN "%12s" RESET "\n", "STT", "ID", "Ten mon", "SL", "Option", "Ghi chu", "Thanh tien");
    printf(CYAN "====================================================================================================\n" RESET);
    
    CartNode *node = billNode->bill.cart.head;
    int i = 1;
    while (node != NULL) {
        char optLabel[16];
        if (node->item.option == 1) strcpy(optLabel, "Cot let");
        else if (node->item.option == 2) strcpy(optLabel, "Suon cay");
        else strcpy(optLabel, "Khong");

        printf("%-4d | %-4d | %-25.25s | %-3d | %-8s | %-20.20s | " GREEN "%12.3f" RESET "\n",
               i + 1,
               node->item.id,
               node->item.name,
               node->item.quantity,
               optLabel,
               node->item.note,
               node->item.totalPrice);
        node = node->next;
        i++;
    }
    
    printf(CYAN "====================================================================================================\n" RESET);
    printf("Tong tinh: " GREEN "%.3f VND" RESET " | Giam gia: " GREEN "%.3f VND" RESET " | Tong: " GREEN "%.3f VND" RESET "\n",
           billNode->bill.total, billNode->bill.discount, billNode->bill.finalPrice);
    printf(CYAN "====================================================================================================\n" RESET);
    
}


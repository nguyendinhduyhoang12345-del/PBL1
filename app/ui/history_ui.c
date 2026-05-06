#include "history_ui.h"
#include <stdio.h>
#include <string.h>

#include "../models/models.h"

// in toàn bộ lịch sử (tóm tắt)
void printAllHistory() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        printf("[!] Chua co hoa don nao.\n");
        return;
    }
    
    printf("\n============== TOAN BO LICH SU HOA DON ==============\n");
    printf("%-8s | %-12s | %-20s | %-12s | %-10s | %-6s\n", "Bill ID", "Customer ID", "Thoi gian", "So tien", "Giam gia", "Items");
    printf("==============================================================\n");
    
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
            char customerName[50], phone[15], dateTime[50];
            double total, discount, finalPrice;

            sscanf(line, "BILL|%d|%d|%[^|]|%[^|]|%[^|]|%lf|%lf|%lf|%d",
                   &billId, &customerId, customerName, phone, dateTime,
                   &total, &discount, &finalPrice, &itemCount);

            printf("%-8d | %-12d | %-20s | %12.3f | %10.3f | %-6d\n",
                   billId, customerId, dateTime, finalPrice, discount, itemCount);
            totalAmount += finalPrice;
            count++;
        }
    }
    
    fclose(file);
    
    printf("==============================================================\n");
    printf("Tong so hoa don: %d | Tong doanh thu: %.3f VND\n", count, totalAmount);
    printf("==============================================================\n");
}

// In lịch sử hóa đơn chi tiết của 1 khách hàng (từ linked list)
void printCustomerHistory(HistoryNode* customerHistory) {
    if (customerHistory == NULL) {
        printf("[!] Khach hang nay chua co lich su giao dich.\n");
        return;
    }

    int customerId = customerHistory->customerId;
    printf("\n=============== LICH SU HOA DON KHACH HANG ID: %d ===============\n", customerId);
    
    double totalAmount = 0;
    int count = 0;
    HistoryNode* current = customerHistory;
    
    while (current != NULL) {
        printf("\n--- Hoa don so: %d | Ngay: %s ---\n", current->bill.id, current->bill.dateTime);
        printf("Khach hang: %s | SDT: %s\n", current->bill.customer.name, current->bill.customer.phone);
        printf("%-4s | %-4s | %-25s | %-3s | %-8s | %-20s | %12s\n", "STT", "ID", "Ten mon", "SL", "Option", "Ghi chu", "Thanh tien");
        printf("-----------------------------------------------------------------------------------------\n");

        for (int i = 0; i < current->bill.itemCount; i++) {
            char optLabel[16];
            if (current->bill.items[i].option == 1) strcpy(optLabel, "Cot let");
            else if (current->bill.items[i].option == 2) strcpy(optLabel, "Suon cay");
            else strcpy(optLabel, "Khong");

            printf("%-4d | %-4d | %-25.25s | %-3d | %-8s | %-20.20s | %12.3f\n",
                   i + 1,
                   current->bill.items[i].id,
                   current->bill.items[i].name,
                   current->bill.items[i].quantity,
                   optLabel,
                   current->bill.items[i].note,
                   current->bill.items[i].totalPrice);
        }

        printf("--------------------------------------------------------\n");
        printf("Tong tinh: %.3f VND | Giam gia: %.3f VND | Tong: %.3f VND\n",
               current->bill.total, current->bill.discount, current->bill.finalPrice);

        totalAmount += current->bill.finalPrice;
        count++;
        current = current->next;
    }
    
    if (count == 0) {
        printf("[!] Khach hang nay chua co lich su giao dich.\n");
        return;
    }
    
    printf("\n========================================================\n");
    printf("Tong so hoa don: %d | Tong doanh thu: %.3f VND\n", count, totalAmount);
    printf("========================================================\n");
}

// In chi tiết hóa đơn theo ID bill
void printBillDetail(HistoryNode* billNode ) { // nhận vào node lịch sử đã tìm được theo ID bill
    if (billNode == NULL) { 
        printf("[!] Khong tim thay hoa don.\n");
        return;
    }
    
    printf("\n=============== CHI TIET HOA DON ID: %d ===============\n", billNode->bill.id);
    printf("Khach hang ID: %d\n", billNode->customerId);
    printf("Khach hang: %s | SDT: %s\n", billNode->bill.customer.name, billNode->bill.customer.phone);
    printf("Ngay: %s\n", billNode->bill.dateTime);
    
    printf("%-4s | %-4s | %-25s | %-3s | %-8s | %-20s | %12s\n", "STT", "ID", "Ten mon", "SL", "Option", "Ghi chu", "Thanh tien");
    printf("-----------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < billNode->bill.itemCount; i++) {
        char optLabel[16];
        if (billNode->bill.items[i].option == 1) strcpy(optLabel, "Cot let");
        else if (billNode->bill.items[i].option == 2) strcpy(optLabel, "Suon cay");
        else strcpy(optLabel, "Khong");

        printf("%-4d | %-4d | %-25.25s | %-3d | %-8s | %-20.20s | %12.3f\n",
               i + 1,
               billNode->bill.items[i].id,
               billNode->bill.items[i].name,
               billNode->bill.items[i].quantity,
               optLabel,
               billNode->bill.items[i].note,
               billNode->bill.items[i].totalPrice);
    }
    
    printf("-----------------------------------------------------------------------------------------\n");
    printf("Tong tinh: %.3f VND | Giam gia: %.3f VND | Tong: %.3f VND\n",
           billNode->bill.total, billNode->bill.discount, billNode->bill.finalPrice);
}


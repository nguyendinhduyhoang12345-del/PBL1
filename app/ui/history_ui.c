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
    printf("%-8s | %-12s | %-20s | %-12s | %-10s\n", "Bill ID", "Customer ID", "Thoi gian", "So tien", "Giam gia");
    printf("====================================================\n");
    
    double totalAmount = 0;
    int count = 0;
    char line[512];

    while (fgets(line, sizeof(line), file)) {
        int billId, customerId;
        double finalPrice, discount;
        char dateTime[50];
        
        sscanf(line, "%d|%d|%[^|]|%lf|%lf", &billId, &customerId, dateTime, &finalPrice, &discount);
        
        printf("%-8d | %-12d | %-20s | %12.3f | %10.3f\n",
               billId, customerId, dateTime, finalPrice, discount);
        totalAmount += finalPrice;
        count++;
    }
    
    fclose(file);
    
    printf("====================================================\n");
    printf("Tong so hoa don: %d | Tong doanh thu: %.3f VND\n", count, totalAmount);
    printf("====================================================\n");
}

// In lịch sử hóa đơn chi tiết của 1 khách hàng (từ file khách hàng)
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
        printf("Tong: %.3f VND | Giam gia: %.3f VND | Tong cuoi: %.3f VND\n",
               current->bill.finalPrice + current->bill.discount, current->bill.discount, current->bill.finalPrice);
        
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
    printf("Ngay: %s\n", billNode->bill.dateTime);
    
    printf("%-4s | %-30s | %-3s | %12s\n", "STT", "Ten mon", "SL", "Thanh tien");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < billNode->bill.itemCount; i++) {
        printf("%-4d | %-30.30s | %-3d | %12.3f\n",
               i + 1,
               billNode->bill.items[i].name,
               billNode->bill.items[i].quantity,
               billNode->bill.items[i].totalPrice);
    }
    
    printf("--------------------------------------------------\n");
    printf("Tong tinh: %.3f VND | Giam gia: %.3f VND | Tong: %.3f VND\n",
           billNode->bill.total, billNode->bill.discount, billNode->bill.finalPrice);
}


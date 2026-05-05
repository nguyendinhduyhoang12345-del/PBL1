#include "history_controller.h"
#include <stdio.h>
#include <stdlib.h>

#include "../services/history_service.h"
#include "../ui/history_ui.h"
#include "../utils/validator.h"

void handleViewHistory() {
    
    // Mặc định hiển thị lịch sử chung của quán
    printAllHistory();
    
    // Submenu lựa chọn
    printf("\n=================== LUA CHON ====================\n");
    printf("1. Xem lich su cua khach hang theo so dien thoai\n");
    printf("2. Tim hoa don theo ID bill\n");
    printf("0. Quay lai\n");
    
    int choice = getValidInt("-> Chon: ");
    
    if (choice == 1) {
        char phone[15];
        printf(">> Nhap so dien thoai khach hang: ");
        scanf("%14s", phone);

        int customerId = getCustomerIdByPhone(phone);
        if (customerId != -1) {
            HistoryNode* currentHistory = getCustomerHistory(customerId);
            printCustomerHistory(currentHistory);
        } else {
            printf("[!] Khong tim thay khach hang voi so dien thoai nay.\n");
        }
    } 
    else if (choice == 2) {
        int billId = getValidInt(">> Nhap ID bill: ");
        HistoryNode* billNode = searchBillById(billId);
        printBillDetail(billNode);
    }
    else if (choice != 0) {
        printf("[!] Lua chon khong hop le.\n");
    }
}
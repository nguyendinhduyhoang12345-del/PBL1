#include "history_controller.h"
#include <stdio.h>
#include <stdlib.h>

#include "../services/history_service.h"
#include "../ui/history_ui.h"
#include "../utils/validator.h"
#include "../utils/helper.h"

void handleViewHistory() {
    
    // Mặc định hiển thị lịch sử chung của quán
    printAllHistory();
    
    while (1) {
        // Submenu lựa chọn
        printf("\n" CYAN "=================== LUA CHON ====================\n" RESET);
        printf("1. Xem lich su cua khach hang theo so dien thoai\n");
        printf("2. Tim hoa don theo ID bill\n");
        printf("0. Quay lai\n");
        
        int choice = getValidInt("-> Chon: ");
        
        if (choice == 1) {
            char phone[15];
            getValidPhoneNumber(phone, sizeof(phone), ">> Nhap so dien thoai khach hang: ");

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
        else if (choice == 0) {
            break;
        }
        else {
            printf("[!] Lua chon khong hop le.\n");
        }
    }
}
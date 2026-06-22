#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer_controller.h"
#include "../services/btree_service.h"
#include "../utils/helper.h"
#include "../utils/validator.h"
#include "../services/file_service.h"
#include "../ui/customer_ui.h"

void handleCustomer(Bill *currentBill) {
    char sdt[15];
    printf("\n" CYAN "=====================" RESET" "YELLOW_BOLD"QUAN LY THONG TIN KHACH HANG" RESET" "CYAN"======================\n" RESET);
    
    getValidPhoneNumber(sdt, sizeof(sdt), ">> Nhap So Dien Thoai khach hang: ");
    
    // Tìm kiếm trong B-Tree 
    Customer* found = searchBTree(btreeRoot, sdt);

    if (found != NULL) {
        showCustomerInfo(found);
        currentBill->customer = *found; // Gán vào hóa đơn hiện tại

    } else {
        printf(CYAN "=====================" RESET" "YELLOW_BOLD" KHACH HANG CHUA CO TRONG HE THONG" RESET" "CYAN"======================\n" RESET);
        printf("1. Dang ky moi | 0. Bo qua (Guest)\n");
        int choice = getValidInt("-> Chon: ");
        
        if (choice == 1) {
            Customer newCust;
            newCust.id = soLuongKH + 1; 
            strcpy(newCust.phone, sdt);
        
            getValidLine(newCust.name, sizeof(newCust.name), " >> Nhap Ten: ");
            
            getValidLine(newCust.address, sizeof(newCust.address), " >> Nhap Dia chi: ");
            
            newCust.totalSpent = 0.000;
            strcpy(newCust.rank, "Bronze"); 

            danhSachKH[soLuongKH] = newCust;
            soLuongKH++;
            insertBTree(&btreeRoot, &danhSachKH[soLuongKH]);
            currentBill->customer = danhSachKH[soLuongKH];
            
            saveAllCustomersToFile();

            showCustomerRegistration(&danhSachKH[soLuongKH - 1]);
        }
    }
}
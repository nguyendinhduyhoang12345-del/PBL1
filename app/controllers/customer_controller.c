#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer_controller.h"
#include "../services/btree_service.h"
#include "../utils/helper.h"
#include "../utils/validator.h"
#include "../services/file_service.h"

void handleCustomer(Bill *currentBill) {
    char sdt[15];
    printf("\n--- QUAN LY KHACH HANG ---\n");
    getValidPhoneNumber(sdt, sizeof(sdt), ">> Nhap So Dien Thoai khach hang: ");
    
    // Tìm kiếm trong B-Tree 
    Customer* found = searchBTree(btreeRoot, sdt);

    if (found != NULL) {
        const char *rankColor = getRankColor(found->rank);
        printf(" => Tim thay: [%s%s%s] | Hang: %s%s%s | Tich luy: %s%.3f VND%s\n",
               YELLOW_BOLD, found->name, RESET,
               rankColor, found->rank, RESET,
               GREEN, found->totalSpent, RESET);
        currentBill->customer = *found; // Gán vào hóa đơn hiện tại

    } else {
        printf(" !! Dien thoai chua co trong he thong. Dang ky moi?\n");
        printf(" 1. Co | 0. Bo qua (Guest)\n");
        int choice = getValidInt("-> Chon: ");
        
        if (choice == 1) {
            Customer newCust; // Dùng biến thường thay vì malloc để nhàn hơn trong việc gán vào mảng
            
            // --- CẬP NHẬT QUAN TRỌNG: GÁN ID TỰ ĐỘNG ---
            newCust.id = soLuongKH + 1; 
            strcpy(newCust.phone, sdt);
            
            getValidLine(newCust.name, sizeof(newCust.name), " >> Nhap Ten: ");
            
            getValidLine(newCust.address, sizeof(newCust.address), " >> Nhap Dia chi: ");
            
            newCust.totalSpent = 0.000; // Định dạng 3 số lẻ
            strcpy(newCust.rank, "Bronze"); 

            // Lưu vào mảng toàn cục
            danhSachKH[soLuongKH] = newCust;
            soLuongKH++;
            insertBTree(&btreeRoot, &danhSachKH[soLuongKH]);
            currentBill->customer = danhSachKH[soLuongKH];
            
            // GỌI FILE SERVICE ĐỂ LƯU XUỐNG FILE
            saveAllCustomersToFile();

            printf(" => Dang ky thanh cong! ID cua quy khach la: %d\n", newCust.id);
            printf("    Ten khach hang: %s%s%s\n", YELLOW_BOLD, newCust.name, RESET);
            printf("    So tien tich luy: %s%.3f VND%s\n", GREEN, newCust.totalSpent, RESET);
            printf("    Hang: %s%s%s\n", getRankColor(newCust.rank), newCust.rank, RESET);
        }
    }
}
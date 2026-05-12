#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer_controller.h"
#include "../services/btree_service.h"
#include "../utils/validator.h"
#include "../services/file_service.h"

void handleCustomer(Bill *currentBill) {
    char sdt[15];
    printf("\n--- QUAN LY KHACH HANG ---\n");
    getValidPhoneNumber(sdt, sizeof(sdt), ">> Nhap So Dien Thoai khach hang: ");
    
    // Tìm kiếm trong B-Tree 
    Customer* found = searchBTree(btreeRoot, sdt);

    if (found != NULL) {
        // Áp dụng định dạng %.3f
        printf(" => Tim thay: [%s] | Hang: %s | Tich luy: %.3f VND\n", found->name, found->rank, found->totalSpent);
        currentBill->customer = *found; // Gán vào hóa đơn hiện tại

    } else {
        printf(" !! Dien thoai chua co trong he thong. Dang ky moi?\n");
        printf(" 1. Co | 0. Bo qua (Khach vang lai)\n");
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
        }
    }
}
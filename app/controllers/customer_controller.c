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
    printf(">> Nhap So Dien Thoai khach hang: ");
    scanf("%14s", sdt);
    clearBuffer();

    // Trim khoảng trắng ở đầu và cuối
    int len = strlen(sdt);
    while (len > 0 && (sdt[len-1] == ' ' || sdt[len-1] == '\t')) {
        sdt[--len] = 0;
    }
    int i = 0;
    while (sdt[i] == ' ' || sdt[i] == '\t') i++;
    if (i > 0) memmove(sdt, sdt + i, strlen(sdt + i) + 1);

    // 1. ÁP DỤNG FLOWCHART: Bắt lỗi phải đúng 10 số
    while (strlen(sdt) != 10) {
        printf("[!] SDT phai co dung 10 so. Vui long nhap lai: ");
        scanf("%s", sdt);
        clearBuffer();
        
        // Trim lại
        len = strlen(sdt);
        while (len > 0 && (sdt[len-1] == ' ' || sdt[len-1] == '\t')) {
            sdt[--len] = 0;
        }
        i = 0;
        while (sdt[i] == ' ' || sdt[i] == '\t') i++;
        if (i > 0) memmove(sdt, sdt + i, strlen(sdt + i) + 1);
    }

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
            
            printf(" >> Nhap Ten: "); 
            fgets(newCust.name, 50, stdin); 
            newCust.name[strcspn(newCust.name, "\n")] = 0;
            
            printf(" >> Nhap Dia chi: "); 
            fgets(newCust.address, 100, stdin); 
            newCust.address[strcspn(newCust.address, "\n")] = 0;
            
            newCust.totalSpent = 0.000; // Định dạng 3 số lẻ
            strcpy(newCust.rank, "Bronze"); 

            // Lưu vào mảng toàn cục
            danhSachKH[soLuongKH] = newCust;
            
            // Nạp vào B-Tree (truyền địa chỉ phần tử trong mảng để trỏ đúng vùng nhớ)
            insertBTree(&btreeRoot, &danhSachKH[soLuongKH]);
            
            // Gán vào hóa đơn hiện tại
            currentBill->customer = danhSachKH[soLuongKH];
            
            // Tăng số lượng khách hàng sau khi đã gán xong xuôi
            soLuongKH++;

            // GỌI FILE SERVICE ĐỂ LƯU XUỐNG FILE
            saveAllCustomersToFile();

            printf(" => Dang ky thanh cong! ID cua quy khach la: %d\n", newCust.id);
        }
    }
}
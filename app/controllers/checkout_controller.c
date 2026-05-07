#include "checkout_controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../services/checkout_service.h"
#include "../services/file_service.h" 
#include "../services/btree_service.h" 
#include "../services/history_service.h"
#include "../ui/bill_ui.h"
#include "../utils/helper.h"
#include "../utils/validator.h"

void processCheckout(Bill* currentBill) {
    printf("\n==================== THANH TOAN ====================\n");
    
    if (currentBill->itemCount == 0) {
        printf("\n[!] Gio hang cua ban dang trong! Vui long chon mon truoc khi thanh toan.\n");
        system("pause");
        return; 
    }

    // --- BƯỚC 1: GỌI SERVICE TÍNH TIỀN ---
    double subtotal = calculateSubtotal(currentBill);
    double discountRate = 0;
    Customer* khGoc = NULL;

    // Tìm khách hàng gốc trên B-Tree
    if (strlen(currentBill->customer.phone) > 0) {
        khGoc = searchBTree(btreeRoot, currentBill->customer.phone);
        if (khGoc != NULL) {
            discountRate = getDiscountPercent(khGoc->rank);
        }
    }

    double discountAmount = subtotal * discountRate;
    double finalPrice = subtotal - discountAmount;

    // sau khi tinh xong, cập nhật lại tổng tiền vào hóa đơn để lưu trữ
    currentBill->total = subtotal; 
    currentBill->discount = discountAmount;
    currentBill->finalPrice = finalPrice;
    getCurrentDateTime(currentBill->dateTime, sizeof(currentBill->dateTime));
    currentBill->id = rand() % 900000 + 100000; // Gán ID ngẫu nhiên cho hóa đơn (100000-999999)



    // --- BƯỚC 2: TƯƠNG TÁC NHẬP LIỆU VỚI THU NGÂN ---
    printf(">> TONG TIEN BAN DAU   : %15.3f VND\n", subtotal);
    
    if (khGoc != NULL) {
        printf(">> RANK KHACH HANG     : %-10s (Giam %.0f%%)\n", khGoc->rank, discountRate * 100);
        printf(">> SO TIEN DUOC GIAM   : -%14.3f VND\n", discountAmount);
    } else {
        printf(">> RANK KHACH HANG     : Khach vang lai (Khong giam gia)\n");
    }
    
    printf("----------------------------------------------------\n");
    printf(">> TONG TIEN CAN TRA   : %15.3f VND\n", finalPrice);
    printf("----------------------------------------------------\n");

    // Tương tác với thu ngân
    double customerMoney = 0;
    customerMoney = getValidDouble("-> Nhap so tien khach dua : ");

    while (customerMoney < finalPrice) {
        printf("[!] Khach dua thieu tien! Con thieu %.3f VND. Nhap lai: ", finalPrice - customerMoney);
        customerMoney = getValidDouble("-> Nhap so tien khach dua : ");
    }

    // --- BƯỚC 3: GỌI UI VẼ HÓA ĐƠN ---
    printBillUI(currentBill, customerMoney, customerMoney - finalPrice);

    // --- BƯỚC 4: XÁC NHẬN VÀ LƯU DATABASE ---
    int xacNhan;
    xacNhan = getValidInt("Xac nhan thanh toan (1: Dong y / 0: Huy bo): ");
    if (xacNhan == 1) {
        if (khGoc != NULL) {
            // 1. Lưu lại thông tin cũ để hiển thị
            double oldTotal = khGoc->totalSpent;
            char oldRank[20];
            strcpy(oldRank, khGoc->rank);

            // 2. Cập nhật dữ liệu khách hàng
            khGoc->totalSpent += finalPrice;
            updateCustomerRank(khGoc); // Cập nhật hạng dựa trên mốc 1tr - 5tr - 10tr

            // 3. Gán dữ liệu customer vào bill để lưu lịch sử đầy đủ
            currentBill->customer.id = khGoc->id;
            strcpy(currentBill->customer.name, khGoc->name);
            strcpy(currentBill->customer.phone, khGoc->phone);
            strcpy(currentBill->customer.rank, khGoc->rank);
            currentBill->customer.totalSpent = khGoc->totalSpent;

            // 4. Ghi lại dữ liệu khách hàng vào file khachhang.txt
            saveAllCustomersToFile(); 

            // 5. Hiển thị thông tin cập nhật
            printf("\n------------------------------------------------------------\n");
            printf("[ CAP NHAT KHACH HANG ]\n");
            printf(">> Tong chi tieu cu : %15.3f VND\n", oldTotal);
            printf(">> Tong chi tieu moi: %15.3f VND\n", khGoc->totalSpent);
            printf(">> Hang hien tai    : %s\n", khGoc->rank);
            if (strcmp(oldRank, khGoc->rank) != 0) {
                printf("\n[*] CHUC MUNG! Quy khach da duoc nang hang tu %s len %s!\n", oldRank, khGoc->rank);
            }
        } 
        else {
            // Khách vãng lai: vẫn lưu lịch sử chung với giá trị cơ bản
            currentBill->customer.id = 0;
            strcpy(currentBill->customer.rank, "Khach vang lai");
            currentBill->customer.totalSpent = finalPrice;
        }

        // 6. Lưu lịch sử hóa đơn chung của quán luôn
        saveBillToGlobalFile(currentBill, currentBill->customer.id);

        // 7. Nếu khách có ID hợp lệ thì lưu cả file lịch sử riêng của khách
        if (khGoc != NULL && strlen(currentBill->customer.phone) > 0) {
            addBillToHistory(currentBill->customer.id, currentBill);
            saveBillToHistoryFile(currentBill->customer.id, currentBill);
        }

        printf("\n[OK] Thanh toan thanh cong! Cam on quy khach.\n");
        
        // Dọn dẹp giỏ hàng để phục vụ người tiếp theo
        currentBill->itemCount = 0; 
        memset(currentBill->customer.phone, 0, sizeof(currentBill->customer.phone)); 
        memset(currentBill->customer.name, 0, sizeof(currentBill->customer.name));
    } else {
        printf("\n[!] Da huy thanh toan.\n");
    }
    
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app/models/models.h"

#include "app/utils/helper.h"
#include "app/utils/validator.h"

#include "app/ui/menu_ui.h"
#include "app/ui/cart_ui.h"
#include "app/ui/welcome_ui.h"

#include "app/controllers/order_controller.h"
#include "app/controllers/customer_controller.h"
#include "app/controllers/checkout_controller.h"
#include "app/controllers/history_controller.h"

#include "app/services/file_service.h"
#include "app/services/history_service.h"
#include "app/services/report_service.h"
#include "app/services/hash_service.h"
#include "app/services/cart_service.h"


// ĐỊNH NGHĨA BIẾN TOÀN CỤC (Phải có ở main.c để link dữ liệu)
MenuItem menu[TONG_SO_MON];
Customer danhSachKH[1000];
int soLuongKH = 0;
BTreeNode* btreeRoot = NULL;
HistoryNode* historyHead = NULL;
HashTable* menuHashTable = NULL;

int main() {
    
    srand(time(NULL));
    loadMenuFromFile("app/database/menu.txt");
    loadCustomersFromFile(&btreeRoot);
    loadHistoryFromFile();
    
    printWelcomeScreen();
    system("pause");
    Bill currentBill = {0}; // Khởi tạo giỏ hàng rỗng
    cartInit(&currentBill.cart); // Khởi tạo giỏ hàng trong hóa đơn
    int choice;

    while(1) {
        system("cls"); // Hoặc "clear" trên Mac/Linux
        printf(CYAN "====================================================" RESET "\n");
        printf(CYAN "          " RESET WHITE "HE THONG QUAN LY QUAN COM TAM PBL1" RESET CYAN "\n");
        printf(CYAN "====================================================" RESET "\n");
        printf(WHITE "  1. Dat mon" RESET "\n");
        printf(WHITE "  2. Xem va chinh sua gio hang" RESET "\n");
        printf(WHITE "  3. Nhap thong tin KH" RESET "\n");
        printf(WHITE "  4. Thanh toan" RESET "\n");
        printf(WHITE "  5. Xem lich su" RESET "\n");
        printf(WHITE "  6. Thong ke doanh thu va xuat file bao cao" RESET "\n");
        printf(WHITE "  0. Thoat" RESET "\n");
        printf(CYAN "====================================================" RESET "\n");
        
        choice = getValidInt("-> Chon chuc nang: ");
        if (choice < 0 || choice > 6) {
            printf("[!] Lua chon khong hop le.\n");
            system("pause");
            continue;
        }
        switch(choice) {
            case 1:
                system("cls");
                handleOrderMenu(&currentBill); // Xử lý đặt món
                system("pause");
                break;
            case 2:
                system("cls");
                handleCartMenu(&currentBill);  // Xử lý giỏ hàng
                system("pause");
                break;
            case 3:
                system("cls");
                handleCustomer(&currentBill);
                system("pause");
                break;
            case 4:
                system("cls");
                processCheckout(&currentBill);
                system("pause");
                break;
            case 5:
                system("cls");
                handleViewHistory();
                system("pause");
                break;
            case 6:
                system("cls");
                handleGenerateReport();
                system("pause");
                break;
            case 0:
                return 0;
            default:
                printf("Chuc nang dang phat trien!\n");
                system("pause");
        }
    }
    return 0;
}
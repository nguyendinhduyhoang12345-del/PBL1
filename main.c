#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app/models/models.h"
#include "app/utils/helper.h"
#include "app/utils/validator.h"
#include "app/services/file_service.h"
#include "app/ui/menu_ui.h"
#include "app/ui/cart_ui.h"
#include "app/controllers/order_controller.h"
#include "app/controllers/customer_controller.h"
#include "app/controllers/checkout_controller.h"
#include "app/controllers/history_controller.h"
#include "app/services/history_service.h"


// ĐỊNH NGHĨA BIẾN TOÀN CỤC (Phải có ở main.c để link dữ liệu)
MenuItem menu[TONG_SO_MON];
Customer danhSachKH[1000];
int soLuongKH = 0;
BTreeNode* btreeRoot = NULL;
HistoryNode* historyHead = NULL;

int main() {
    
    srand(time(NULL));
    loadMenuFromFile("app/database/menu.txt");
    loadCustomersFromFile(&btreeRoot);
    loadHistoryFromFile();
    system("pause");
    Bill currentBill = {0}; // Khởi tạo giỏ hàng rỗng
    int choice;

    while(1) {
        system("cls"); // Hoặc "clear" trên Mac/Linux
        printf("====================================================\n");
        printf("          HE THONG QUAN LY QUAN COM TAM PBL1\n");
        printf("====================================================\n");
        printf("  1. Dat mon\n");
        printf("  2. Xem va chinh sua gio hang\n");
        printf("  3. Nhap thong tin KH\n");
        printf("  4. Thanh toan\n");
        printf("  5. Xem lich su\n");
        printf("  6. Thong ke doanh thu\n");
        printf("  0. Thoat\n");
        printf("====================================================\n");
        
        choice = getValidInt("-> Chon chuc nang: ");
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
            case 0:
                return 0;
            default:
                printf("Chuc nang dang phat trien!\n");
                system("pause");
        }
    }
    return 0;
}
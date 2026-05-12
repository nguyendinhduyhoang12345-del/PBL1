#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "app/models/models.h"

#include "app/utils/helper.h"
#include "app/utils/validator.h"

#include "app/ui/menu_ui.h"
#include "app/ui/cart_ui.h"

#include "app/controllers/order_controller.h"
#include "app/controllers/customer_controller.h"
#include "app/controllers/checkout_controller.h"
#include "app/controllers/history_controller.h"

#include "app/services/file_service.h"
#include "app/services/history_service.h"
#include "app/services/report_service.h"
<<<<<<< HEAD
#include "app/services/hash_service.h"
#include "app/services/cart_service.h"
=======
#include "app/services/hash_table_service.h"
#include "app/services/doubly_linked_list_service.h"
#include "app/services/menu_hash_service.h"
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247


// ĐỊNH NGHĨA BIẾN TOÀN CỤC (Phải có ở main.c để link dữ liệu)
MenuItem menu[TONG_SO_MON];
Customer danhSachKH[1000];
int soLuongKH = 0;
BTreeNode* btreeRoot = NULL;
HistoryNode* historyHead = NULL;
<<<<<<< HEAD
HashTable* menuHashTable = NULL;
=======
HashTable* menuHash = NULL;
HashTable hashTable = {0};
DoublyLinkedList doublyLinkedList = {0};
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

int main() {
    
    srand(time(NULL));
    loadMenuFromFile("app/database/menu.txt");
<<<<<<< HEAD
    loadCustomersFromFile(&btreeRoot);
    loadHistoryFromFile();
    system("pause");
    Bill currentBill = {0}; // Khởi tạo giỏ hàng rỗng
    cartInit(&currentBill.cart); // Khởi tạo giỏ hàng trong hóa đơn
=======
    menuHash = loadMenuToHashTable("app/database/menu.txt");
    loadCustomersFromFile(&btreeRoot);
    loadHistoryFromFile();
    system("pause");
    Bill currentBill = {0};
    currentBill.cart = dllCreate();
    currentBill.itemCount = 0;
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
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
        printf("  6. Thong ke doanh thu va xuat file bao cao\n");
        printf("  0. Thoat\n");
        printf("====================================================\n");
        
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
<<<<<<< HEAD
=======
    dllFree(currentBill.cart);
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
    return 0;
}
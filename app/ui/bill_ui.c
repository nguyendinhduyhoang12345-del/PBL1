#include "bill_ui.h"
#include <stdio.h>
#include <string.h>

#include "../utils/helper.h"
<<<<<<< HEAD
#include "../services/cart_service.h"
=======
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

void printBillUI(Bill* bill, double customerMoney, double change) {
    
    printf("\n======================== HOA DON THANH TOAN ========================\n");
    printf("ID Hoa don: %d\n", bill->id);
    printf("Ngay gio  : %s\n", bill->dateTime); 
    printf("Khach hang: %s\n", strlen(bill->customer.name) > 0 ? bill->customer.name : "Khach vang lai");
    printf("--------------------------------------------------------------------\n");
    printf("%-4s | %-40s | %-5s | %15s\n", "STT", "Ten mon", "SL", "Thanh tien");
    printf("--------------------------------------------------------------------\n");
    
<<<<<<< HEAD
    int i = 1;
    CartNode *node = bill->cart.head;
    while (node) {
        double thanhTienMon = node->item.price * node->item.quantity;
        printf("%-4d | %-40.40s | %-5d | %15.3f\n",
                i, node->item.name, node->item.quantity, thanhTienMon);
        node = node->next;
        i++;
=======
    for(int i = 0; i < bill->itemCount; i++) {
        double thanhTienMon = bill->items[i].price * bill->items[i].quantity;
        
        printf("%-4d | %-40.40s | %-5d | %15.3f\n", 
               i + 1, 
               bill->items[i].name, 
               bill->items[i].quantity, 
               thanhTienMon);
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
    }
    
    printf("--------------------------------------------------------------------\n");
    printf("%-51s %15.3f\n", "Tam tinh:", bill->total);
    printf("%-51s %15.3f\n", "Giam gia:", bill->discount);
    printf("%-51s %15.3f\n", "TONG CONG CAN TRA:", bill->finalPrice);
    printf("%-51s %15.3f\n", "Khach dua:", customerMoney);
    printf("%-51s %15.3f\n", "Tien thoi:", change);
    printf("====================================================================\n");
}
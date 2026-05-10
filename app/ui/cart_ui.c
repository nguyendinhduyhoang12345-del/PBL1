#include <stdio.h>
#include <string.h>
#include "cart_ui.h"
#include "../utils/helper.h"
#include "../services/hash_service.h"
#include "../services/cart_service.h"

void showCartUI(Bill *currentBill) {
    printf("\n============================= GIO HANG CUA BAN ==============================\n");
    if (currentBill->cart.itemCount == 0) {
        printf("          ( Gio hang hien dang trong )\n");
        printf("==========================================================\n");
        return;
    }

    double tamTinh = 0;
    printf("%-3s | %-28s | %-4s | %10s | %12s\n", "STT", "Ten mon", "SL", "Don gia", "Thanh tien");
    printf("=============================================================================\n");

    CartNode *node = currentBill->cart.head;
    int i = 1;
    while (node) {
        OrderItem item = node->item;
        printf("%-3d | %-25.25s | %-3d | %12.0lf | %15.0lf\n",
               i, item.name, item.quantity, item.price, item.totalPrice);
        if (item.option > 0) {
            printf("    | [Loai: %s]\n", item.option == 2 ? "Suon cay" : "Cot let");
        }
        if (strlen(item.note) > 0) {
            printf("    | [Ghi chu: %s]\n", item.note);
        }
        tamTinh += item.totalPrice;
        node = node->next;
        i++;
    }

    printf("=============================================================================\n");
    printf(" >> TONG TAM TINH: %41.0lf VND\n", tamTinh);
    printf("=============================================================================\n");
}
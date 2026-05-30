#include <stdio.h>
#include <string.h>
#include "cart_ui.h"
#include "../utils/helper.h"
#include "../services/hash_service.h"
#include "../services/cart_service.h"

#define CART_LINE_WIDTH 77
#define CART_NAME_WIDTH 30
#define CART_QTY_WIDTH 5
#define CART_PRICE_WIDTH 12
#define CART_TOTAL_WIDTH 15

static void printCyanLine() {
    printf(CYAN);
    for (int i = 0; i < CART_LINE_WIDTH; i++) putchar('=');
    printf(RESET "\n");
}

void showCartUI(Bill *currentBill) {
    printf("\n");
    printCyanLine();
    const char *title = " GIO HANG CUA BAN ";
    int titleWidth = (int)strlen(title);
    int leftPadding = (CART_LINE_WIDTH - titleWidth) / 2;
    int rightPadding = CART_LINE_WIDTH - titleWidth - leftPadding;
    printf(CYAN "%*s" RESET WHITE "%s" RESET CYAN "%*s\n" RESET,
           leftPadding, "", title, rightPadding, "");
    if (currentBill->cart.itemCount == 0) {
        printf(CYAN "          " RESET WHITE "( Gio hang hien dang trong )" RESET "\n");
        printCyanLine();
        return;
    }

    double tamTinh = 0;
    printf(CYAN "%-3s" RESET " | " WHITE "%-*s" RESET " | " WHITE "%*s" RESET " | " WHITE "%*s" RESET " | " WHITE "%*s" RESET "\n",
           "STT", CART_NAME_WIDTH, "Ten mon", CART_QTY_WIDTH, "SL", CART_PRICE_WIDTH, "Don gia", CART_TOTAL_WIDTH, "Thanh tien");
    printCyanLine();

    CartNode *node = currentBill->cart.head;
    int i = 1;
    while (node) {
        OrderItem item = node->item;
        printf(CYAN "%-3d" RESET " | " WHITE "%-*.*s" RESET " | " WHITE "%*d" RESET " | " GREEN "%*.*f" RESET " | " GREEN "%*.*f" RESET "\n",
               i,
               CART_NAME_WIDTH, CART_NAME_WIDTH, item.name,
               CART_QTY_WIDTH, item.quantity,
               CART_PRICE_WIDTH, 0, item.price,
               CART_TOTAL_WIDTH, 0, item.totalPrice);
        if (item.option > 0) {
            printf(CYAN "    | " RESET YELLOW_BOLD "[Loai: %s]" RESET "\n", item.option == 2 ? "Suon cay" : "Cot let");
        }
        if (strlen(item.note) > 0) {
            printf(CYAN "    | " RESET YELLOW_BOLD "[Ghi chu: %s]" RESET "\n", item.note);
        }
        tamTinh += item.totalPrice;
        node = node->next;
        i++;
    }

    printCyanLine();
    printf(CYAN " >> " RESET WHITE "TONG TAM TINH:" RESET " " GREEN "%*.*f VND" RESET "\n", CART_TOTAL_WIDTH + 2, 0, tamTinh);
    printCyanLine();
}
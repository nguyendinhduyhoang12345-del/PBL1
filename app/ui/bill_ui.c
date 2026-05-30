#include "bill_ui.h"
#include <stdio.h>
#include <string.h>

#include "../utils/helper.h"
#include "../services/cart_service.h"

#define BILL_STT_WIDTH   4
#define BILL_NAME_WIDTH 40
#define BILL_QTY_WIDTH   5
#define BILL_TOTAL_WIDTH 15
#define BILL_LINE_WIDTH 73

void printBillUI(Bill* bill, double customerMoney, double change) {
    
    printf("\n" CYAN "=========================== HOA DON THANH TOAN ==========================\n" RESET);
    printf("ID Hoa don: %d\n", bill->id);
    printf("Ngay gio  : %s\n", bill->dateTime);
    printf("Khach hang: " YELLOW_BOLD "%s" RESET "\n",
           strlen(bill->customer.name) > 0 ? bill->customer.name : "Khach");
    if (strlen(bill->customer.rank) > 0) {
        const char *rankColor = getRankColor(bill->customer.rank);
        printf("Hang      : %s%s%s\n", rankColor, bill->customer.rank, RESET);
    }
    printf(CYAN);
    for (int i = 0; i < BILL_LINE_WIDTH; i++) putchar('-');
    printf(RESET "\n");
    printf(WHITE "%-*s | %-*s | %-*s | " GREEN "%*s" RESET "\n",
           BILL_STT_WIDTH, "STT",
           BILL_NAME_WIDTH, "Ten mon",
           BILL_QTY_WIDTH, "SL",
           BILL_TOTAL_WIDTH, "Thanh tien");
    printf(CYAN);
    for (int i = 0; i < BILL_LINE_WIDTH; i++) putchar('-');
    printf(RESET "\n");
    
    int i = 1;
    CartNode *node = bill->cart.head;
    while (node) {
        double thanhTienMon = node->item.price * node->item.quantity;
        printf(WHITE "%-*d | %-*.*s | %-*d | " GREEN "%*.*f" RESET "\n",
               BILL_STT_WIDTH, i,
               BILL_NAME_WIDTH, BILL_NAME_WIDTH, node->item.name,
               BILL_QTY_WIDTH, node->item.quantity,
               BILL_TOTAL_WIDTH, 3, thanhTienMon);
        node = node->next;
        i++;
    }
    
    printf(CYAN "-------------------------------------------------------------------------\n" RESET);
    printf(WHITE "%-57s " GREEN "%15.3f" RESET "\n", "Tam tinh:", bill->total);
    printf(WHITE "%-57s " GREEN "%15.3f" RESET "\n", "Giam gia:", bill->discount);
    printf(WHITE "%-57s " GREEN "%15.3f" RESET "\n", "TONG CONG CAN TRA:", bill->finalPrice);
    printf(WHITE "%-57s " GREEN "%15.3f" RESET "\n", "Khach dua:", customerMoney);
    printf(WHITE "%-57s " GREEN "%15.3f" RESET "\n", "Tien thoi:", change);
    printf(CYAN "=========================================================================\n" RESET);
}
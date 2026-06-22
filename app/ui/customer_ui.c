#include "customer_ui.h"
#include <stdio.h>
#include <string.h>
#include "../utils/helper.h"

#define CUSTOMER_LINE_WIDTH 73

static void printCyanLine() {
    printf(CYAN);
    for (int i = 0; i < CUSTOMER_LINE_WIDTH; i++) putchar('=');
    printf(RESET "\n");
}

void showCustomerInfo(Customer *customer) {
    printf("\n");
    printCyanLine();
    printf(CYAN "%-25s" RESET " " YELLOW_BOLD "THONG TIN KHACH HANG" RESET " " CYAN "%28s\n" RESET, "", "");
    printCyanLine();
    
    const char *rankColor = getRankColor(customer->rank);
    
    printf(WHITE "ID Khach Hang   : " RESET YELLOW_BOLD "%d\n" RESET, customer->id);
    printf(WHITE "Ten Khach Hang  : " RESET YELLOW_BOLD "%s\n" RESET, customer->name);
    printf(WHITE "So Dien Thoai   : " RESET YELLOW_BOLD "%s\n" RESET, customer->phone);
    printf(WHITE "Dia Chi         : " RESET YELLOW_BOLD "%s\n" RESET, customer->address);
    printf(WHITE "Hang Khach Hang : " RESET "%s%s%s\n", rankColor, customer->rank, RESET);
    printf(WHITE "Tich Luy        : " RESET GREEN "%.3f VND\n" RESET, customer->totalSpent);
    
    printCyanLine();
}

void showCustomerRegistration(Customer *customer) {
    printf("\n");
    printCyanLine();
    printf(CYAN "%-20s" RESET " " YELLOW_BOLD "DANG KY THANH CONG" RESET " " CYAN "%30s\n" RESET, "", "");
    printCyanLine();
    
    printf(WHITE "ID Khach Hang   : " RESET YELLOW_BOLD "%d\n" RESET, customer->id);
    printf(WHITE "Ten Khach Hang  : " RESET YELLOW_BOLD "%s\n" RESET, customer->name);
    printf(WHITE "So Dien Thoai   : " RESET YELLOW_BOLD "%s\n" RESET, customer->phone);
    printf(WHITE "Dia Chi         : " RESET YELLOW_BOLD "%s\n" RESET, customer->address);
    printf(WHITE "Hang Khach Hang : " RESET "%s%s%s\n", getRankColor(customer->rank), customer->rank, RESET);
    printf(WHITE "Tich Luy        : " RESET GREEN "%.3f VND\n" RESET, customer->totalSpent);
    
    printCyanLine();
}

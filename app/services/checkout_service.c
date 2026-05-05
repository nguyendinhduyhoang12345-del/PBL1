#include "checkout_service.h"
#include <string.h>

double calculateSubtotal(Bill* bill) {
    double total = 0;
    for (int i = 0; i < bill->itemCount; i++) {
        total += bill->items[i].price * bill->items[i].quantity;
    }
    return total;
}

double getDiscountPercent(char* rank) {
    if (strcmp(rank, "Diamond") == 0) return 0.30; // Giảm 30%
    if (strcmp(rank, "Gold") == 0) return 0.20;    // Giảm 20%
    if (strcmp(rank, "Silver") == 0) return 0.10;  // Giảm 10%
    return 0.0; // Khách thường (Bronze hoặc vãng lai) không giảm
}

void updateCustomerRank(Customer* cust) {
    // Điều kiện xét từ cao xuống thấp
    if (cust->totalSpent >= 10000000) {
        strcpy(cust->rank, "Diamond");
    } else if (cust->totalSpent >= 5000000) {
        strcpy(cust->rank, "Gold");
    } else if (cust->totalSpent >= 1000000) {
        strcpy(cust->rank, "Silver");
    } else {
        strcpy(cust->rank, "Bronze");
    }
}
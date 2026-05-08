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
    if (strcmp(rank, RANK_DIAMOND) == 0) return 0.30;
    if (strcmp(rank, RANK_GOLD) == 0) return 0.20;
    if (strcmp(rank, RANK_SILVER) == 0) return 0.10;
    return 0.0; // Khách thường (Bronze hoặc vãng lai) không giảm
}

void updateCustomerRank(Customer* cust) {
    // Điều kiện xét từ cao xuống thấp
    if (cust->totalSpent >= 50000000) {
        strcpy(cust->rank, RANK_DIAMOND);
    } else if (cust->totalSpent >= 2000000) {
        strcpy(cust->rank, RANK_GOLD);
    } else if (cust->totalSpent >= 500000) {
        strcpy(cust->rank, RANK_SILVER);
    } else {
        strcpy(cust->rank, RANK_BRONZE);
    }
}
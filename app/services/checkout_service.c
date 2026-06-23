#include "checkout_service.h"
#include <string.h>
#include "../services/cart_service.h"

double calculateSubtotal(Bill* bill) {
    double total = 0;
    CartNode *node = bill->cart.head;
    while (node) {
        total += node->item.price * node->item.quantity;
        node = node->next;
    }
    return total;
    return total;
}

double getDiscountPercent(char* rank) {
    if (strcmp(rank, RANK_DIAMOND) == 0) return 0.30;
    if (strcmp(rank, RANK_GOLD) == 0) return 0.20;
    if (strcmp(rank, RANK_SILVER) == 0) return 0.10;
    return 0.0;
}

void updateCustomerRank(Customer* cust) {
    if (cust->totalSpent >= 5000000) {
        strcpy(cust->rank, RANK_DIAMOND);
    } else if (cust->totalSpent >= 2000000) {
        strcpy(cust->rank, RANK_GOLD);
    } else if (cust->totalSpent >= 500000) {
        strcpy(cust->rank, RANK_SILVER);
    } else {
        strcpy(cust->rank, RANK_BRONZE);
    }
}
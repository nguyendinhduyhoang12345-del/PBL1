#ifndef CHECKOUT_CONTROLLER_H
#define CHECKOUT_CONTROLLER_H

#include "../models/models.h"
#include "../services/btree_service.h" 

// Hàm xử lý toàn bộ luồng thanh toán
void processCheckout(Bill* currentBill);

#endif
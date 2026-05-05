#ifndef ORDER_CONTROLLER_H
#define ORDER_CONTROLLER_H

#include "../models/models.h"

void addToCart(Bill *currentBill);

void removeFromCart(Bill *currentBill);

void updateQuantity(Bill *currentBill);

// Điều phối luồng Đặt món
void handleOrderMenu(Bill* currentBill);

// Điều phối luồng Xem và chỉnh sửa giỏ hàng
void handleCartMenu(Bill* currentBill);


#endif
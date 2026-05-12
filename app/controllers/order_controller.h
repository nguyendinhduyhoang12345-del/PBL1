#ifndef ORDER_CONTROLLER_H
#define ORDER_CONTROLLER_H

#include "../models/models.h"

<<<<<<< HEAD
void addToCart(Bill *currentBill, OrderItem item);
=======
void addToCart(Bill *currentBill);
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

void removeFromCart(Bill *currentBill);

void updateQuantity(Bill *currentBill);

// Điều phối luồng Đặt món
void handleOrderMenu(Bill* currentBill);

// Điều phối luồng Xem và chỉnh sửa giỏ hàng
void handleCartMenu(Bill* currentBill);


#endif
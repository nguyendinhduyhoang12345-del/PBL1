#ifndef CART_SERVICE_H
#define CART_SERVICE_H

#include "../models/models.h"

void cartInit(Cart *cart);
void cartAddItem(Cart *cart, OrderItem item);
CartNode *cartGetNodeByIndex(Cart *cart, int index);
int cartRemoveItemByIndex(Cart *cart, int index);
int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity);

#endif
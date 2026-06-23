#include <stdlib.h>
#include "cart_service.h"

void cartInit(Cart *cart) {
    cart->head = NULL;
    cart->tail = NULL;
    cart->itemCount = 0;
}

void cartAddItem(Cart *cart, OrderItem item) {
    CartNode *node = (CartNode *)malloc(sizeof(CartNode));
    node->item = item;
    node->prev = cart->tail;
    node->next = NULL;
    if (cart->tail) cart->tail->next = node; 
    cart->tail = node;
    if (!cart->head) cart->head = node;
    cart->itemCount++;
}

CartNode *cartGetNodeByIndex(Cart *cart, int index) {
    if (!cart || index < 0 || index >= cart->itemCount) return NULL;
    CartNode *node = cart->head;
    for (int i = 0; i < index; i++) node = node->next;
    return node;
}

int cartRemoveItemByIndex(Cart *cart, int index) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (!node) return 0;
    if (node->prev) node->prev->next = node->next;
    else cart->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else cart->tail = node->prev;
    free(node);
    cart->itemCount--;
    return 1;
}

int cartUpdateQuantityByIndex(Cart *cart, int index, int newQuantity) {
    CartNode *node = cartGetNodeByIndex(cart, index);
    if (!node) return 0;
    node->item.quantity = newQuantity;
    node->item.totalPrice = node->item.price * newQuantity;
    return 1;
}
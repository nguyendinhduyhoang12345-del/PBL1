#ifndef CHECKOUT_SERVICE_H
#define CHECKOUT_SERVICE_H

#include "../models/models.h"

double calculateSubtotal(Bill* bill);
double getDiscountPercent(char* rank);
void updateCustomerRank(Customer* cust);

#endif
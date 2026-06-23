#ifndef HISTORY_SERVICE_H
#define HISTORY_SERVICE_H

#include "../models/models.h"

// Thêm hóa đơn vào linked list (bộ nhớ)
int addBillToHistory(int customerId, Bill* bill);

// Lấy danh sách hóa đơn của 1 khách hàng (từ linked list)
HistoryNode* getCustomerHistory(int customerId);

void freeHistoryList(HistoryNode* head);

int getCustomerIdByPhone(char* phone);

HistoryNode* searchBillById(int billId);

#endif
#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H
#include "../models/models.h"

void loadMenuFromFile(const char* filename);

void loadCustomersFromFile(BTreeNode** root);

void saveAllCustomersToFile();

// Lưu hóa đơn vào file lịch sử của khách đó
void saveBillToHistoryFile(int customerId, Bill* bill);

// Lưu hóa đơn tóm tắt vào file tổng hợp toàn bộ quán
void saveBillToGlobalFile(Bill* bill, int customerId);

// Load lịch sử từ file
void loadHistoryFromFile();

#endif
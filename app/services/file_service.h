#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H
#include "../models/models.h"

// Hàm đọc file menu.txt và nạp vào mảng toàn cục
void loadMenuFromFile(const char* filename);

void loadCustomersFromFile(BTreeNode** root);

void saveAllCustomersToFile();

#endif
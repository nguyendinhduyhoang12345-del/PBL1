#include "history_service.h"
#include "btree_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern BTreeNode* btreeRoot;




// thêm hóa đơn vào linked list (bộ nhớ)
void addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    newNode->customerId = customerId;
    newNode->bill = *bill; // Sao chép toàn bộ thông tin hóa đơn
    newNode->next = historyHead;
    historyHead = newNode;
}

// lưu hóa đơn vào file lịch sử của từng khách
void saveBillToHistoryFile(int customerId, Bill* bill) {
    char filename[100];
    sprintf(filename, "app/database/history_khach_%d.txt", customerId);

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        file = fopen(filename, "w");
        if (file == NULL) {
            return;
        }
    }

    fprintf(file, "%d|%d|%s|%.3f|%.3f\n", 
            bill->id, customerId, bill->dateTime, bill->finalPrice, bill->discount);
    fclose(file);
}

// lưu hóa đơn tóm tắt vào file lịch sử chung của quán
void saveBillToGlobalFile(Bill* bill, int customerId) {
    FILE* file = fopen("app/database/history.txt", "a");
    if (file == NULL) {
        file = fopen("app/database/history.txt", "w");
        if (file == NULL) {
            return; 
        }
    }
    fprintf(file, "%d|%d|%s|%.3f|%.3f\n",
            bill->id, customerId, bill->dateTime, bill->finalPrice, bill->discount);
    fclose(file);
}

// load lịch sử từ file
void loadHistoryFromFile() { // Đọc file lịch sử chung của quán để khởi tạo linked list trong bộ nhớ
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return; 
    }

    char line[200];
    while (fgets(line, sizeof(line), file)) {
        int billId, customerId;
        double finalPrice, discount;
        char dateTime[50];

        sscanf(line, "%d|%d|%[^|]|%lf|%lf", &billId, &customerId, dateTime, &finalPrice, &discount);
        
        // Tạo Bill tạm để lưu vào linked list
        Bill tempBill = {0};
        tempBill.id = billId;
        tempBill.finalPrice = finalPrice;
        tempBill.discount = discount;
        strcpy(tempBill.dateTime, dateTime);
        
        // thêm vào linked list để quản lý trong bộ nhớ
        addBillToHistory(customerId, &tempBill);
    }
    fclose(file);
}

// lấy lịch sử hóa đơn của một khách hàng cụ thể từ linked list
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* result = NULL; // Danh sách kết quả (có thể có nhiều hóa đơn của cùng 1 khách)
    HistoryNode* current = historyHead; // Duyệt qua linked list để tìm tất cả hóa đơn của khách này
    
    while (current != NULL) {
        if (current->customerId == customerId) {
            HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
            *newNode = *current;
            newNode->next = result; // Thêm vào đầu danh sách kết quả
            result = newNode; // Cập nhật lại head của danh sách kết quả
        }
        current = current->next;
    }
    
    return result;
}

// Tìm ID khách hàng theo số điện thoại
int getCustomerIdByPhone(char* phone) {
    Customer* cust = searchBTree(btreeRoot, phone);
    if (cust != NULL) {
        return cust->id;
    }
    return -1;  // Không tìm thấy
}

// Tìm hóa đơn theo ID bill
HistoryNode* searchBillById(int billId) {
    HistoryNode* current = historyHead;
    while (current != NULL) {
        if (current->bill.id == billId) {
            return current;
        }
        current = current->next;
    }
    return NULL;  // Không tìm thấy
}
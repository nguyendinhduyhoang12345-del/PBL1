#include "history_service.h"
#include "btree_service.h"
#include "../services/cart_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern BTreeNode* btreeRoot;


int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) {  
        printf("[!] Loi: Khong du bo nho\n");
        return -1;
    }
    newNode->customerId = customerId;
    newNode->bill = *bill;
    newNode->next = historyHead;
    historyHead = newNode;
    return 0;
}

// lấy lịch sử hóa đơn của một khách hàng cụ thể
HistoryNode* getCustomerHistory(int customerId) {
    HistoryNode* result = NULL;
    HistoryNode* current = historyHead;
    
    while (current != NULL) {
        if (current->customerId == customerId) {
            HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
            *newNode = *current;
            newNode->next = result;
            result = newNode;
        }
        current = current->next;
    }
    
    return result;
}

// giải phóng bộ nhớ của linked list lịch sử
void freeHistoryList(HistoryNode* head) {
    while (head != NULL) {
        HistoryNode* temp = head;
        head = head->next;
        free(temp);
    }
}

// Tìm ID khách hàng theo số điện thoại
int getCustomerIdByPhone(char* phone) {
    Customer* cust = searchBTree(btreeRoot, phone);
    if (cust != NULL) {
        return cust->id;
    }
    return -1;
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
    return NULL;
}
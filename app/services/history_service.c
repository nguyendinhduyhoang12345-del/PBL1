#include "history_service.h"
#include "btree_service.h"
#include "../services/cart_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern BTreeNode* btreeRoot;




// thêm hóa đơn vào linked list (bộ nhớ)
int addBillToHistory(int customerId, Bill* bill) {
    HistoryNode* newNode = (HistoryNode*)malloc(sizeof(HistoryNode));
    if (newNode == NULL) {  
        printf("[!] Loi: Khong du bo nho\n");
        return -1;
    }
    newNode->customerId = customerId;
    newNode->bill = *bill; // Sao chép toàn bộ thông tin hóa đơn
    newNode->next = historyHead;
    historyHead = newNode;
    return 0;
}

// lưu hóa đơn vào file lịch sử của từng khách
void saveBillToHistoryFile(int customerId, Bill* bill) {
    char filename[100];
    sprintf(filename, "app/database/history_khach_%d.txt", customerId);

    FILE* file = fopen(filename, "a");
    if (file == NULL) {
        file = fopen(filename, "w");
        if (file == NULL) return;
    }

    // Lưu dòng thông tin bill
    fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d|%s|%.3f\n",
            bill->id,
            customerId,
            bill->customer.name,
            bill->customer.phone,
            bill->dateTime,
            bill->total,
            bill->discount,
            bill->finalPrice,
            bill->cart.itemCount,
            bill->customer.rank,
            bill->customer.totalSpent);

    // Lưu từng item
    CartNode *node = bill->cart.head;
    while (node) {
        fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%.3f|%s\n",
                node->item.id,
                node->item.name,
                node->item.price,
                node->item.quantity,
                node->item.option,
                node->item.totalPrice,
                node->item.note);
        node = node->next;
    }

    // Lưu dấu kết thúc
    fprintf(file, "END\n");
    fclose(file);
}

// lưu hóa đơn tóm tắt vào file lịch sử chung của quán
void saveBillToGlobalFile(Bill* bill, int customerId) {
    FILE* file = fopen("app/database/history.txt", "a");
    if (file == NULL) {
        file = fopen("app/database/history.txt", "w");
        if (file == NULL) return;
    }

    // Lưu dòng thông tin bill
    fprintf(file, "BILL|%d|%d|%s|%s|%s|%.3f|%.3f|%.3f|%d|%s|%.3f\n",
            bill->id,
            customerId,
            bill->customer.name,
            bill->customer.phone,
            bill->dateTime,
            bill->total,
            bill->discount,
            bill->finalPrice,
            bill->cart.itemCount,
            bill->customer.rank,
            bill->customer.totalSpent);

    // Lưu từng item
    CartNode *node = bill->cart.head;
    while (node) {
        fprintf(file, "ITEM|%d|%s|%.3f|%d|%d|%.3f|%s\n",
                node->item.id,
                node->item.name,
                node->item.price,
                node->item.quantity,
                node->item.option,
                node->item.totalPrice,
                node->item.note);
        node = node->next;
    }

    // Lưu dấu kết thúc
    fprintf(file, "END\n");
    fclose(file);
}

// load lịch sử từ file
void loadHistoryFromFile() {
    FILE* file = fopen("app/database/history.txt", "r");
    if (file == NULL) {
        return;
    }

    char line[500];
    Bill tempBill = {0};
    int itemCount = 0;
    int billStarted = 0;
    int currentCustomerId = -1;

    while (fgets(line, sizeof(line), file)) {
        int len = strlen(line);
        while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
            line[--len] = '\0';
        }

        if (strncmp(line, "BILL|", 5) == 0) {
            int billId, customerId, itemCountInFile;
            char nameKhach[50], phoneKhach[15], dateTime[50], rankKhach[20];
            double total, discount, finalPrice, totalSpent;

            sscanf(line, "BILL|%d|%d|%49[^|]|%14[^|]|%49[^|]|%lf|%lf|%lf|%d|%19[^|]|%lf",
                    &billId,
                    &customerId,
                    nameKhach,
                    phoneKhach,
                    dateTime,
                    &total,
                    &discount,
                    &finalPrice,
                    &itemCountInFile,
                    rankKhach,
                    &totalSpent);

            tempBill = (Bill){0};
            tempBill.id = billId;
            tempBill.customer.id = customerId;
            strcpy(tempBill.customer.name, nameKhach);
            strcpy(tempBill.customer.phone, phoneKhach);
            strcpy(tempBill.dateTime, dateTime);
            tempBill.total = total;
            tempBill.discount = discount;
            tempBill.finalPrice = finalPrice;
            tempBill.cart.itemCount = itemCountInFile;
            strcpy(tempBill.customer.rank, rankKhach);
            tempBill.customer.totalSpent = totalSpent;

            itemCount = 0;
            currentCustomerId = customerId;
            billStarted = 1;
        }
        else if (strncmp(line, "ITEM|", 5) == 0 && billStarted) {
            if (itemCount < MAX_GIO_HANG) {
                int id, quantity, option;
                char name[50], note[100];
                double price, totalPrice;

                sscanf(line, "ITEM|%d|%49[^|]|%lf|%d|%d|%lf|%99[^\n]",
                        &id, name, &price, &quantity, &option, &totalPrice, note);
                
                OrderItem item;
                item.id = id;
                strcpy(item.name, name);
                item.price = price;
                item.quantity = quantity;
                item.option = option;
                item.totalPrice = totalPrice;
                strcpy(item.note, note);
                cartAddItem(&tempBill.cart, item); // nếu không có dòng này thì sẽ không cập nhật được itemCount trong cart, dẫn đến khi lưu vào file sẽ bị sai số lượng item
                itemCount++;
            }
        }
        else if (strcmp(line, "END") == 0 && billStarted) {
            tempBill.cart.itemCount = itemCount;
            if (addBillToHistory(currentCustomerId, &tempBill) != 0) {
                printf("[!] Canh bao: Khong the them bill vao lich su\n");
            }

            tempBill = (Bill){0};
            itemCount = 0;
            billStarted = 0;
            currentCustomerId = -1;
        }
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
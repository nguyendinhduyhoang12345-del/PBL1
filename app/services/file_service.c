#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file_service.h"
#include "../services/btree_service.h"
#include "../services/hash_service.h"
#include "../services/history_service.h"
#include "../services/cart_service.h"

void loadMenuFromFile(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("[!] Loi: Khong the mo file %s!\n", filename);
        return;
    }

    char line[200];
    int i = 0;
    
    // Lấy giờ hiện tại để tạo tồn kho linh hoạt
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int gioHienTai = tm.tm_hour;

    menuHashTable = createHashTable(); // Khởi tạo hash table cho menu

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0; // Xóa ký tự xuống dòng
        
        // Cắt chuỗi bằng dấu |
        char *token = strtok(line, "|");
        if (token == NULL) continue;
        menu[i].id = atoi(token);
        
        token = strtok(NULL, "|");
        if (token != NULL) strcpy(menu[i].name, token);
        
        token = strtok(NULL, "|");
        if (token != NULL) menu[i].price = atof(token);
        
        token = strtok(NULL, "|");
        if (token != NULL) menu[i].hasOptions = atoi(token);
        
        hashTableInsert(menuHashTable, menu[i].id, menu[i]); // Thêm món vào hash table
        i++;
        if (i >= TONG_SO_MON) break; // Chốt chặn chỉ nạp tối đa 25 món
    }
    fclose(file);
}

void loadCustomersFromFile(BTreeNode** root) {
    FILE *f = fopen("app/database/khachhang.txt", "r");
    if (f == NULL) {
        printf("\n  [!] Khong tim thay file khachhang.txt");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) < 5) continue;

        Customer* c = (Customer*)malloc(sizeof(Customer));

        char *token = strtok(line, "|");
        if (token) c->id = atoi(token);

        token = strtok(NULL, "|");
        if (token) {
            strcpy(c->phone, token);
            int len = strlen(c->phone);
            while (len > 0 && (c->phone[len-1] == ' ' || c->phone[len-1] == '\t')) {
                c->phone[--len] = 0;
            }
            int i = 0;
            while (c->phone[i] == ' ' || c->phone[i] == '\t') i++;
            if (i > 0) memmove(c->phone, c->phone + i, strlen(c->phone + i) + 1);
        }

        token = strtok(NULL, "|");
        if (token) strcpy(c->name, token);

        token = strtok(NULL, "|");
        if (token) strcpy(c->address, token);

        token = strtok(NULL, "|");
        if (token) c->totalSpent = atof(token);

        token = strtok(NULL, "|");
        if (token) strcpy(c->rank, token);

        danhSachKH[soLuongKH] = *c;
        insertBTree(root, &danhSachKH[soLuongKH]);
        soLuongKH++;
        free(c);
    }
    fclose(f);
}

void saveAllCustomersToFile() {
    FILE *f = fopen("app/database/khachhang.txt", "w");
    if (f == NULL) {
        printf("[!] Loi: Khong the mo file khachhang.txt de ghi\n");
        return;
    }
    for (int i = 0; i < soLuongKH; i++) {
        fprintf(f, "%d|%s|%s|%s|%.3f|%s\n", 
                danhSachKH[i].id,
                danhSachKH[i].phone, 
                danhSachKH[i].name, 
                danhSachKH[i].address, 
                danhSachKH[i].totalSpent, 
                danhSachKH[i].rank);
    }
    fclose(f);
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
                cartAddItem(&tempBill.cart, item);
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file_service.h"
#include "../services/btree_service.h"
#include "../services/hash_service.h"

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
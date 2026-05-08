#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file_service.h"
#include "../services/btree_service.h"

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

    // Đọc từng dòng trong file
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
        
        // Tự động tạo tồn kho linh hoạt theo khung giờ (dựa trên code cũ của bạn)
        if (i >= 20) {
            menu[i].stock = 500 + rand() % 501; // Giải khát
        } else {
            if (gioHienTai >= 6 && gioHienTai < 11) menu[i].stock = 400 + rand() % 101;
            else if (gioHienTai >= 11 && gioHienTai < 14) menu[i].stock = 300 + rand() % 101;
            else if (gioHienTai >= 14 && gioHienTai < 17) menu[i].stock = 270 + rand() % 31;
            else if (gioHienTai >= 17 && gioHienTai < 21) menu[i].stock = 50 + rand() % 221;
            else menu[i].stock = rand() % 51;
        }
        
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
        if (token) c->id = atoi(token); // Token 1: ID

        token = strtok(NULL, "|");
        if (token) {
            strcpy(c->phone, token);
            // Trim khoảng trắng ở đầu và cuối số điện thoại
            int len = strlen(c->phone);
            while (len > 0 && (c->phone[len-1] == ' ' || c->phone[len-1] == '\t')) {
                c->phone[--len] = 0;
            }
            int i = 0;
            while (c->phone[i] == ' ' || c->phone[i] == '\t') i++;
            if (i > 0) memmove(c->phone, c->phone + i, strlen(c->phone + i) + 1);
        } // Token 2: SĐT

        token = strtok(NULL, "|");
        if (token) strcpy(c->name, token); // Token 3: Tên

        token = strtok(NULL, "|");
        if (token) strcpy(c->address, token); // Token 4: Địa chỉ

        token = strtok(NULL, "|");
        if (token) c->totalSpent = atof(token); // Token 5: Tiền

        token = strtok(NULL, "|");
        if (token) strcpy(c->rank, token); // Token 6: Rank

        danhSachKH[soLuongKH] = *c;
        insertBTree(root, &danhSachKH[soLuongKH]);
        soLuongKH++;
        free(c);
    }
    fclose(f);
    printf("\n[He thong] Da load %d khach hang tu file.\n", soLuongKH);
}

void saveAllCustomersToFile() {
    FILE *f = fopen("app/database/khachhang.txt", "w");
    if (f == NULL) {  // ✓ Kiểm tra lỗi
        printf("[!] Loi: Khong the mo file khachhang.txt de ghi\n");
        return;
    }
    for (int i = 0; i < soLuongKH; i++) {
        // Thêm %d| ở đầu dòng
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
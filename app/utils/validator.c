#include <stdio.h>
#include <string.h>
#include "validator.h"

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInt(const char* prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        printf("  !! Loi: Vui long nhap so hop le! \n");
        clearBuffer();
        printf("%s", prompt);
    }
    clearBuffer(); 
    return value;
}

double getValidDouble(const char* prompt) {
    double value;
    printf("%s", prompt);
    while (scanf("%lf", &value) != 1) {
        printf("  !! Loi: Vui long nhap so hop le! \n");
        clearBuffer();
        printf("%s", prompt);
    }
    clearBuffer();
    return value;
}

// Hàm kiểm tra nếu chuỗi chỉ chứa chữ số
int isDigits(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return 0; 
        }
    }
    return 1;
}

// Hàm loại bỏ khoảng trắng và ký tự xuống dòng ở đầu và cuối chuỗi
void trimString(char* s) {
    int len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || s[len-1] == ' ' || s[len-1] == '\t')) {
        s[--len] = '\0';
    }
    int start = 0;
    while (s[start] == ' ' || s[start] == '\t') {
        start++;
    }
    if (start > 0) {
        memmove(s, s + start, strlen(s + start) + 1);
    }
}

// Hàm nhập số điện thoại hợp lệ
void getValidPhoneNumber(char* phone, int maxLen, const char* prompt) {
    while (1) {
        printf("%s", prompt);
        scanf("%s", phone);
        clearBuffer();
        trimString(phone);
        if (strlen(phone) != 10) {
            printf("  !! Loi: SDT phai co 10 chu so.\n");
            continue;
        }
        if (!isDigits(phone)) {
            printf("  !! Loi: SDT chi duoc chua chu so.\n");
            continue;
        }
        break;
    }
}

// Hàm nhập chuỗi hợp lệ
void getValidLine(char* buffer, int maxLen, const char* prompt) {
    printf("%s", prompt);
    if (fgets(buffer, maxLen, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    trimString(buffer);
}
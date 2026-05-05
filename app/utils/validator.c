#include <stdio.h>
#include "validator.h"

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int getValidInt(const char* prompt) {
    int value;
    printf("%s", prompt);
    while (scanf("%d", &value) != 1) {
        printf("  !! Loi: Vui long chi nhap so! \n");
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

// kiểm tra chuỗi chỉ gồm chữ số
int isDigits(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] < '0' || s[i] > '9') {
            return 0; 
        }
    }
    return 1;
}
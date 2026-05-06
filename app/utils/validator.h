#ifndef VALIDATOR_H
#define VALIDATOR_H

// Hàm xóa bộ đệm để chống trôi lệnh (while(getchar() != '\n'))
void clearBuffer();

// Hàm ép người dùng phải nhập đúng số nguyên, nếu nhập chữ sẽ báo lỗi và bắt nhập lại
int getValidInt(const char* prompt);

double getValidDouble(const char* prompt);

int isDigits(const char* s);

void trimString(char* s);

void getValidPhoneNumber(char* phone, int maxLen, const char* prompt);

void getValidLine(char* buffer, int maxLen, const char* prompt);

#endif
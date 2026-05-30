#ifndef HELPER_H
#define HELPER_H

#define CYAN        "\033[96m"
#define GREEN       "\033[92m"
#define YELLOW_BOLD "\033[1;33m"
#define BROWN       "\033[0;33m"
#define SILVER      "\033[0;97m"
#define WHITE       "\033[0;37m"
#define RESET       "\033[0m"

const char* getRankColor(const char *rank);

// Hàm kẻ đường ngang trang trí (độ dài 136 ký tự để khớp với Menu của bạn)
void printLine();

// Hàm lấy ngày giờ hiện tại và lưu vào chuỗi buffer
void getCurrentDateTime(char* buffer, int maxSize);

#endif
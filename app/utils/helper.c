#include <stdio.h>
#include <time.h>
#include "helper.h"

void printLine() {
    for(int i = 0; i < 136; i++) printf("=");
    printf("\n");
}

void getCurrentDateTime(char* buffer, int maxSize) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    // Định dạng: Ngày/Tháng/Năm Giờ:Phút:Giây
    strftime(buffer, maxSize, "%d/%m/%Y %H:%M:%S", &tm);
}
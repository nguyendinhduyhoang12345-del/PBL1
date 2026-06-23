#include <stdio.h>
#include <string.h>
#include <time.h>
#include "helper.h"
#include "../models/models.h"

const char* getRankColor(const char *rank) {
    if (strcmp(rank, RANK_BRONZE) == 0) return BROWN;
    if (strcmp(rank, RANK_SILVER) == 0) return SILVER;
    if (strcmp(rank, RANK_GOLD) == 0) return YELLOW_BOLD;
    if (strcmp(rank, RANK_DIAMOND) == 0) return CYAN;
    return RESET;
}

void printLine() {
    for(int i = 0; i < 136; i++) printf("=");
    printf("\n");
}

void getCurrentDateTime(char* buffer, int maxSize) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(buffer, maxSize, "%d/%m/%Y %H:%M:%S", &tm);
}
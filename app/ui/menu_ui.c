#include <stdio.h>
#include <string.h>
#include "../models/models.h"
#include "../utils/helper.h"
#include "menu_ui.h"

static void printCyanLine() {
    printf(CYAN);
    printLine();
    printf(RESET);
}

static void padSpaces(int count) {
    for (int i = 0; i < count; i++) putchar(' ');
}

static void printItemCell(int id, const char *name, double price, int stock, int nameWidth) {
    printf(" %2d.", id);
    printf(WHITE "%-*.*s" RESET, nameWidth, nameWidth, name);
    printf(" " GREEN "%9.0lf" RESET, price);
    printf(" (K:%-3d)", stock);
}

static void printEmptyCell(int width) {
    padSpaces(width);
}

void showMenuUI() {
    char left[100], mid[100], right[100];
    char pbl_ascii[5][80] = {   
        "  _    _         _  __ ",
        " | |  | |       | |/ / ",
        " | |__| |       | ' /  ",
        " |  __  |       | . \\  ",
        " |_|  |_|  AND  |_|\\_\\ RESTAURANT"
    };

    printf("\n");
    printCyanLine();
    // In tiêu đề nửa trên
    printf(CYAN "| " RESET WHITE "%-42s" RESET CYAN " | " RESET "%-87s" CYAN " |" RESET "\n", "[ COM / MON CHINH ]", "");
    printCyanLine();

    // NỬA TRÊN: Cơm (Trái) - Combo & Khẩu hiệu (Phải)
    for (int i = 0; i < 8; i++) {
        printf(CYAN "| " RESET);
        printItemCell(menu[i].id, menu[i].name, menu[i].price, menu[i].stock, 20);
        printf(CYAN " | " RESET);

        if (i < 5) {
            printf("%-87s", pbl_ascii[i]);
        } else if (i == 5) {
            padSpaces(87);
        } else if (i == 6) {
            printItemCell(menu[10].id, menu[10].name, menu[10].price, menu[10].stock, 26);
            padSpaces(39);
        } else if (i == 7) {
            printItemCell(menu[11].id, menu[11].name, menu[11].price, menu[11].stock, 26);
            padSpaces(39);
        }

        printf(CYAN " |" RESET "\n");
    }

    printCyanLine();
    
    // NỬA DƯỚI: Canh - Món thêm - Giải khát
    printf(CYAN "| " RESET WHITE "%-42s" RESET CYAN " | " RESET WHITE "%-42s" RESET CYAN " | " RESET WHITE "%-42s" RESET CYAN " |" RESET "\n", "[ CANH ]", "[ MON THEM ]", "[ GIAI KHAT ]");
    printCyanLine();

    // Lặp 8 lần vì Mảng "Món thêm" có tới 8 món (dài nhất ở nửa dưới)
    for (int i = 0; i < 8; i++) { 
        printf(CYAN "| " RESET);
        if (i < 2) {
            printItemCell(menu[8+i].id, menu[8+i].name, menu[8+i].price, menu[8+i].stock, 20);
        } else {
            printEmptyCell(42);
        }

        printf(CYAN " | " RESET);
        printItemCell(menu[12+i].id, menu[12+i].name, menu[12+i].price, menu[12+i].stock, 20);
        printf(CYAN " | " RESET);

        if (i < 5) {
            printItemCell(menu[20+i].id, menu[20+i].name, menu[20+i].price, menu[20+i].stock, 20);
        } else {
            printEmptyCell(42);
        }

        printf(CYAN " |" RESET "\n");
    }
    printCyanLine();
}
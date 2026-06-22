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

static void printItemCell(int id, const char *name, double price, int nameWidth) {
    printf(" %2d.", id);
    printf(WHITE "%-*.*s" RESET, nameWidth, nameWidth, name);
    printf(" " GREEN "%9.0lf" RESET, price);
}

void showMenuUI() {

    const char *pbl_ascii[5] = {
        "  _    _         _  __",
        " | |  | |       | |/ /",
        " | |__| |       | ' /",
        " |  __  |       | . \\",
        " |_|  |_|  AND  |_|\\_\\ RESTAURANT"
    };

    printf("\n");
    printCyanLine();

    printf(CYAN "| " RESET
           YELLOW_BOLD "%-42s" RESET
           CYAN " | " RESET
           "%-87s"
           CYAN " |" RESET "\n",
           "[ COM / MON CHINH ]", "");
    printCyanLine();

    for (int i = 0; i < 8; i++) {
        printf(CYAN "| " RESET);
        printItemCell(menu[i].id, menu[i].name, menu[i].price, 28);

        printf(CYAN " | " RESET);

        if (i < 5) {
            printf(YELLOW_BOLD "%-87s" RESET, pbl_ascii[i]);
        } else if (i == 5) {
            padSpaces(87);
        } else if (i == 6) {
            printItemCell(menu[10].id, menu[10].name, menu[10].price, 28);
            padSpaces(45); 
        } else { // i == 7
            printItemCell(menu[11].id, menu[11].name, menu[11].price, 28);
            padSpaces(45);
        }

        printf(CYAN " |" RESET "\n");
    }

    printCyanLine();

    printf(CYAN "| " RESET
           YELLOW_BOLD "%-42s" RESET
           CYAN " | " RESET
           YELLOW_BOLD "%-42s" RESET
           CYAN " | " RESET
           YELLOW_BOLD "%-42s" RESET
           CYAN " |" RESET "\n",
           "[ CANH ]", "[ MON THEM ]", "[ GIAI KHAT ]");
    printCyanLine();

    for (int i = 0; i < 8; i++) {
        printf(CYAN "| " RESET);
        if (i < 2) {
            printItemCell(menu[8 + i].id, menu[8 + i].name, menu[8 + i].price, 28);
        } else {
            padSpaces(42);
        }

        printf(CYAN " | " RESET);
        printItemCell(menu[12 + i].id, menu[12 + i].name, menu[12 + i].price, 28);

        printf(CYAN " | " RESET);
        if (i < 5) {
            printItemCell(menu[20 + i].id, menu[20 + i].name, menu[20 + i].price, 28);
        } else {
            padSpaces(42);
        }

        printf(CYAN " |" RESET "\n");
    }

    printCyanLine();
}
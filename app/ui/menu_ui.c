#include <stdio.h>
#include <string.h>
#include "../models/models.h"
#include "../utils/helper.h"
#include "menu_ui.h"

void showMenuUI() {
    char left[100], mid[100], right[100];
    char pbl_ascii[5][80] = {
        "      ____  ____  _      ",
        "     |  _ \\| __ )| |     ",
        "     | |_) |  _ \\| |     ",
        "     |  __/| |_) | |___  ",
        "     |_|   |____/|_____| COMBO"
    };

    printf("\n");
    printLine();
    // In tiêu đề nửa trên
    printf("| %-42s | %-87s |\n", "[ COM / MON CHINH ]", "");
    printLine();

    // NỬA TRÊN: Cơm (Trái) - Combo & Khẩu hiệu (Phải)
    for (int i = 0; i < 8; i++) {
        // Cột Trái: 8 món Cơm đầu tiên (index 0 -> 7)
        sprintf(left, " %2d.%-20.20s %9.0lf (K:%-3d)", menu[i].id, menu[i].name, menu[i].price, menu[i].stock);

        // Cột Phải: Ghép ASCII Art và 2 món Combo
        if (i < 5) {
            sprintf(right, "%s", pbl_ascii[i]);
        } else if (i == 5) {
            strcpy(right, "");
        } else if (i == 6) { 
            // Món Combo 1 (index 10)
            sprintf(right, " %2d.%-26.26s %9.0lf (K:%-3d)", menu[10].id, menu[10].name, menu[10].price, menu[10].stock);
        } else if (i == 7) { 
            // Món Combo 2 (index 11)
            sprintf(right, " %2d.%-26.26s %9.0lf (K:%-3d)", menu[11].id, menu[11].name, menu[11].price, menu[11].stock);
        }
        
        printf("| %-42s | %-87s |\n", left, right);
    }

    printLine();
    
    // NỬA DƯỚI: Canh - Món thêm - Giải khát
    printf("| %-42s | %-42s | %-42s |\n", "[ CANH ]", "[ MON THEM ]", "[ GIAI KHAT ]");
    printLine();

    // Lặp 8 lần vì Mảng "Món thêm" có tới 8 món (dài nhất ở nửa dưới)
    for (int i = 0; i < 8; i++) { 
        // Cột Trái: Canh (index 8, 9)
        if (i < 2) sprintf(left, " %2d.%-20.20s %9.0lf (K:%-3d)", menu[8+i].id, menu[8+i].name, menu[8+i].price, menu[8+i].stock);
        else strcpy(left, "");

        // Cột Giữa: Món thêm (index 12 -> 19)
        sprintf(mid, " %2d.%-20.20s %9.0lf (K:%-3d)", menu[12+i].id, menu[12+i].name, menu[12+i].price, menu[12+i].stock);

        // Cột Phải: Giải khát (index 20 -> 24)
        if (i < 5) sprintf(right, " %2d.%-20.20s %9.0lf (K:%-3d)", menu[20+i].id, menu[20+i].name, menu[20+i].price, menu[20+i].stock);
        else strcpy(right, "");

        printf("| %-42s | %-42s | %-42s |\n", left, mid, right);
    }
    printLine();
}
#include "welcome_ui.h"
#include <stdio.h>
#include "../utils/helper.h"

void printFrameLine(const char *frame) {
    printf(CYAN "%s" RESET "\n", frame);
}

// Dòng có khung cyan + text đổi màu ở giữa
void printContentLine(const char *textColor, const char *text) {
    printf(CYAN "  ║" RESET "%s%-64s" RESET CYAN "║" RESET "\n", textColor, text);
}

void printWelcomeScreen() {
    printFrameLine("  ╔═════════════════════════════════════════════════════════════════╗");
    printFrameLine("  ║                                                                 ║");
    printContentLine(YELLOW_BOLD, "              PBL1: ĐỒ ÁN LẬP TRÌNH TÍNH TOÁN                    ");
    printFrameLine("  ╠═════════════════════════════════════════════════════════════════╣");
    printContentLine(WHITE,       "                                                                 ");
    printContentLine(YELLOW_BOLD, "        TÊN ĐỀ TÀI: ĐẶT HÀNG VÀ THANH TOÁN MÓN ĂN                ");
    printContentLine(WHITE,       "                                                                 ");
    printFrameLine("  ╠═════════════════════════════════════════════════════════════════╣");
    printContentLine(WHITE,       "                                                                 ");
    printContentLine(WHITE,       "  Giáo viên hướng dẫn:                                           ");
    printContentLine(WHITE,       "       TRƯƠNG NGỌC CHÂU                                          ");
    printContentLine(WHITE,       "  Sinh viên thực hiện:                                           ");
    printContentLine(WHITE,       "       NGUYỄN ĐÌNH DUY HOÀNG    2HT_Nhật 1    MSSV: 102250222    ");
    printContentLine(WHITE,       "       THÁI NGUYỄN ANH KIỆT     2HT_Nhật 1    MSSV: 102250233    ");
    printContentLine(WHITE,       "                                                                 ");
    printFrameLine("  ╚═════════════════════════════════════════════════════════════════╝");
}

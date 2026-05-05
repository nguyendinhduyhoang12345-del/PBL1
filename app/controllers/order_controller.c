#include <stdio.h>
#include <string.h>
#include "../models/models.h"
#include "../utils/validator.h"
#include "../ui/menu_ui.h"
#include "../ui/cart_ui.h"
#include "order_controller.h"


void addToCart(Bill *currentBill) {
    int idNhap, slNhap;

    while (1) {
        int countMain = 0;
        for (int i = 0; i < currentBill->itemCount; i++) {
            if (currentBill->items[i].id <= 12) {
                countMain += currentBill->items[i].quantity;
            }
        }
        
        if (currentBill->itemCount >= MAX_GIO_HANG) {
            printf("\n[!] Gio hang da day! Vui long thanh toan.\n");
            break;
        }

        printf("\n--- DAT MON ---");
        printf("\n>> Nhap Ma Mon (Mon chinh: %d/5 | Mon them & Nuoc: Khong gioi han) : ", countMain);
        idNhap = getValidInt("");

        if (idNhap == 0) break;
        if (idNhap < 1 || idNhap > TONG_SO_MON) {
            printf("  !! Loi: Ma mon ko hop le!\n");
            continue;
        }

        int idx = idNhap - 1;

        // --- LOGIC KIỂM TRA 5 MÓN CHÍNH (Theo Flowchart) ---
        if (idNhap <= 12 && countMain >= 5) {
            printf("  !! Thong bao: Ban da dat du 5 mon chinh (Gioi han 5).\n");
            continue;
        }

        slNhap = getValidInt(">> Nhap so luong: ");
        if (slNhap <= 0 || slNhap > menu[idx].stock) {
            printf("  !! Loi: Kho ko du hoac SL ko hop le (Con: %d)\n", menu[idx].stock);
            continue;
        }

        // Xử lý tùy chọn sườn (Nếu có)
        int opt = 0;
        double giaThucTe = menu[idx].price;
        if (menu[idx].hasOptions) {
            printf("  1. Cot let | 2. Suon cay (+10k)\n");
            opt = getValidInt("  >> Chon loai: ");
            if (opt == 2) giaThucTe += 10000;
        }

        // Ghi chú
        char ghiChu[100] = "";
        printf("  >> Ghi chu (Bam Enter bo qua): ");
        fgets(ghiChu, sizeof(ghiChu), stdin);
        ghiChu[strcspn(ghiChu, "\n")] = 0;

        // Đưa vào giỏ hàng
        int pos = currentBill->itemCount;
        currentBill->items[pos].id = idNhap;
        strcpy(currentBill->items[pos].name, menu[idx].name);
        currentBill->items[pos].price = giaThucTe;
        currentBill->items[pos].quantity = slNhap;
        currentBill->items[pos].option = opt;
        strcpy(currentBill->items[pos].note, ghiChu);
        currentBill->items[pos].totalPrice = giaThucTe * slNhap;

        menu[idx].stock -= slNhap; // Cập nhật tồn kho RAM
        currentBill->itemCount++;

        printf("  => Da them %d '%s'", slNhap, menu[idx].name);

        // Nếu món có option thì in thêm loại sườn
        if (menu[idx].hasOptions) {
            if (opt == 1) printf(" (Cot let)");
            else printf(" (Suon cay)");
        }

        printf(" vao gio!\n");
    }
}

void removeFromCart(Bill *currentBill) {
    if (currentBill->itemCount == 0) return;
    
    int stt = getValidInt(">> Nhap STT mon muon xoa: ");
    if (stt < 1 || stt > currentBill->itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    int idx = stt - 1;
    int menuIdx = currentBill->items[idx].id - 1;

    // Hoàn trả tồn kho
    menu[menuIdx].stock += currentBill->items[idx].quantity;

    // Dịch chuyển mảng để xóa
    for (int i = idx; i < currentBill->itemCount - 1; i++) {
        currentBill->items[i] = currentBill->items[i + 1];
    }
    currentBill->itemCount--;
    printf("  => Da xoa mon va hoan lai ton kho!\n");
}

void updateQuantity(Bill *currentBill) {
    if (currentBill->itemCount == 0) return;

    int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
    if (stt < 1 || stt > currentBill->itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    int idx = stt - 1;
    int menuIdx = currentBill->items[idx].id - 1;
    int oldQty = currentBill->items[idx].quantity;
    
    int newQty = getValidInt(">> Nhap so luong moi: ");
    if (newQty <= 0) {
        printf("  !! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.\n");
        return;
    }

    // Kiểm tra nếu là món chính thì tổng sau khi sửa không được > 5
    if (currentBill->items[idx].id <= 12) {
        int countMainExceptThis = 0;
        for (int i = 0; i < currentBill->itemCount; i++) {
            if (i != idx && currentBill->items[i].id <= 12) 
                countMainExceptThis += currentBill->items[i].quantity;
        }
        if (countMainExceptThis + newQty > 5) {
            printf("  !! Loi: Sua SL khien tong mon chinh > 5 (Hien tai cac mon khac da co: %d).\n", countMainExceptThis);
            return;
        }
    }

    // Kiểm tra tồn kho
    int diff = newQty - oldQty;
    if (diff > menu[menuIdx].stock) {
        printf("  !! Loi: Kho khong du hang (Con: %d).\n", menu[menuIdx].stock);
        return;
    }

    // Cập nhật
    menu[menuIdx].stock -= diff;
    currentBill->items[idx].quantity = newQty;
    currentBill->items[idx].totalPrice = newQty * currentBill->items[idx].price;
    printf("  => Da cap nhat so luong!\n");
}


//Dành cho case 1
void handleOrderMenu(Bill* currentBill) {
    
    showMenuUI();      // Gọi hàm hiển thị menu
    addToCart(currentBill); // Gọi hàm nhập ID và thêm vào giỏ mà bạn đã có sẵn
}

// Dành cho case 2
void handleCartMenu(Bill* currentBill) {
    while (1) {
        
        showCartUI(currentBill); // Hiển thị trạng thái giỏ hàng hiện tại
        
        printf("\n[ THAO TAC GIO HANG ]\n");
        printf("1. Xoa mon khoi gio\n");
        printf("2. Thay doi so luong\n");
        printf("0. Quay lai Menu chinh\n");
        
        int subChoice = getValidInt("-> Chon: ");

        if (subChoice == 1) {
            removeFromCart(currentBill); // Gọi hàm xóa bạn đã có
        } 
        else if (subChoice == 2) {
            updateQuantity(currentBill); // Gọi hàm cập nhật bạn đã có
        } 
        else if (subChoice == 0) {
            break; // Thoát vòng lặp để về main
        }
    }
}
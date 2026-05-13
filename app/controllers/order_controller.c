#include <stdio.h>
#include <string.h>
#include "../models/models.h"
#include "../utils/validator.h"
#include "../ui/menu_ui.h"
#include "../ui/cart_ui.h"
#include "order_controller.h"
#include "../services/hash_service.h"
#include "../services/cart_service.h"

void addToCart(Bill *currentBill, OrderItem item) {
    CartNode *node = currentBill->cart.head;
    while (node) {
        if (node->item.id == item.id &&
            node->item.option == item.option &&
            strcmp(node->item.note, item.note) == 0) {
            node->item.quantity += item.quantity;
            node->item.totalPrice = node->item.quantity * node->item.price;
            return;
        }
        node = node->next;
    }
    cartAddItem(&currentBill->cart, item);
}

void removeFromCart(Bill *currentBill) {
    if (currentBill->cart.itemCount == 0) return;
    int stt = getValidInt(">> Nhap STT mon muon xoa: ");
    if (stt < 1 || stt > currentBill->cart.itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    if (!node) {
        printf("  !! Loi: Khong tim thay mon de xoa.\n");
        return;
    }
    MenuItem *menuItem = hashTableSearch(menuHashTable, node->item.id);
    if (menuItem) menuItem->stock += node->item.quantity; // Hoàn lại tồn kho
    cartRemoveItemByIndex(&currentBill->cart, idx);
    printf("  => Da xoa mon va hoan lai ton kho!\n");
}

void updateQuantity(Bill *currentBill) {
    if (currentBill->cart.itemCount == 0) return;

    int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
    if (stt < 1 || stt > currentBill->cart.itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    int idx = stt - 1;
    CartNode *node = cartGetNodeByIndex(&currentBill->cart, idx);
    if (!node) {
        printf("  !! Loi: Khong tim thay mon de sua.\n");
        return;
    }
    
    int newQty = getValidInt(">> Nhap so luong moi: ");
    if (newQty <= 0) {
        printf("  !! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.\n");
        return;
    }

    MenuItem *menuItem = hashTableSearch(menuHashTable, node->item.id);
    if (!menuItem) {
        printf("  !! Loi: Khong tim thay mon trong menu.\n");
        return;
    }
    // Kiểm tra nếu là món chính thì tổng sau khi sửa không được > 5
    int oldQty = node->item.quantity;
    int menuIdx = menuItem->id - 1;
    if (menuIdx < 12) { // nếu là món chính
        int countMain = 0;
        CartNode *tmp = currentBill->cart.head;
        while (tmp) {
            if (tmp->item.id <= 12) countMain += tmp->item.quantity;
            tmp = tmp->next;
        }
        countMain = countMain - oldQty + newQty; // trừ đi số lượng cũ và cộng số lượng mới
        if (countMain > 5) {
            printf("  !! Loi: Tong so mon chinh khong duoc vuot qua 5 (Dang co: %d).\n", countMain);
            return;
        }
    }

    // Kiểm tra tồn kho
    int diff = newQty - oldQty;
    if (diff > 0 && diff > menuItem->stock) {
        printf("  !! Loi: Kho khong du hang (Con: %d).\n", menuItem->stock);
        return;
    }

    // Cập nhật
    if (menuItem) menuItem->stock -= diff; // Cập nhật tồn kho
    cartUpdateQuantityByIndex(&currentBill->cart, idx, newQty);
    printf("  => Da cap nhat so luong!\n");
}


//Dành cho case 1
void handleOrderMenu(Bill* currentBill) {
    showMenuUI(); // Hiển thị menu món ăn
    while (1) {
        int countMain = 0;
        CartNode *tmp = currentBill->cart.head;
        while (tmp) {
            if (tmp->item.id <= 12) countMain += tmp->item.quantity;
            tmp = tmp->next;
        }

        printf("\n--- DAT MON ---");
        printf("\n>>(Mon chinh: %d/5 | Mon them & Nuoc: Khong gioi han, Nhap 0 de thoat) : ", countMain);
        int idNhap = getValidInt("\n>> Nhap Ma Mon: ");
        if (idNhap == 0) break;

        MenuItem *menuItem = hashTableSearch(menuHashTable, idNhap);
        if (!menuItem) {
            printf("  !! Loi: Ma mon ko hop le!\n");
            continue;
        }

        if (idNhap < 1 || idNhap > TONG_SO_MON) {
            printf("  !! Loi: Ma mon ko hop le!\n");
            continue;
        }
        if (idNhap <= 12 && countMain >= 5) {
            printf("  !! Thong bao: Ban da dat du 5 mon chinh.\n");
            continue;
        }
         
        int slNhap = getValidInt(">> Nhap so luong: ");
        if (slNhap <= 0 || slNhap > menuItem->stock || slNhap > 5) {
            printf("  !! Loi: Kho ko du hoac SL ko hop le (Con: %d)\n", menuItem->stock);
            continue;
        }
        if (idNhap <= 12 && countMain + slNhap > 5) {
            printf("  !! Loi: Tong so mon chinh khong duoc vuot qua 5 (Dang co: %d).\n", countMain);
            continue;
        }

        // Xử lý tùy chọn sườn (Nếu có)
        int opt = 0;
        double giaThucTe = menuItem->price;
        if (menuItem->hasOptions) {
            while (1) {
                printf("  1. Cot let | 2. Suon cay (+10k)\n");
                opt = getValidInt("  >> Chon loai: ");
                if (opt == 1 || opt == 2) {
                    if (opt == 2) giaThucTe += 10000;
                    break;
                } else {
                    printf("  !! Loi: Chi chon 1 hoac 2.\n");
                }
            }
        }

        // Ghi chú
        char ghiChu[100] = "";
        printf("  >> Ghi chu (Bam Enter bo qua): ");
        fgets(ghiChu, sizeof(ghiChu), stdin);
        ghiChu[strcspn(ghiChu, "\n")] = 0;

        OrderItem newItem;
        newItem.id = menuItem->id;
        strcpy(newItem.name, menuItem->name);
        newItem.price = giaThucTe;
        newItem.quantity = slNhap;
        newItem.option = opt;
        strcpy(newItem.note, ghiChu);
        newItem.totalPrice = giaThucTe * slNhap;

        printf("  => Da them %d '%s'", slNhap, menuItem->name);

        if (menuItem->hasOptions) {
            if (opt == 1) printf(" (Cot let)");
            else printf(" (Suon cay)");
        }

        printf(" vao gio!\n");
        addToCart(currentBill, newItem);
        menuItem->stock -= slNhap;
    }
    
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
        else {
            printf("[!] Lua chon khong hop le.\n");
        }
    }
}
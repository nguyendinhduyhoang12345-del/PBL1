#include <stdio.h>
#include <string.h>
<<<<<<< HEAD
=======
#include <stdlib.h>
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
#include "../models/models.h"
#include "../utils/validator.h"
#include "../ui/menu_ui.h"
#include "../ui/cart_ui.h"
#include "order_controller.h"
<<<<<<< HEAD
#include "../services/hash_service.h"
#include "../services/cart_service.h"

CartNode *findCartNodeById(Cart *cart, int itemId) {
    CartNode *node = cart->head;
    while (node) {
        if (node->item.id == itemId) return node;
        node = node->next;
    }
    return NULL;
}

void addToCart(Bill *currentBill, OrderItem item) {
    CartNode *existing = findCartNodeById(&currentBill->cart, item.id);
    if (existing) {
        existing->item.quantity += item.quantity;
        existing->item.totalPrice = existing->item.quantity * existing->item.price;
        return;
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
=======
#include "../services/hash_table_service.h"
#include "../services/doubly_linked_list_service.h"
#include "../services/menu_hash_service.h"


void addToCart(Bill *currentBill) {
    int idNhap, slNhap;

    while (1) {
        int countMain = 0;
        // Đếm số món chính từ DLL
        DLLNode* node = currentBill->cart->head;
        while (node) {
            if (node->item.id <= 12) {
                countMain += node->item.quantity;
            }
            node = node->next;
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
        }

        printf("\n--- DAT MON ---");
        printf("\n>>(Mon chinh: %d/5 | Mon them & Nuoc: Khong gioi han, Nhap 0 de thoat) : ", countMain);
<<<<<<< HEAD
        int idNhap = getValidInt("\n>> Nhap Ma Mon: ");
        if (idNhap == 0) break;

        MenuItem *menuItem = hashTableSearch(menuHashTable, idNhap);
        if (!menuItem) {
            printf("  !! Loi: Ma mon ko hop le!\n");
            continue;
        }

=======
        idNhap = getValidInt("\n>> Nhap Ma Mon: ");

        if (idNhap == 0) break;
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
        if (idNhap < 1 || idNhap > TONG_SO_MON) {
            printf("  !! Loi: Ma mon ko hop le!\n");
            continue;
        }
<<<<<<< HEAD
        if (idNhap <= 12 && countMain >= 5) {
            printf("  !! Thong bao: Ban da dat du 5 mon chinh.\n");
            continue;
        }
         
        int slNhap = getValidInt(">> Nhap so luong: ");
=======
         
        // Lấy MenuItem từ hash table
        MenuItem* menuItem = getMenuItemFromHash(menuHash, idNhap);
        if (!menuItem) {
            printf("  !! Loi: Khong tim thay mon an!\n");
            continue;
        }

        if (idNhap <= 12 && countMain >= 5) {
            printf("  !! Thong bao: Ban da dat du 5 mon chinh (Gioi han 5).\n");
            continue;
        }

        slNhap = getValidInt(">> Nhap so luong: ");
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
        if (slNhap <= 0 || slNhap > menuItem->stock) {
            printf("  !! Loi: Kho ko du hoac SL ko hop le (Con: %d)\n", menuItem->stock);
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

<<<<<<< HEAD
        OrderItem newItem;
        newItem.id = menuItem->id;
        strcpy(newItem.name, menuItem->name);
        newItem.price = giaThucTe;
        newItem.quantity = slNhap;
        newItem.option = opt;
        strcpy(newItem.note, ghiChu);
        newItem.totalPrice = giaThucTe * slNhap;

        printf("  => Da them %d '%s'", slNhap, menuItem->name);

=======
        // Tạo OrderItem
        OrderItem item;
        item.id = idNhap;
        strcpy(item.name, menuItem->name);
        item.price = giaThucTe;
        item.quantity = slNhap;
        item.option = opt;
        strcpy(item.note, ghiChu);
        item.totalPrice = giaThucTe * slNhap;

        // Thêm vào DLL
        dllAppend(currentBill->cart, &item);
        currentBill->itemCount = dllGetCount(currentBill->cart);

        // Giảm stock (có thể update trong hash table nếu cần)
        menuItem->stock -= slNhap;
        printf("  => Da them %d '%s'", slNhap, menuItem->name);
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
        if (menuItem->hasOptions) {
            if (opt == 1) printf(" (Cot let)");
            else printf(" (Suon cay)");
        }
<<<<<<< HEAD

        printf(" vao gio!\n");
        addToCart(currentBill, newItem);
        menuItem->stock -= slNhap;
    }
    
=======
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

    DLLNode* node = dllGetNodeAt(currentBill->cart, stt);
    if (!node) {
        printf("  !! Loi: Khong tim thay mon de xoa.\n");
        return;
    }
    int itemId = node->item.id;
    int qty = node->item.quantity;

    dllRemoveAt(currentBill->cart, stt);
    currentBill->itemCount = dllGetCount(currentBill->cart);

    // Hoàn lại stock
    MenuItem* menuItem = getMenuItemFromHash(menuHash, itemId);
    if (menuItem) {
        menuItem->stock += qty;
    }
    printf("  => Da xoa mon va hoan lai ton kho!\n");
}

void updateQuantity(Bill *currentBill) {
    if (currentBill->itemCount == 0) return;

    int stt = getValidInt(">> Nhap STT mon muon sua SL: ");
    if (stt < 1 || stt > currentBill->itemCount) {
        printf("  !! STT khong hop le.\n");
        return;
    }

    DLLNode* node = dllGetNodeAt(currentBill->cart, stt);
    if (!node) {
        printf("  !! Loi: Khong tim thay item!\n");
        return;
    }

    int itemId = node->item.id;
    int oldQty = node->item.quantity;   

    int newQty = getValidInt(">> Nhap so luong moi: ");
    if (newQty <= 0) {
        printf("  !! So luong phai > 0. Muon xoa hay chon chuc nang Xoa.\n");
        return;
    }

    // Kiểm tra nếu là món chính thì tổng sau khi sửa không được > 5
    if (itemId <= 12) {
        int countMainExceptThis = 0;
        // Đếm từ DLL
        DLLNode* n = currentBill->cart->head;
        while (n) {
            if (n->item.id <= 12 && n->item.id != itemId) {
                countMainExceptThis += n->item.quantity;
            }
            n = n->next;
        }
        if (countMainExceptThis + newQty > 5) {
            printf("  !! Loi: Sua SL khien tong mon chinh > 5.\n");
            return;
        }
    }

    // Kiểm tra tồn kho
    MenuItem* menuItem = getMenuItemFromHash(menuHash, itemId);
    if (!menuItem) {
        printf("  !! Loi: Khong tim thay mon!\n");
        return;
    }
    int diff = newQty - oldQty;
    if (diff > menuItem->stock) {
        printf("  !! Loi: Kho khong du hang (Con: %d).\n", menuItem->stock);
        return;
    }

    // Cập nhật
    dllUpdateQuantity(currentBill->cart, stt, newQty);
    menuItem->stock -= diff;
    printf("  => Da cap nhat so luong!\n");
}


//Dành cho case 1
void handleOrderMenu(Bill* currentBill) {
    if (!currentBill->cart) {
        currentBill->cart = dllCreate();
    }
    showMenuUI();      // Gọi hàm hiển thị menu
    addToCart(currentBill); // Gọi hàm nhập ID và thêm vào giỏ mà bạn đã có sẵn
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
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
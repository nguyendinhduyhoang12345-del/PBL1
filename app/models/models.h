#ifndef MODELS_H
#define MODELS_H

#define MAX_DON_HANG 100
#define MAX_GIO_HANG 30
#define TONG_SO_MON 25

// Cấu trúc thực thể cơ bản
typedef struct {
    int id;
    char name[50];
    double price;
    int stock;
    int hasOptions;
} MenuItem;

typedef struct {
    int id;
    char name[50];
    double price;
    int quantity;
    int option;
    char note[100];
    double totalPrice;
} OrderItem;

typedef struct {
    int id;
    char phone[15];
    char name[50];
    char address[100];
    double totalSpent; // Tổng tiền đã chi tiêu ở quán
    char rank[20];     // Hạng thành viên (Mới, Bạc, Vàng, VIP)
} Customer;

typedef struct {
    int id;
    Customer customer;
    OrderItem items[MAX_GIO_HANG];
    int itemCount;
    double total;
    double discount;
    double finalPrice;
    char dateTime[50];
} Bill;

typedef struct {
    int customerId;
    int billCount;       // Số hóa đơn của khách
    double revenue;      // Tổng doanh thu của khách
} CustomerRevenue;

// CTDL động
// danh sách liên kết quản lý lịch sử hóa đơn
typedef struct HistoryNode {
    int customerId;        // ID khách hàng (để dễ tra cứu lịch sử từng khách)
    Bill bill;             // Toàn bộ thông tin hóa đơn (items, dateTime, total, ...)
    struct HistoryNode* next;
} HistoryNode;

#define BTREE_ORDER 4
typedef struct BTreeNode {
    Customer* customers[BTREE_ORDER - 1];
    struct BTreeNode* children[BTREE_ORDER];
    int numKeys;
    int isLeaf;
} BTreeNode;

extern MenuItem menu[TONG_SO_MON];
extern Customer danhSachKH[1000];
extern int soLuongKH;
extern BTreeNode* btreeRoot;
extern HistoryNode* historyHead;

#endif
#ifndef MODELS_H
#define MODELS_H

#define MAX_DON_HANG 100
#define TONG_SO_MON 25

#define RANK_BRONZE "Bronze"
#define RANK_SILVER "Silver"
#define RANK_GOLD "Gold"
#define RANK_DIAMOND "Diamond"

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

typedef struct HashTable HashTable;
typedef struct DoublyLinkedList DoublyLinkedList;

typedef struct {
    int id;
    Customer customer;
    DoublyLinkedList* cart;  // Thay thế itemsHash và cartDisplay
    OrderItem* items;        // Giữ để tương thích history
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

#define HASH_TABLE_SIZE 31

typedef struct HashNode {
    int itemId;
    OrderItem item;
    struct HashNode* next;
} HashNode;

typedef struct HashTable {
    HashNode** table;
    int size;
    int count;
} HashTable;

typedef struct DLLNode {
    OrderItem item;
    int position;
    struct DLLNode* next;
    struct DLLNode* prev;
} DLLNode;

typedef struct DoublyLinkedList {
    DLLNode* head; 
    DLLNode* tail;
    int count;
} DoublyLinkedList;

extern MenuItem menu[TONG_SO_MON];
extern Customer danhSachKH[1000];
extern int soLuongKH;
extern BTreeNode* btreeRoot;
extern HistoryNode* historyHead;


`n`n#endif

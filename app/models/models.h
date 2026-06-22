#ifndef MODELS_H
#define MODELS_H

#define MAX_DON_HANG 100
#define MAX_GIO_HANG 30
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

typedef struct CartNode {
    OrderItem item;
    struct CartNode *prev;
    struct CartNode *next;
} CartNode;

typedef struct {
    CartNode *head;
    CartNode *tail;
    int itemCount;
} Cart;

typedef struct {
    int id;
    Customer customer;
    Cart cart;
    double total;
    double discount;
    double finalPrice;
    char dateTime[50];
} Bill;

typedef struct {
    int customerId;
    int billCount;       
    double revenue;
} CustomerRevenue;


typedef struct HistoryNode {
    int customerId;
    Bill bill;
    struct HistoryNode* next;
} HistoryNode;

#define BTREE_ORDER 4
typedef struct BTreeNode {
    Customer* customers[BTREE_ORDER - 1];
    struct BTreeNode* children[BTREE_ORDER];
    int numKeys;
    int isLeaf;
} BTreeNode;

#define HASH_TABLE_SIZE 101

typedef struct HashNode {
    int key;
    MenuItem item;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode **table;
    int size;
} HashTable;


extern MenuItem menu[TONG_SO_MON];
extern Customer danhSachKH[1000];
extern int soLuongKH;
extern BTreeNode* btreeRoot;
extern HistoryNode* historyHead;

extern HashTable* menuHashTable;
extern Cart cart;


#endif
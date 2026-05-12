#ifndef MODELS_H
#define MODELS_H

#define MAX_DON_HANG 100
<<<<<<< HEAD
#define MAX_GIO_HANG 30
=======
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
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

<<<<<<< HEAD
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
=======
typedef struct HashTable HashTable;
typedef struct DoublyLinkedList DoublyLinkedList;
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

typedef struct {
    int id;
    Customer customer;
<<<<<<< HEAD
    Cart cart;
=======
    DoublyLinkedList* cart;  // Thay thế itemsHash và cartDisplay
    OrderItem* items;        // Giữ để tương thích history
    int itemCount;
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247
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

<<<<<<< HEAD
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

=======
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
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

extern MenuItem menu[TONG_SO_MON];
extern Customer danhSachKH[1000];
extern int soLuongKH;
extern BTreeNode* btreeRoot;
extern HistoryNode* historyHead;

<<<<<<< HEAD
extern HashTable* menuHashTable;
extern Cart cart;


#endif
=======

`n`n#endif
>>>>>>> 18329fa10e66c70c2d31276e6e4db4bd7d213247

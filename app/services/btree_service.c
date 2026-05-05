#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree_service.h"

BTreeNode* createBTreeNode(int isLeaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->isLeaf = isLeaf;
    newNode->numKeys = 0;
    for (int i = 0; i < BTREE_ORDER; i++) newNode->children[i] = NULL;
    return newNode;
}

Customer* searchBTree(BTreeNode* root, char* phone) {
    if (root == NULL) return NULL;
    int i = 0;
    while (i < root->numKeys && strcmp(phone, root->customers[i]->phone) > 0) i++;
    if (i < root->numKeys && strcmp(phone, root->customers[i]->phone) == 0)
        return root->customers[i];
    if (root->isLeaf) return NULL;
    return searchBTree(root->children[i], phone);
}

void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
    BTreeNode* z = createBTreeNode(child->isLeaf);
    z->numKeys = 1; // Với BTREE_ORDER = 4, node mới sẽ lấy 1 khóa từ phần tử cuối
    z->customers[0] = child->customers[2]; // Lấy phần tử cuối cùng của child

    if (!child->isLeaf) {
        z->children[0] = child->children[2];
        z->children[1] = child->children[3];
    }
    child->numKeys = 1;

    for (int j = parent->numKeys; j >= i + 1; j--) parent->children[j+1] = parent->children[j];
    parent->children[i+1] = z;

    for (int j = parent->numKeys - 1; j >= i; j--) parent->customers[j+1] = parent->customers[j];
    parent->customers[i] = child->customers[1]; // Đẩy phần tử giữa lên làm khóa parent
    parent->numKeys++;
}

void insertNonFull(BTreeNode* node, Customer* cust) {
    int i = node->numKeys - 1;
    if (node->isLeaf) {
        while (i >= 0 && strcmp(cust->phone, node->customers[i]->phone) < 0) {
            node->customers[i+1] = node->customers[i];
            i--;
        }
        node->customers[i+1] = cust;
        node->numKeys++;
    } else {
        while (i >= 0 && strcmp(cust->phone, node->customers[i]->phone) < 0) i--;
        if (node->children[i+1]->numKeys == BTREE_ORDER - 1) {
            splitChild(node, i + 1, node->children[i+1]);
            if (strcmp(cust->phone, node->customers[i+1]->phone) > 0) i++;
        }
        insertNonFull(node->children[i+1], cust);
    }
}

void insertBTree(BTreeNode** root, Customer* cust) {
    if (*root == NULL) {
        *root = createBTreeNode(1);
        (*root)->customers[0] = cust;
        (*root)->numKeys = 1;
    } else {
        if ((*root)->numKeys == BTREE_ORDER - 1) {
            BTreeNode* s = createBTreeNode(0);
            s->children[0] = *root;
            splitChild(s, 0, *root);
            int i = 0;
            if (strcmp(cust->phone, s->customers[0]->phone) > 0) i++;
            insertNonFull(s->children[i], cust);
            *root = s;
        } else {
            insertNonFull(*root, cust);
        }
    }
}
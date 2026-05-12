#ifndef BTREE_SERVICE_H
#define BTREE_SERVICE_H
#include "../models/models.h"

BTreeNode* createBTreeNode(int isLeaf);
void splitChild(BTreeNode* parent, int i, BTreeNode* child);
void insertNonFull(BTreeNode* node, Customer* cust);
void insertBTree(BTreeNode** root, Customer* cust);
Customer* searchBTree(BTreeNode* root, char* phone);

#endif
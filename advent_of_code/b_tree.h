#pragma once


typedef struct _BNode {
	char* id;
	struct _BNode* left;
	struct _BNode* right;
} BNode;

typedef struct {
	BNode* head;
} BTree;


void init_btree(BTree* tree);

#pragma once
#include "sources.h"

typedef struct _QNode {
	llu data;
	struct _QNode* next;
} QNode;

typedef struct {
	int size;
	QNode* head;
	QNode* tail;
} Queue;


void init_queue(Queue* q);
void push_queue(Queue* q, llu value);
llu pop_queue(Queue* q);
llu peek_queue(Queue* q);
llu peek_index_queue(Queue* q, int i);
int empty_queue(Queue* q);
void concat_queue(Queue* q1, Queue* q2);
void destroy_queue(Queue* q);
void print_queue(Queue* q);


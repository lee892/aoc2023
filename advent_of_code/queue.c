
#include "queue.h"
#include "sources.h"

void init_queue(Queue* q) {
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
}


llu pop_queue(Queue* q) {
	if (q->head == NULL) {
		printf("Error: pop() on empty queue\n");
		return NULL;
	}
	QNode* temp = q->head;
	q->head = q->head->next;
	void* data = temp->data;
	if (q->tail == temp) q->tail = NULL;

	free(temp);
	q->size--;
	return data;
}


void push_queue(Queue* q, llu data) {
	if (q->head != NULL && q->tail == NULL) {
		printf("Error: queue head and tail are mismatched\n");
	}

	QNode* temp = (QNode*)malloc(sizeof(QNode));
	temp->data = data;
	temp->next = NULL;
	if (q->head == NULL) {
		q->head = temp;
		q->tail = q->head;
	} else {
		q->tail->next = temp;
		q->tail = temp;
	}
	q->size++;
}

llu peek_queue(Queue* q) {
	if (q->head == NULL) return NULL;
	return q->head->data;
}

llu peek_index_queue(Queue* q, int index) {
	if (index >= q->size || q->head == NULL) {
		printf("Error: attempt to peek invalid index\n");
		return 0;
	}
	int i = 0;
	QNode* curr = q->head;

	while (i < index) {
		curr = curr->next;
		i++;
	}
	return curr->data;
}

int empty_queue(Queue* q) {
	return q->size == 0;
}

void concat_queue(Queue* q1, Queue* q2) {
	if (q2->size == 0) {
		//printf("Error: queue 2 is empty\n");
		return;
	}
	QNode* curr = q2->head;
	assert(q2->head != NULL);
	while (curr != NULL) {
		push_queue(q1, curr->data);
		curr = curr->next;
	}
}

void destroy_queue(Queue* q) {
	QNode* curr = q->head;
	QNode* temp = NULL;
	while (curr) {
		temp = curr;
		curr = curr->next;
		free(temp);
		q->size--;
	}
	assert(q->size == 0);
	q->head = NULL;
	q->tail = NULL;
}

void print_queue(Queue* q) {
	QNode* curr = q->head;
	
	while (curr != NULL) {
		printf("%p ", curr->data);
		curr = curr->next;
	}

}

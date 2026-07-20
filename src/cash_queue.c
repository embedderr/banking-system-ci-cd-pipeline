/**
 * @file cash_queue.c
 * @brief Implementation of the withdraw/deposit FIFO queue.
 */

#include "cash_queue.h"

#include <stdlib.h>

void cash_queue_init(CashQueue *q) {
    if (q == NULL) {
        return;
    }

    q->front = NULL;
    q->rear = NULL;
}

bool cash_queue_is_empty(const CashQueue *q) {
    if (q == NULL) {
        return true;
    }

    return (q->front == NULL) && (q->rear == NULL);
}

StatusCode cash_queue_enqueue(CashQueue *q, const CashRequestInput *input) {
    CashNode *new_node;

    if ((q == NULL) || (input == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    new_node = (CashNode *)malloc(sizeof(CashNode));
    if (new_node == NULL) {
        return STATUS_ERROR_ALLOC_FAILED;
    }

    new_node->data = *input;
    new_node->next = NULL;

    if (cash_queue_is_empty(q)) {
        new_node->prev = NULL;
        q->front = new_node;
        q->rear = new_node;
    } else {
        new_node->prev = q->rear;
        q->rear->next = new_node;
        q->rear = new_node;
    }

    return STATUS_OK;
}

StatusCode cash_queue_dequeue(CashQueue *q, CashRequestInput *out_processed) {
    CashNode *node_to_remove;

    if (q == NULL) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (cash_queue_is_empty(q)) {
        return STATUS_ERROR_QUEUE_EMPTY;
    }

    node_to_remove = q->front;

    if (out_processed != NULL) {
        *out_processed = node_to_remove->data;
    }

    if (q->front == q->rear) {
        /* Only one node in the queue. */
        q->front = NULL;
        q->rear = NULL;
    } else {
        q->front = q->front->next;
        q->front->prev = NULL;
    }

    free(node_to_remove);

    return STATUS_OK;
}

StatusCode cash_queue_peek_front(const CashQueue *q, CashRequestInput *out_front) {
    if ((q == NULL) || (out_front == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (cash_queue_is_empty(q)) {
        return STATUS_ERROR_QUEUE_EMPTY;
    }

    *out_front = q->front->data;

    return STATUS_OK;
}

void cash_queue_destroy(CashQueue *q) {
    CashNode *current;
    CashNode *next_node;

    if (q == NULL) {
        return;
    }

    current = q->front;
    while (current != NULL) {
        next_node = current->next;
        free(current);
        current = next_node;
    }

    q->front = NULL;
    q->rear = NULL;
}

const CashNode *cash_queue_get_front_node(const CashQueue *q) {
    if (q == NULL) {
        return NULL;
    }

    return q->front;
}

const CashNode *cash_queue_get_next_node(const CashNode *node) {
    if (node == NULL) {
        return NULL;
    }

    return node->next;
}

/**
 * @file bill_queue.c
 * @brief Implementation of the bill payment FIFO queue.
 */

#include "bill_queue.h"

#include <stdlib.h>

void bill_queue_init(BillQueue *q) {
    if (q == NULL) {
        return;
    }

    q->front = NULL;
    q->rear = NULL;
}

bool bill_queue_is_empty(const BillQueue *q) {
    if (q == NULL) {
        return true;
    }

    return (q->front == NULL) && (q->rear == NULL);
}

StatusCode bill_queue_enqueue(BillQueue *q, const BillRequestInput *input) {
    BillNode *new_node;

    if ((q == NULL) || (input == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    new_node = (BillNode *)malloc(sizeof(BillNode));
    if (new_node == NULL) {
        return STATUS_ERROR_ALLOC_FAILED;
    }

    new_node->data = *input;
    new_node->next = NULL;

    if (bill_queue_is_empty(q)) {
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

StatusCode bill_queue_dequeue(BillQueue *q, BillRequestInput *out_processed) {
    BillNode *node_to_remove;

    if (q == NULL) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (bill_queue_is_empty(q)) {
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

StatusCode bill_queue_peek_front(const BillQueue *q, BillRequestInput *out_front) {
    if ((q == NULL) || (out_front == NULL)) {
        return STATUS_ERROR_INVALID_INPUT;
    }

    if (bill_queue_is_empty(q)) {
        return STATUS_ERROR_QUEUE_EMPTY;
    }

    *out_front = q->front->data;

    return STATUS_OK;
}

void bill_queue_destroy(BillQueue *q) {
    BillNode *current;
    BillNode *next_node;

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

const BillNode *bill_queue_get_front_node(const BillQueue *q) {
    if (q == NULL) {
        return NULL;
    }

    return q->front;
}

const BillNode *bill_queue_get_next_node(const BillNode *node) {
    if (node == NULL) {
        return NULL;
    }

    return node->next;
}

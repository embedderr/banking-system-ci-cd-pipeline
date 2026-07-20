/**
 * @file bill_queue.h
 * @brief FIFO queue of bill payment requests (electricity, gas, internet).
 *
 * Per the I/O Separation Principle (docs/design/LLD.md, Section 1.2),
 * this module performs no console I/O. ui_screens collects a
 * BillRequestInput from the user and passes it to bill_queue_enqueue();
 * this module only stores, orders, and returns data.
 *
 * Traceability: satisfies SRS-018, SRS-019, SRS-020, SRS-021, SRS-022.
 */

#ifndef BILL_QUEUE_H
#define BILL_QUEUE_H

#include <stdbool.h>

#include "common.h"

/**
 * @brief Raw data for a single bill payment request, collected by
 *        ui_screens before being handed to bill_queue_enqueue().
 */
typedef struct {
    char bill_type[MAX_BILL_TYPE_LEN]; /**< e.g. "Electricity", "Gas", "Internet". */
    long invoice_number;               /**< Invoice number for the bill. */
    Money amount;                      /**< Amount due. */
} BillRequestInput;

/**
 * @brief One node in the bill payment queue's doubly linked list.
 */
typedef struct BillNode {
    BillRequestInput data; /**< The request stored at this node. */
    struct BillNode *next; /**< Next node toward the rear, or NULL. */
    struct BillNode *prev; /**< Previous node toward the front, or NULL. */
} BillNode;

/**
 * @brief A FIFO queue of bill payment requests.
 *
 * Must be initialized with bill_queue_init() before use, and released
 * with bill_queue_destroy() when no longer needed.
 */
typedef struct {
    BillNode *front; /**< Node at the front of the queue (next to be dequeued), or NULL if empty. */
    BillNode
        *rear; /**< Node at the rear of the queue (most recently enqueued), or NULL if empty. */
} BillQueue;

/**
 * @brief Initializes @p q to an empty queue.
 *
 * @param q  Non-NULL pointer to the queue to initialize.
 */
void bill_queue_init(BillQueue *q);

/**
 * @brief Checks whether @p q currently holds no requests.
 *
 * @param q  Non-NULL pointer to the queue to check.
 * @return true if the queue is empty (or @p q is NULL); false otherwise.
 */
bool bill_queue_is_empty(const BillQueue *q);

/**
 * @brief Adds a new bill payment request to the rear of the queue.
 *
 * @param q      Non-NULL pointer to the queue.
 * @param input  Non-NULL pointer to the request data to copy into the
 *               queue. The caller retains ownership of @p input itself;
 *               its contents are copied, not referenced.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_ALLOC_FAILED if memory allocation failed;
 *         STATUS_ERROR_INVALID_INPUT if @p q or @p input is NULL.
 */
StatusCode bill_queue_enqueue(BillQueue *q, const BillRequestInput *input);

/**
 * @brief Removes and returns the request at the front of the queue.
 *
 * @param q               Non-NULL pointer to the queue.
 * @param out_processed    Optional pointer to receive a copy of the
 *                          removed request's data. Pass NULL to discard it.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_QUEUE_EMPTY if the queue has no requests;
 *         STATUS_ERROR_INVALID_INPUT if @p q is NULL.
 */
StatusCode bill_queue_dequeue(BillQueue *q, BillRequestInput *out_processed);

/**
 * @brief Copies the request at the front of the queue without removing it.
 *
 * @param q          Non-NULL pointer to the queue.
 * @param out_front  Non-NULL pointer to receive a copy of the front
 *                   request's data.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_QUEUE_EMPTY if the queue has no requests;
 *         STATUS_ERROR_INVALID_INPUT if @p q or @p out_front is NULL.
 */
StatusCode bill_queue_peek_front(const BillQueue *q, BillRequestInput *out_front);

/**
 * @brief Frees every node currently in the queue and resets it to empty.
 *
 * Safe to call on an already-empty queue. After this call, @p q may be
 * reused via bill_queue_init() or discarded.
 *
 * @param q  Non-NULL pointer to the queue to destroy.
 */
void bill_queue_destroy(BillQueue *q);

/**
 * @brief Returns the node at the front of the queue, for read-only
 *        iteration by ui_screens (e.g. to display the entire queue).
 *
 * @param q  Non-NULL pointer to the queue.
 * @return Pointer to the front node, or NULL if the queue is empty or
 *         @p q is NULL.
 */
const BillNode *bill_queue_get_front_node(const BillQueue *q);

/**
 * @brief Returns the node following @p node in the queue, for read-only
 *        iteration.
 *
 * @param node  Pointer to the current node (as returned by
 *              bill_queue_get_front_node() or a previous call to this
 *              function).
 * @return Pointer to the next node, or NULL if @p node is the last node
 *         or is itself NULL.
 */
const BillNode *bill_queue_get_next_node(const BillNode *node);

#endif /* BILL_QUEUE_H */

/**
 * @file cash_queue.h
 * @brief FIFO queue of cash withdrawal/deposit requests.
 *
 * Per the I/O Separation Principle (docs/design/LLD.md, Section 1.2),
 * this module performs no console I/O. ui_screens collects a
 * CashRequestInput from the user and passes it to cash_queue_enqueue();
 * this module only stores, orders, and returns data.
 *
 * Applying a dequeued request's amount to the target account's balance
 * (SRS-030) is performed by main.c, which calls cash_queue_dequeue() to
 * retrieve the request, then calls the appropriate account module
 * function. This module has no dependency on the account module (see
 * docs/design/HLD.md, Section 2).
 *
 * Traceability: satisfies SRS-015, SRS-016, SRS-017, SRS-029, SRS-030.
 */

#ifndef CASH_QUEUE_H
#define CASH_QUEUE_H

#include <stdbool.h>

#include "common.h"

/**
 * @brief Distinguishes a withdrawal request from a deposit request.
 */
typedef enum {
    CASH_TXN_WITHDRAW, /**< Cash is to be withdrawn from the account. */
    CASH_TXN_DEPOSIT   /**< Cash is to be deposited into the account. */
} CashTransactionType;

/**
 * @brief Raw data for a single withdraw/deposit request, collected by
 *        ui_screens before being handed to cash_queue_enqueue().
 */
typedef struct {
    long account_number;      /**< Account the transaction applies to. */
    Money amount;             /**< Cash amount for the transaction. */
    CashTransactionType type; /**< Withdraw or deposit. */
} CashRequestInput;

/**
 * @brief One node in the cash queue's doubly linked list.
 */
typedef struct CashNode {
    CashRequestInput data; /**< The request stored at this node. */
    struct CashNode *next; /**< Next node toward the rear, or NULL. */
    struct CashNode *prev; /**< Previous node toward the front, or NULL. */
} CashNode;

/**
 * @brief A FIFO queue of withdraw/deposit requests.
 *
 * Must be initialized with cash_queue_init() before use, and released
 * with cash_queue_destroy() when no longer needed.
 */
typedef struct {
    CashNode *front; /**< Node at the front of the queue (next to be dequeued), or NULL if empty. */
    CashNode
        *rear; /**< Node at the rear of the queue (most recently enqueued), or NULL if empty. */
} CashQueue;

/**
 * @brief Initializes @p q to an empty queue.
 *
 * @param q  Non-NULL pointer to the queue to initialize.
 */
void cash_queue_init(CashQueue *q);

/**
 * @brief Checks whether @p q currently holds no requests.
 *
 * @param q  Non-NULL pointer to the queue to check.
 * @return true if the queue is empty (or @p q is NULL); false otherwise.
 */
bool cash_queue_is_empty(const CashQueue *q);

/**
 * @brief Adds a new withdraw/deposit request to the rear of the queue.
 *
 * @param q      Non-NULL pointer to the queue.
 * @param input  Non-NULL pointer to the request data to copy into the
 *               queue. The caller retains ownership of @p input itself;
 *               its contents are copied, not referenced.
 * @return STATUS_OK on success;
 *         STATUS_ERROR_ALLOC_FAILED if memory allocation failed;
 *         STATUS_ERROR_INVALID_INPUT if @p q or @p input is NULL.
 */
StatusCode cash_queue_enqueue(CashQueue *q, const CashRequestInput *input);

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
StatusCode cash_queue_dequeue(CashQueue *q, CashRequestInput *out_processed);

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
StatusCode cash_queue_peek_front(const CashQueue *q, CashRequestInput *out_front);

/**
 * @brief Frees every node currently in the queue and resets it to empty.
 *
 * Safe to call on an already-empty queue. After this call, @p q may be
 * reused via cash_queue_init() or discarded.
 *
 * @param q  Non-NULL pointer to the queue to destroy.
 */
void cash_queue_destroy(CashQueue *q);

/**
 * @brief Returns the node at the front of the queue, for read-only
 *        iteration by ui_screens (e.g. to display the entire queue).
 *
 * @param q  Non-NULL pointer to the queue.
 * @return Pointer to the front node, or NULL if the queue is empty or
 *         @p q is NULL.
 */
const CashNode *cash_queue_get_front_node(const CashQueue *q);

/**
 * @brief Returns the node following @p node in the queue, for read-only
 *        iteration.
 *
 * @param node  Pointer to the current node (as returned by
 *              cash_queue_get_front_node() or a previous call to this
 *              function).
 * @return Pointer to the next node, or NULL if @p node is the last node
 *         or is itself NULL.
 */
const CashNode *cash_queue_get_next_node(const CashNode *node);

#endif /* CASH_QUEUE_H */

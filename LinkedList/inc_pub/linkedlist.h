/**
 * @file linkedlist.h
 * @brief Public header for a doubly-linked list in ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This header provides the public interface for a doubly-linked list used in the
 * ft_nm project to manage symbol line data. It includes error codes, a node
 * structure for storing writer_line_t data, and functions for list manipulation.
 */

#ifndef _IG_LINKEDLIST_H_
#define _IG_LINKEDLIST_H_

#include "../../Writer/inc_pub/writer.h"  // For writer_line_t

/**
 * @brief Error codes for linked list operations
 */
enum LinkedList_Error {
    LL_SUCCESS = 0,            /**< Success */
    LL_ERR_NULL_INPUT = -1,    /**< Invalid input (NULL pointer) */
    LL_ERR_EMPTY_LIST = -2,    /**< List is empty */
    LL_ERR_MALLOC_FAIL = -3,   /**< Memory allocation failed */
    LL_ERR_NOT_FRONT = -4      /**< Head is not the first node */
};

/**
 * @brief Structure representing a node in the doubly-linked list
 */
typedef struct dl_list_s
{
    struct dl_list_s* prev; /**< Pointer to the previous node */
    struct dl_list_s* next; /**< Pointer to the next node */
    writer_line_t *line;    /**< Pointer to the symbol line data */
} dl_list_t;

/**
 * @brief Removes and returns the front node from the list
 * @param[in,out] head Pointer to the head of the list; updated to next node on success
 * @param[out] line Pointer to store the removed node's writer_line_t data
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT if head or line is NULL,
 *             LL_ERR_EMPTY_LIST if list is empty
 */
int LinkedList_nodePopFront(dl_list_t** head, writer_line_t** const line);

/**
 * @brief Adds a node with data to the front of the list
 * @param[in,out] head Pointer to the head of the list; updated to new node on success
 * @param[in] line Pointer to the writer_line_t data to add
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT if head is NULL,
 *             LL_ERR_MALLOC_FAIL if memory allocation fails
 */
int LinkedList_nodePushFront(dl_list_t** head, writer_line_t* const line);

/**
 * @brief Sorts the linked list using a comparison function
 * @param[in,out] head Pointer to the head of the list; updated to sorted head
 * @param[in] cmp Comparison function for writer_line_t elements; returns <0, 0, or >0
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT if head or cmp is NULL,
 *             LL_ERR_EMPTY_LIST if list is empty, LL_ERR_MALLOC_FAIL if allocation fails
 */
int LinkedList_sort(dl_list_t** head, int (*cmp)(const writer_line_t*, const writer_line_t*));

/**
 * @brief Deletes the entire linked list
 * @param[in,out] head Pointer to the head of the list; set to NULL on success
 * @param[in] del Optional function to free writer_line_t data; NULL if not needed
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT if head is NULL
 */
int LinkedList_delete(dl_list_t** head, void (*del)(void*));

#endif /* _IG_LINKEDLIST_H_ */

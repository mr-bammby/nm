/**
 * @file linkedlist.c
 * @brief Doubly linked list functions for ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 *
 * This file contains functions for managing a doubly linked list of
 * writer_line_t structures, used to sort and display symbol names in ft_nm.
 */

#include "../inc_pub/linkedlist.h"
#include <stdlib.h>

/**
 * @brief Removes the first node from the doubly linked list
 * @param[in,out] head Pointer to the head of the list
 * @param[out] line Pointer to store the removed writer_line_t data
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT on invalid input,
 *             LL_ERR_EMPTY_LIST if list is empty, LL_ERR_NOT_FRONT if head is not the first node
 */
int LinkedList_nodePopFront(dl_list_t** head, writer_line_t** const line)
{
    dl_list_t* curr_node;

    if ((line == NULL) || (head == NULL))
    {
        return LL_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (*head == NULL)
    {
        return LL_ERR_EMPTY_LIST;  // List is empty
    }
    if ((*head)->prev != NULL)
    {
        return LL_ERR_NOT_FRONT;   // Ensure head is the first node
    }
    curr_node = *head;
    *head = curr_node->next;
    if (*head != NULL)  // Check to avoid null pointer dereference
    {
        (*head)->prev = NULL;  // Update next node's prev pointer
    }
    *line = curr_node->line;   // Store removed node's data
    free(curr_node);           // Free the removed node
    return LL_SUCCESS;
}

/**
 * @brief Adds a new node to the front of the doubly linked list
 * @param[in,out] head Pointer to the head of the list
 * @param[in] line Pointer to the writer_line_t data to store in the new node
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT on invalid input or if head is not the first node,
 *             LL_ERR_MALLOC_FAIL on memory allocation failure
 */
int LinkedList_nodePushFront(dl_list_t** head, writer_line_t* const line)
{
    dl_list_t* new_node;

    if ((line == NULL) || (head == NULL))
    {
        return LL_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (*head != NULL)
    {
        if ((*head)->prev != NULL)
        {
            return LL_ERR_NOT_FRONT;  // Ensure head is the first node
        }
    }
    new_node = malloc(sizeof(dl_list_t));
    if (new_node == NULL)
    {
        return LL_ERR_MALLOC_FAIL;  // Memory allocation failure
    }
    new_node->line = line;        // Set new node's data
    new_node->prev = NULL;        // New node is the new head, so prev is NULL
    new_node->next = *head;       // Link new node to current head
    if (*head != NULL)
    {
        (*head)->prev = new_node;  // Update current head's prev pointer
    }
    *head = new_node;             // Update head to new node
    return LL_SUCCESS;
}

/**
 * @brief Deletes the entire doubly linked list and optionally frees the data
 * @param[in,out] head Pointer to the head of the list
 * @param[in] del Optional function pointer to free the writer_line_t data
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT on invalid input,
 *             LL_ERR_EMPTY_LIST if list is empty, LL_ERR_NOT_FRONT if head is not the first node
 */
int LinkedList_delete(dl_list_t** head, void (*del)(void*))
{
    dl_list_t* curr_node;
    dl_list_t* prev_node;

    if (head == NULL)
    {
        return LL_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (*head == NULL)
    {
        return LL_ERR_EMPTY_LIST;  // List is empty
    }
    if ((*head)->prev != NULL)
    {
        return LL_ERR_NOT_FRONT;   // Ensure head is the first node
    }
    curr_node = *head;
    *head = NULL;                 // Clear head to indicate empty list
    while (curr_node != NULL)
    {
        prev_node = curr_node;    // Store current node for freeing
        curr_node = curr_node->next;  // Move to next node
        if (del != NULL)          // Call user-provided function to free line data
        {
            del(prev_node->line);
        }
        free(prev_node);          // Free the current node
    }
    return LL_SUCCESS;
}

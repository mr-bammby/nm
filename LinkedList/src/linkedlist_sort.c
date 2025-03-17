/**
 * @file linkedlist_sort.c
 * @brief Sorting functions for doubly linked list in ft_nm
 * @author Domen Banfi
 * @date 2025-03-16
 * @version 1.0
 * @todo Implement a more efficient sorting algorithm (e.g., merge sort) for better performance with large lists
 *
 * This file contains functions for sorting a doubly linked list of
 * writer_line_t structures, used to order symbol names in ft_nm.
 * The sorting uses insertion sort for simplicity and stability,
 * preserving the order of equal elements as determined by the comparison function.
 */

#include "../inc_pub/linkedlist.h"
#include <stdlib.h>


/**
 * @brief Inserts a node into the sorted list at the correct position
 * @param[in,out] head Pointer to the head of the list
 * @param[in] inserting_node Node to be inserted
 * @param[in] next_node Node after which the insertion occurs
 */
static void insert_node(dl_list_t** head, dl_list_t* inserting_node, dl_list_t* next_node)
{
    if (head == NULL || inserting_node == NULL || next_node == NULL)
    {
        return;  // Silently return on invalid input to prevent crashes
    }
    inserting_node->next = next_node;       // Link inserting node to next node
    inserting_node->prev = next_node->prev; // Link inserting node to previous node
    if (next_node->prev == NULL)
    {
        *head = inserting_node;            // Update head if inserting at the start
    }
    else
    {
        next_node->prev->next = inserting_node; // Link previous node to inserting node
    }
    next_node->prev = inserting_node;       // Link next node back to inserting node
}

/**
 * @brief Sorts the doubly linked list using insertion sort
 * @param[in,out] head Pointer to the head of the list
 * @param[in] cmp Function pointer to compare two writer_line_t structures
 * @return int LL_SUCCESS on success, LL_ERR_NULL_INPUT on invalid input
 */
int LinkedList_sort(dl_list_t** head, int (*cmp)(const writer_line_t*, const writer_line_t*))
{
    dl_list_t* next_node, *temp_node;
    dl_list_t* curr_node;
    dl_list_t* chk_node;

    if ((head == NULL) || (cmp == NULL))
    {
        return LL_ERR_NULL_INPUT;  // Invalid input: NULL pointer
    }
    if (*head == NULL)
    {
        return LL_SUCCESS;         // Empty list is already sorted
    }
    curr_node = *head;
    *head = NULL;                  // Initialize sorted list as empty
    while (curr_node != NULL)
    {
        chk_node = *head;
        next_node = curr_node->next;  // Store next node for iteration
        curr_node->next = NULL;       // Disconnect current node
        curr_node->prev = NULL;       // Disconnect current node
        if (*head == NULL)
        {
            *head = curr_node;        // First node in sorted list
        }
        else
        {
            while (chk_node != NULL)
            {
                if (cmp(curr_node->line, chk_node->line) > 0)
                {
                    temp_node = chk_node;  // Store last checked node
                    chk_node = chk_node->next;  // Move to next node
                }
                else
                {
                    insert_node(head, curr_node, chk_node);  // Insert at current position
                    break;
                }
            }
            if (chk_node == NULL)
            {
                temp_node->next = curr_node;  // Append to end of sorted list
                curr_node->prev = temp_node;  // Link back to last node
            }
        }
        curr_node = next_node;  // Move to next unsorted node
    }
    return LL_SUCCESS;
}

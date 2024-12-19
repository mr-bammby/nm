#include "../inc_pub/linkedlist.h"
#include <stdio.h>

static void insert_node(dl_list_t** head, dl_list_t* inserting_node,  dl_list_t* next_node)
{
    inserting_node->next = next_node;
    inserting_node->prev = next_node->prev;
    if (next_node->prev == NULL)
    {
        *head = inserting_node;
    }
    else
    {
        next_node->prev->next = inserting_node;
    }
    next_node->prev = inserting_node;
    return ;
}


int LinkedList_sort(dl_list_t** head, int (*cmp)(const writer_line_t*, const writer_line_t*))
{
    dl_list_t* next_node, *temp_node;
    dl_list_t* curr_node;
    dl_list_t* chk_node;

    if ((head == NULL) || (cmp == NULL))
    {
        return (-1);
    }
    curr_node = *head;
    *head = NULL;
    while (curr_node != NULL)
    {
        chk_node = *head;
        next_node = curr_node->next;
        curr_node->next = NULL;
        curr_node->prev = NULL;
        if (*head == NULL)
        {
            *head = curr_node;
        }
        else
        {
            while (chk_node != NULL)
            {
                if (cmp(curr_node->line, chk_node->line) > 0)
                {
                    temp_node = chk_node;
                    chk_node = chk_node->next;
                }
                else
                {
                    insert_node(head, curr_node, chk_node);
                    break;
                }
            }
            if (chk_node == NULL)
            {
                temp_node->next = curr_node;
                curr_node->prev = temp_node;
            }
        }
        curr_node = next_node;
    }
    return (0);
}

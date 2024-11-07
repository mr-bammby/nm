#include "../inc_pub/linkedlist.h"

static void insert_node(dl_list_t** head, dl_list_t* inserting_node,  dl_list_t* next_node)
{
    inserting_node->next = next_node;
    inserting_node->prev = next_node->prev;
    next_node->prev = inserting_node;
    if (next_node->prev == NULL)
    {
        *head = inserting_node;
    }
    return ;
}


int LinkedList_sort(dl_list_t** head, int (*cmp)(const writer_line_t*, const writer_line_t*))
{
    dl_list_t** start = head;
    dl_list_t* curr_node;
    dl_list_t* chk_node;

    if ((head == NULL) || (cmp == NULL))
    {
        return (-1);
    }
    *head = NULL;
    curr_node = *start;
    while (curr_node != NULL)
    {
        chk_node = *head;
        curr_node->next = NULL;
        curr_node->prev = NULL;
        while (chk_node != NULL)
        {
            if (cmp(curr_node->line, chk_node->line) > 0)
            {
                chk_node = chk_node->next;
            }
            else
            {
                insert_node(head, curr_node, chk_node);
            }
        }
        if (chk_node == NULL)
        {
            curr_node->prev = *start;
            start = &curr_node;
        }
    }
    return (0);
}

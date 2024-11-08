#include "../inc_pub/linkedlist.h"
#include <stdio.h>

int LinkedList_nodePushBack(dl_list_t** head, writer_line_t* const line, dl_list_t** const node, dl_list_t* const clue)
{
    dl_list_t* next_node, *new_node;
    dl_list_t* curr_node = NULL;

    if ((line == NULL)  || (head == NULL))
    {
        return(-1);
    }
    new_node = malloc(sizeof(dl_list_t));
    if (new_node == NULL)
    {
        return(-3);
    }
    new_node->line = line;
    new_node->next = NULL;
    if (clue != NULL)
    {
        if (clue->next == NULL)
        {
            head = &(clue->next);
            curr_node = clue;
        }
    }
    next_node = *head;
    while (next_node != NULL)
    {   
        head = &(next_node->next);
        next_node = *head;
        curr_node = next_node;
    }
    new_node->prev = curr_node;
    if (curr_node== NULL)
    {
        *head = new_node;
    }
    if (node != NULL)
    {
        *node = new_node;
    }
    
    return(0);
}

int LinkedList_nodePopBack(dl_list_t** head, writer_line_t** const line, dl_list_t** const node, dl_list_t* const clue)
{
    dl_list_t* next_node;
    dl_list_t* curr_node = NULL;

    if ((line == NULL)  || (head == NULL))
    {
        return(-1);
    }
    if (*head == NULL)
    {
        return(-2);
    }
    if (clue != NULL)
    {
        if (clue->next == NULL)
        {
            curr_node = clue;
            head = &(clue->next);
        }
    }
    next_node = *head;
    while (next_node != NULL)
    {   
        head = &(next_node->next);
        curr_node = next_node;
        next_node = *head;
    }
    curr_node->prev->next = NULL;
    if (node != NULL)
    {
        *node = curr_node->prev;
    }
    *line = curr_node->line;
    free(curr_node);
    return(0);
}

int LinkedList_nodePopFront(dl_list_t** head, writer_line_t** const line)
{
    dl_list_t* curr_node;

    if ((line == NULL)  || (head == NULL))
    {
        return(-1);
    }
    if (*head == NULL)
    {
        return(-2);
    }
    if ((*head)->prev != NULL)
    {
        return(-4);
    }
    curr_node = *head;
    *head = curr_node->next;
    (*head)->prev = NULL;
    *line = curr_node->line;
    free(curr_node);
    return(0);
}

int LinkedList_delete(dl_list_t** head, void (*del)(void*))
{
    dl_list_t* curr_node;
    dl_list_t* prev_node;

    if (head == NULL)
    {
        return(-1);
    }
    if (*head == NULL)
    {
        return(-2);
    }
    if ((*head)->prev != NULL)
    {
        return(-4);
    }
    curr_node = *head;
    *head = NULL;
    while (curr_node != NULL)
    {
        prev_node = curr_node;
        if (del != NULL)
        {
            del(prev_node->line);
        }
        free(prev_node);
    }
    return(0);
}

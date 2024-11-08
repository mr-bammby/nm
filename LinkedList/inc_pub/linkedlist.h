#ifndef _IG_LINKEDLIST_H_
#define _IG_LINKEDLIST_H_

#include "../../Writer/inc_pub/writer.h"

typedef struct dl_list_s
{
    struct dl_list_s* prev;
    struct dl_list_s* next;
    writer_line_t *line;
} dl_list_t;

int LinkedList_nodePushBack(dl_list_t** head, writer_line_t* const line, dl_list_t** const node, dl_list_t* const clue);
int LinkedList_nodePopBack(dl_list_t** head, writer_line_t** const line, dl_list_t** const node, dl_list_t* const clue);
int LinkedList_nodePopFront(dl_list_t** head, writer_line_t** const line);
int LinkedList_sort(dl_list_t** head, int (*cmp)(const writer_line_t*, const writer_line_t*));
int LinkedList_delete(dl_list_t** head, void (*del)(void*));

#endif /* _IG_LINKEDLIST_H_ */
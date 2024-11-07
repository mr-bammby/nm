#ifndef _IG_LINKEDLIST_H_
#define _IG_LINKEDLIST_H_

#include "../../Writer/inc_pub/writer.h"

typedef struct dl_list_s
{
    dl_list_s* prev;
    dl_list_s* next;
    writer_line_t *line;
} dl_list_t;

int LinkedList_nodePushBack(dl_list_t** head, const dl_list_t* line);
int LinkedList_nodePopBack(dl_list_t** head, dl_list_t* line);
int LinkedList_nodePopFront(dl_list_t** head, dl_list_t* line);
int LinkedList_sort(dl_list_t** sort, int (*cmp)(writer_line_t*, writer_line_t*));
int LinkedList_delete(dl_list_t** sort, int (*del)(writer_line_t*));


#endif /* _IG_LINKEDLIST_H_ */
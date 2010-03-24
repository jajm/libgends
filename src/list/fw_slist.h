#ifndef fw_slist_h_included
#define fw_slist_h_included

#include "types.h"
#include "generic.h"

/************************************************
 * 		Fixed-Width Linked List		*
 * 		TODO : Comment IT		*
 ************************************************/
typedef s32 llist_pos_t;

struct fw_slist_node{
	void *d;
	struct fw_slist_node *next;
};

typedef struct _fw_slist{
	struct fw_slist_node *head;
	u32 width;
} *fw_slist;


fw_slist fw_slist_new(u32 width);
s8 fw_slist_add(fw_slist l, llist_pos_t pos, void *data);
s8 fw_slist_del(fw_slist l, llist_pos_t pos);
void *fw_slist_get(fw_slist l, llist_pos_t pos);
llist_pos_t fw_slist_chk(fw_slist l, void *data);
void fw_slist_print(fw_slist l);
s8 fw_slist_free(fw_slist *l);

#endif /* Not fw_slist_h_included */


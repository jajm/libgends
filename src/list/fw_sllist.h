#ifndef __fw_sllist_h__
#define __fw_sllist_h__

#include "types.h"
#include "err_code.h"
#include "generic.h"
#include "llist.h"

/************************************************
 * 		Fixed-Width Linked List		*
 * 		TODO : Comment IT		*
 ************************************************/
struct fw_sllist_node{
	void *d;
	struct fw_sllist_node *next;
};

typedef struct _fw_sllist{
	struct fw_sllist_node *head;
	u32 width;
} *fw_sllist;


fw_sllist fw_sllist_new(u32 width);
s8 fw_sllist_add(fw_sllist l, llist_pos_t pos, void *data);
s8 fw_sllist_del(fw_sllist l, llist_pos_t pos);
void *fw_sllist_get(fw_sllist l, llist_pos_t pos);
llist_pos_t fw_sllist_chk(fw_sllist l, void *data);
void fw_sllist_print(fw_sllist l);
s8 fw_sllist_free(fw_sllist *l);

#endif /* Not __fw_sllist_h__ */


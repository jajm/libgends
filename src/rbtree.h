#ifndef rbtree_h_included
#define rbtree_h_included

#include <stdbool.h>
#include <stdint.h>
#include "func_ptr.h"
#include "rbtree_node.h"

/* Insert data into the tree */
/*      root : pointer to pointer to the root node
 *      data : pointer to the data
 * copy_data : true => copy the data using alloc_f
 *             false => don't copy the data, only the pointer
 *   alloc_f : function that takes a pointer and return another pointer to a
 *             copy of data
 *  getkey_f : function that takes a pointer and return a key for the pointed
 *             data. Key will be used to determine where the new node will be
 *             inserted
 *  cmpkey_f : function that takes two keys and compare them. Should return 0 if
 *             keys are the same, a negative number if first key is lesser than
 *             second key, and a positive number otherwise. */
/* Return: 0: success
 *         a negative value: failure
 *         a positive value: key of data is already in the tree: nothing done */
int8_t
gds_rbtree_add(
	gds_rbtree_node_t **root,
	void *data,
	gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

void *
gds_rbtree_get(
	gds_rbtree_node_t *root,
	void *key,
	gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

int8_t
gds_rbtree_del(
	gds_rbtree_node_t **root,
	void *key,
	gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f,
	bool free_data,
	gds_func_ptr_t free_f
);

void
gds_rbtree_free(
	gds_rbtree_node_t *root,
	bool free_data,
	gds_func_ptr_t free_f
);

#endif /* Not rbtree_h_included */


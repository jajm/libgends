#ifndef compact_rbtree_h_included
#define compact_rbtree_h_included

#include "compact_rbtree_node.h"

int8_t
gds_compact_rbtree_add(
	gds_compact_rbtree_node_t **root,
	void *data,
	gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

void *
gds_compact_rbtree_get(
	gds_compact_rbtree_node_t *root,
	void *key,
	gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

int8_t
gds_compact_rbtree_del(
	gds_compact_rbtree_node_t **root,
	void *key,
	gds_func_ptr_t getkey_f,
	gds_func_ptr_t cmpkey_f,
	bool free_data,
	gds_func_ptr_t free_f
);

void
gds_compact_rbtree_free(
	gds_compact_rbtree_node_t *root,
	bool free_data,
	gds_func_ptr_t free_f
);

#endif /* Not defined compact_rbtree_h_included */


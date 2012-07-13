#ifndef compact_rbtree_h_included
#define compact_rbtree_h_included

#include "compact_rbtree_node.h"
#include "callbacks.h"
#include "slist_node.h"

int8_t
gds_compact_rbtree_add(
	gds_compact_rbtree_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb
);

int8_t
gds_compact_rbtree_set(
	gds_compact_rbtree_node_t **root,
	void *data,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb,
	gds_alloc_cb alloc_cb
);

void *
gds_compact_rbtree_get(
	gds_compact_rbtree_node_t *root,
	void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb
);

int8_t
gds_compact_rbtree_del(
	gds_compact_rbtree_node_t **root,
	void *key,
	gds_getkey_cb getkey_cb,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb
);

void
gds_compact_rbtree_free(
	gds_compact_rbtree_node_t *root,
	gds_free_cb free_cb
);

gds_slist_node_t *
gds_compact_rbtree_values(
	gds_compact_rbtree_node_t *root,
	gds_alloc_cb alloc_cb
);

#endif /* Not defined compact_rbtree_h_included */


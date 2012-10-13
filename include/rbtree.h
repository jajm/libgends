#ifndef rbtree_h_included
#define rbtree_h_included

#include <stdint.h>
#include "rbtree_node.h"
#include "core/callbacks.h"
#include "slist_node.h"

int8_t
gds_rbtree_add(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb key_alloc_cb,
	gds_alloc_cb alloc_cb
);

int8_t
gds_rbtree_set(
	gds_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb key_alloc_cb,
	gds_free_cb free_cb,
	gds_alloc_cb alloc_cb
);

void *
gds_rbtree_get(
	gds_rbtree_node_t *root,
	void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb
);

int8_t
gds_rbtree_del(
	gds_rbtree_node_t **root,
	void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

void
gds_rbtree_free(
	gds_rbtree_node_t *root,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

gds_slist_node_t *
gds_rbtree_keys(
	gds_rbtree_node_t *root,
	gds_alloc_cb key_alloc_cb
);

gds_slist_node_t *
gds_rbtree_values(
	gds_rbtree_node_t *root,
	gds_alloc_cb alloc_cb
);

typedef struct {
	void *key;
	void *data;
} gds_rbtree_list_node_t;

gds_slist_node_t *
gds_rbtree_nodes(
	gds_rbtree_node_t *root,
	gds_alloc_cb key_alloc_cb,
	gds_alloc_cb alloc_cb
);

#endif /* Not defined rbtree_h_included */


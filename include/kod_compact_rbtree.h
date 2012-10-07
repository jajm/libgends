#ifndef kod_compact_rbtree_h_included
#define kod_compact_rbtree_h_included

#include <stdint.h>
#include "kod_compact_rbtree_node.h"
#include "core/callbacks.h"
#include "slist_node.h"

int8_t
gds_kod_compact_rbtree_add(
	gds_kod_compact_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb key_alloc_cb,
	gds_alloc_cb alloc_cb
);

int8_t
gds_kod_compact_rbtree_set(
	gds_kod_compact_rbtree_node_t **root,
	void *key,
	void *data,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb key_alloc_cb,
	gds_free_cb free_cb,
	gds_alloc_cb alloc_cb
);

void *
gds_kod_compact_rbtree_get(
	gds_kod_compact_rbtree_node_t *root,
	void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb
);

int8_t
gds_kod_compact_rbtree_del(
	gds_kod_compact_rbtree_node_t **root,
	void *key,
	gds_cmpkey_cb cmpkey_cb,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

void
gds_kod_compact_rbtree_free(
	gds_kod_compact_rbtree_node_t *root,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

gds_slist_node_t *
gds_kod_compact_rbtree_keys(
	gds_kod_compact_rbtree_node_t *root,
	gds_alloc_cb key_alloc_cb
);

gds_slist_node_t *
gds_kod_compact_rbtree_values(
	gds_kod_compact_rbtree_node_t *root,
	gds_alloc_cb alloc_cb
);

typedef struct {
	void *key;
	void *data;
} gds_kod_compact_rbtree_list_node_t;

gds_slist_node_t *
gds_kod_compact_rbtree_nodes(
	gds_kod_compact_rbtree_node_t *root,
	gds_alloc_cb key_alloc_cb,
	gds_alloc_cb alloc_cb
);

#endif /* Not defined kod_compact_rbtree_h_included */


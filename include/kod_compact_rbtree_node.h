#ifndef kod_compact_rbtree_node_h_included
#define kod_compact_rbtree_node_h_included

#include <stdint.h>
#include "core/callbacks.h"
#include "inline_compact_rbtree.h"

struct gds_kod_compact_rbtree_node_s {
	void *key;
	void *data;
	gds_inline_compact_rbtree_node_t rbtree;
};
typedef struct gds_kod_compact_rbtree_node_s gds_kod_compact_rbtree_node_t;

gds_kod_compact_rbtree_node_t *
gds_kod_compact_rbtree_node_new(
	void *key,
	gds_alloc_cb key_alloc_cb,
	void *data,
	gds_alloc_cb alloc_cb
);

void *
gds_kod_compact_rbtree_node_get_key(
	gds_kod_compact_rbtree_node_t *node,
	gds_alloc_cb key_alloc_cb
);

int8_t
gds_kod_compact_rbtree_node_set_key(
	gds_kod_compact_rbtree_node_t *node,
	void *key,
	gds_alloc_cb key_alloc_cb,
	gds_free_cb key_free_cb
);

void *
gds_kod_compact_rbtree_node_get_data(
	gds_kod_compact_rbtree_node_t *node,
	gds_alloc_cb alloc_cb
);

int8_t
gds_kod_compact_rbtree_node_set_data(
	gds_kod_compact_rbtree_node_t *node,
	void *data,
	gds_alloc_cb alloc_cb,
	gds_free_cb free_cb
);

void
gds_kod_compact_rbtree_node_free(
	gds_kod_compact_rbtree_node_t *node,
	gds_free_cb key_free_cb,
	gds_free_cb free_cb
);

#endif /* Not kod_compact_rbtree_node_h_included */


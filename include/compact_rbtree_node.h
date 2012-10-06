#ifndef compact_rbtree_node_h_included
#define compact_rbtree_node_h_included

#include <stdint.h>
#include "core/callbacks.h"
#include "inline_compact_rbtree.h"

struct gds_compact_rbtree_node_s {
	void *data;
	gds_inline_compact_rbtree_node_t rbtree;
};
typedef struct gds_compact_rbtree_node_s gds_compact_rbtree_node_t;

gds_compact_rbtree_node_t *
gds_compact_rbtree_node_new(
	void *data,
	gds_alloc_cb alloc_cb
);

void *
gds_compact_rbtree_node_get_data(
	gds_compact_rbtree_node_t *node,
	gds_alloc_cb alloc_cb
);

int8_t
gds_compact_rbtree_node_set_data(
	gds_compact_rbtree_node_t *node,
	void *data,
	gds_alloc_cb alloc_cb,
	gds_free_cb free_cb
);

void
gds_compact_rbtree_node_free(
	gds_compact_rbtree_node_t *node,
	gds_free_cb free_cb
);

#endif /* Not compact_rbtree_node_h_included */


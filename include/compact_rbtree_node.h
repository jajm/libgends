#ifndef compact_rbtree_node_h_included
#define compact_rbtree_node_h_included

#include <stdint.h>
#include <stdbool.h>
#include "core/callbacks.h"

struct gds_compact_rbtree_node_s {
	void *data;
	bool red;
	struct gds_compact_rbtree_node_s *son[2];
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

bool
gds_compact_rbtree_node_is_red(
	gds_compact_rbtree_node_t *node
);

void
gds_compact_rbtree_node_free(
	gds_compact_rbtree_node_t *node,
	gds_free_cb free_cb
);

#endif /* Not compact_rbtree_node_h_included */


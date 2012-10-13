#ifndef rbtree_keyin_node_h_included
#define rbtree_keyin_node_h_included

#include <stdint.h>
#include "core/callbacks.h"
#include "rbtree_inline.h"

struct gds_rbtree_keyin_node_s {
	void *data;
	gds_rbtree_inline_node_t rbtree;
};
typedef struct gds_rbtree_keyin_node_s gds_rbtree_keyin_node_t;

gds_rbtree_keyin_node_t *
gds_rbtree_keyin_node_new(
	void *data,
	gds_alloc_cb alloc_cb
);

void *
gds_rbtree_keyin_node_get_data(
	gds_rbtree_keyin_node_t *node,
	gds_alloc_cb alloc_cb
);

int8_t
gds_rbtree_keyin_node_set_data(
	gds_rbtree_keyin_node_t *node,
	void *data,
	gds_alloc_cb alloc_cb,
	gds_free_cb free_cb
);

void
gds_rbtree_keyin_node_free(
	gds_rbtree_keyin_node_t *node,
	gds_free_cb free_cb
);

#endif /* Not rbtree_keyin_node_h_included */


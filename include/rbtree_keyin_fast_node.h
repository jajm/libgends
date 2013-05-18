#ifndef rbtree_keyin_fast_node_h_included
#define rbtree_keyin_fast_node_h_included

#include <stdbool.h>
#include <stdint.h>
#include "callbacks.h"
#include "inline/rbtree_fast.h"

struct gds_rbtree_keyin_fast_node_s {
	void *data;
	gds_inline_rbtree_fast_node_t rbtree;
};

typedef struct gds_rbtree_keyin_fast_node_s gds_rbtree_keyin_fast_node_t;

gds_rbtree_keyin_fast_node_t *
gds_rbtree_keyin_fast_node_new(
	void *data
);

void *
gds_rbtree_keyin_fast_node_get_data(
	gds_rbtree_keyin_fast_node_t *node
);

int8_t
gds_rbtree_keyin_fast_node_set_data(
	gds_rbtree_keyin_fast_node_t *node,
	void *data,
	gds_free_cb free_cb
);

void
gds_rbtree_keyin_fast_node_free(
	gds_rbtree_keyin_fast_node_t *node,
	gds_free_cb free_cb
);

#endif /* Not rbtree_keyin_fast_node_h_included */


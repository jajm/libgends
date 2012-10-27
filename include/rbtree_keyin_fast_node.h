#ifndef rbtree_keyin_fast_node_h_included
#define rbtree_keyin_fast_node_h_included

#include <stdbool.h>
#include <stdint.h>
#include "callbacks.h"

struct gds_rbtree_keyin_fast_node_s {
	void *data;
	bool red;
	struct gds_rbtree_keyin_fast_node_s *parent;
	struct gds_rbtree_keyin_fast_node_s *left;
	struct gds_rbtree_keyin_fast_node_s *right;
};

typedef struct gds_rbtree_keyin_fast_node_s gds_rbtree_keyin_fast_node_t;

gds_rbtree_keyin_fast_node_t *
gds_rbtree_keyin_fast_node_new(
	void *data,
	gds_alloc_cb alloc_cb
);

void *
gds_rbtree_keyin_fast_node_get_data(
	gds_rbtree_keyin_fast_node_t *node,
	gds_alloc_cb alloc_cb
);

int8_t
gds_rbtree_keyin_fast_node_set_data(
	gds_rbtree_keyin_fast_node_t *node,
	void *data,
	gds_alloc_cb alloc_cb,
	gds_free_cb free_cb
);

bool
gds_rbtree_keyin_fast_node_is_red(
	gds_rbtree_keyin_fast_node_t *node
);

void
gds_rbtree_keyin_fast_node_free(
	gds_rbtree_keyin_fast_node_t *node,
	gds_free_cb free_cb
);

#endif /* Not rbtree_keyin_fast_node_h_included */


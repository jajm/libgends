#ifndef compact_rbtree_node_h_included
#define compact_rbtree_node_h_included

#include <stdint.h>
#include <stdbool.h>
#include "func_ptr.h"

struct gds_compact_rbtree_node_s {
	void *data;
	bool red;
	struct gds_compact_rbtree_node_s *son[2];
};
typedef struct gds_compact_rbtree_node_s gds_compact_rbtree_node_t;

gds_compact_rbtree_node_t *
gds_compact_rbtree_node_new(
	void *data,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

void *
gds_compact_rbtree_node_get_data(
	gds_compact_rbtree_node_t *node,
	bool copy_data,
	gds_func_ptr_t alloc_f
);

int8_t
gds_compact_rbtree_node_set_data(
	gds_compact_rbtree_node_t *node,
	void *data,
	bool copy_data,
	gds_func_ptr_t alloc_f,
	bool free_old_data,
	gds_func_ptr_t free_f
);

bool
gds_compact_rbtree_node_is_red(
	gds_compact_rbtree_node_t *node
);

void
gds_compact_rbtree_node_free(
	gds_compact_rbtree_node_t *node,
	bool free_data,
	gds_func_ptr_t free_f
);

#endif /* Not compact_rbtree_node_h_included */


#ifndef gds_inline_rbtree_h_included
#define gds_inline_rbtree_h_included

#include <stdbool.h>
#include <stdint.h>

struct gds_inline_rbtree_node_s {
	_Bool red;
	struct gds_inline_rbtree_node_s *son[2];
};
typedef struct gds_inline_rbtree_node_s gds_inline_rbtree_node_t;

typedef int32_t (*gds_rbt_cmp_cb)(gds_inline_rbtree_node_t *,
	gds_inline_rbtree_node_t *, void *);
typedef int32_t (*gds_rbt_cmp_with_key_cb)(gds_inline_rbtree_node_t *,
	void *, void *);
typedef void (*gds_rbt_replace_cb)(gds_inline_rbtree_node_t *,
	gds_inline_rbtree_node_t *, void *);

void
gds_inline_rbtree_node_init(
	gds_inline_rbtree_node_t *node
);

int8_t
gds_inline_rbtree_add(
	gds_inline_rbtree_node_t **root,
	gds_inline_rbtree_node_t *node,
	gds_rbt_cmp_cb rbt_cmp_cb,
	void *rbt_cmp_data
);

gds_inline_rbtree_node_t *
gds_inline_rbtree_get_node(
	gds_inline_rbtree_node_t *root,
	void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb,
	void *rbt_cmp_data
);

int8_t
gds_inline_rbtree_del(
	gds_inline_rbtree_node_t **root,
	void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb,
	void *rbt_cmp_data,
	gds_rbt_replace_cb rbt_replace_cb,
	void *rbt_free_data
);

#endif /* Not gds_inline_rbtree_h_included */


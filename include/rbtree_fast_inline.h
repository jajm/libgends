#ifndef gds_rbtree_fast_inline_h_included
#define gds_rbtree_fast_inline_h_included

#include <stdbool.h>
#include <stdint.h>

struct gds_rbtree_fast_inline_node_s {
	_Bool red;
	struct gds_rbtree_fast_inline_node_s *parent;
	struct gds_rbtree_fast_inline_node_s *left;
	struct gds_rbtree_fast_inline_node_s *right;
};

typedef struct gds_rbtree_fast_inline_node_s gds_rbtree_fast_inline_node_t;

/* Compare a data and a red-black tree node */
/* Used during data insertion to determine where to place the future node. */
/* Must return a negative value if data should go to the left of node,
 * zero if data is already contained in node, and a positive value if data
 * should go to the right of node. */
typedef int32_t (*gds_rbtf_cmp_data_cb)(
	void *,                             // data
	gds_rbtree_fast_inline_node_t *,    // red-black tree node
	void *                              // user data
);

/* Create a red-black tree node */
/* Used during data insertion. It should create the node structure containing a
 * gds_rbtree_fast_inline_node_t node. */
/* Must return a pointer to the gds_rbtree_fast_inline_node_t node, or NULL if
 * it's not possible. */
typedef gds_rbtree_fast_inline_node_t * (*gds_rbtf_create_node_cb)(
	void *,    // data
	void *     // user data
);

/* Compare a key and a red-black tree node */
/* Used during search (for retrieving and deleting data) to determine where to
 * search for data. */
/* Must return a negative value if search should continue in left sub-tree of
 * node, zero if key corresponds to the node, and a positive value if search
 * should continue in right sub-tree of node. */
typedef int32_t (*gds_rbtf_cmp_key_cb)(
	void *,                             // key
	gds_rbtree_fast_inline_node_t *,    // red-black tree node
	void *                              // user data
);

/* Replace a node by another one */
/* Used during deletion. It should override node1's data by node2's data and
 * remove (free) node2 */
typedef void (*gds_rbtf_replace_cb)(
	gds_rbtree_fast_inline_node_t *,    // node1
	gds_rbtree_fast_inline_node_t *,    // node2
	void *                              // user data
);

/* Initialize red-black tree inline node with default values */
void
gds_rbtree_fast_inline_node_init(
	gds_rbtree_fast_inline_node_t *node
);

/* Insert data */
/* root                  : root node of tree
 * data                  : data to insert
 * rbtf_cmp_data_cb      : see above documentation about gds_rbtf_cmp_data_cb
 * rbtf_cmp_data_data    : user data passed to rbtf_cmp_data_cb
 * rbtf_create_node_cb   : see above documentation about gds_rbtf_create_node_cb
 * rbtf_create_node_data : user data passed to rbtf_create_node_data */
/* Return: 0 if data was successfully inserted
 *         1 if data was already in tree */
/* NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function. */
int8_t
gds_rbtree_fast_inline_add(
	gds_rbtree_fast_inline_node_t **root,
	void *data,
	gds_rbtf_cmp_data_cb rbtf_cmp_data_cb,
	void *rbtf_cmp_data_data,
	gds_rbtf_create_node_cb rbtf_create_node_cb,
	void *rbtf_create_node_data
);

/* Search a node by key */
/* root              : root node of tree
 * key               : key to search
 * rbtf_cmp_key_cb   : see above documentation about gds_rbtf_cmp_key_cb
 * rbtf_cmp_key_data : user data passed to rbtf_cmp_key_cb */
/* Return pointer to found node, or NULL if key was not found. */
gds_rbtree_fast_inline_node_t *
gds_rbtree_fast_inline_get_node(
	gds_rbtree_fast_inline_node_t *root,
	void *key,
	gds_rbtf_cmp_key_cb rbtf_cmp_key_cb,
	void *rbtf_cmp_key_data
);

/* Remove data by its key */
/* root              : root node of tree
 * key               : key of data to remove
 * rbtf_cmp_key_cb   : see above documentation about gds_rbtf_cmp_key_cb
 * rbtf_cmp_key_data : user data passed to rbtf_cmp_key_cb
 * rbtf_replace_cb   : see above documentation about gds_rbtf_replace_cb
 * rbtf_replace_data : user data passed to rbtf_replace_cb */
/* Return: 0 if data was successfully removed
 *         1 if data was not found */
/* NOTE: root will be modified so it will always point to the root of tree after
 * the function call. You should consider this when using this function. */
int8_t
gds_rbtree_fast_inline_del(
	gds_rbtree_fast_inline_node_t **root,
	void *key,
	gds_rbtf_cmp_key_cb rbtf_cmp_key_cb,
	void *rbtf_cmp_key_data,
	gds_rbtf_replace_cb rbtf_replace_cb,
	void *rbtf_replace_data
);

#endif /* Not gds_rbtree_fast_inline_h_included */


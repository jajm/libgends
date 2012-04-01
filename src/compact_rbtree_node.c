#include <stdlib.h>
#include "log.h"
#include "func_ptr.h"
#include "compact_rbtree_node.h"

gds_compact_rbtree_node_t * gds_compact_rbtree_node_new(void *data,
	bool copy_data, gds_func_ptr_t alloc_f)
{
	gds_compact_rbtree_node_t *node;
	
	if(copy_data && alloc_f == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}
	
	node = malloc(sizeof(gds_compact_rbtree_node_t));

	if(node == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}

	if(copy_data) {
		node->data = (void *)alloc_f(data);
	} else {
		node->data = data;
	}
	node->red = true;
	node->son[0] = NULL;
	node->son[1] = NULL;

	return node;
}

void * gds_compact_rbtree_node_get_data(gds_compact_rbtree_node_t *node,
	bool copy_data, gds_func_ptr_t alloc_f)
{
	void *data;

	if (node == NULL || (copy_data && alloc_f == NULL)) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	if (copy_data) {
		data = (void *)alloc_f(node->data);
	} else {
		data = node->data;
	}

	return data;
}

int8_t gds_compact_rbtree_node_set_data(gds_compact_rbtree_node_t *node,
	void *data, bool copy_data, gds_func_ptr_t alloc_f, bool free_old_data,
	gds_func_ptr_t free_f)
{
	void *d;

	if (node == NULL || (copy_data && alloc_f == NULL)
	|| (free_old_data && free_f == NULL) ) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	if (copy_data) {
		d = (void *)alloc_f(data);
	} else {
		d = data;
	}

	if (free_old_data) {
		free_f(node->data);
	}
	node->data = d;

	return 0;
}

bool gds_compact_rbtree_node_is_red(gds_compact_rbtree_node_t *node)
{
	return (node != NULL && node->red);
}

void gds_compact_rbtree_node_free(gds_compact_rbtree_node_t *node,
	bool free_data, gds_func_ptr_t free_f)
{
	if(node) {
		if(free_data && free_f != NULL) {
			free_f(node->data);
		}
		free(node);
	}
}

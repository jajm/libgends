#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "log.h"
#include "rbtree_node.h"

gds_rbtree_node_t * gds_rbtree_node_new(void *data, bool copy_data,
	gds_func_ptr_t alloc_f)
{
	gds_rbtree_node_t *n;

	if(copy_data && alloc_f == NULL) {
		GDS_LOG_ERROR("Bad arguments");
		return NULL;
	}

	n = malloc(sizeof(gds_rbtree_node_t));
	if(n == NULL) {
		GDS_LOG_ERROR("Memory allocation error");
		return NULL;
	}
	
	if(copy_data) {
		n->data = (void *)alloc_f(data);
	} else {
		n->data = data;
	}

	n->red = true;
	n->parent = n->left = n->right = NULL;
	
	return n;
}

bool gds_rbtree_node_is_red(gds_rbtree_node_t *node)
{
	return (node != NULL && node->red == true);
}

int8_t gds_rbtree_node_set_data(gds_rbtree_node_t *node, void *data,
	bool copy_data, gds_func_ptr_t alloc_f, bool free_old_data,
	gds_func_ptr_t free_f)
{
	void *d;

	if(node == NULL || (copy_data && alloc_f == NULL)
	|| (free_old_data && free_f == NULL)) {
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

void * gds_rbtree_node_get_data(gds_rbtree_node_t *node, bool copy_data,
	gds_func_ptr_t alloc_f)
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

void gds_rbtree_node_free(gds_rbtree_node_t *node, bool free_data,
	gds_func_ptr_t free_f)
{
	if(node) {
		if(free_data) {
			if(free_f == NULL) {
				GDS_LOG_WARNING("No free function provided");
			} else {
				free_f(node->data);
			}
		}
		free(node);
	}
}

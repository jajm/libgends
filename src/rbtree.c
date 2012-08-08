#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "rbtree.h"
#include "rbtree_node.h"
#include "callbacks.h"

gds_rbtree_node_t * gds_rbtree_node_grandparent(gds_rbtree_node_t *n)
{
	gds_rbtree_node_t *g = NULL;

	if(n != NULL && n->parent != NULL)
		g = n->parent->parent;

	return g;
}

gds_rbtree_node_t * gds_rbtree_node_uncle(gds_rbtree_node_t *n)
{
	gds_rbtree_node_t *g, *u = NULL;

	g = gds_rbtree_node_grandparent(n);
	if(g != NULL) {
		if(g->left == n->parent)
			u = g->right;
		else
			u = g->left;
	}

	return u;
}

gds_rbtree_node_t * gds_rbtree_node_sibling(gds_rbtree_node_t *n)
{
	gds_rbtree_node_t *s = NULL;

	if(n != NULL && n->parent != NULL) {
		if(n->parent->left == n)
			s = n->parent->right;
		else
			s = n->parent->left;
	}

	return s;
}

gds_rbtree_node_t * gds_rbtree_rotate_left(gds_rbtree_node_t *n)
{
	if(n != NULL && n->right != NULL) {
		n->right->parent = n->parent;
		if(n->parent != NULL) {
			if(n->parent->left == n)
				n->parent->left = n->right;
			else
				n->parent->right = n->right;
		}
		n->parent = n->right;
		n->right = n->right->left;
		if(n->right != NULL)
			n->right->parent = n;
		n->parent->left = n;
		return n->parent;
	}
	return NULL;
}

gds_rbtree_node_t * gds_rbtree_rotate_right(gds_rbtree_node_t *n)
{
	if(n != NULL && n->left != NULL) {
		n->left->parent = n->parent;
		if(n->parent != NULL) {
			if(n->parent->left == n)
				n->parent->left = n->left;
			else
				n->parent->right = n->left;
		}
		n->parent = n->left;
		n->left = n->left->right;
		if(n->left != NULL)
			n->left->parent = n;
		n->parent->right = n;
		return n->parent;
	}
	return NULL;
}

int8_t gds_rbtree_insert_bottom(gds_rbtree_node_t **root,
	void *data, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb, gds_rbtree_node_t **node_p)
{
	gds_rbtree_node_t *node;
	gds_rbtree_node_t *tmp, *parent = NULL;
	void *nkey, *key;
	int32_t cmp;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	nkey = getkey_cb(data);
	tmp = *root;
	while(tmp != NULL) {
		key = getkey_cb(tmp->data);
		cmp = cmpkey_cb(nkey, key);
		parent = tmp;
		if(cmp < 0) {
			tmp = tmp->left;
		} else if(cmp > 0) {
			tmp = tmp->right;
		} else {
			break;
		}
	}

	if(tmp != NULL) {
		GDS_LOG_WARNING("Node key already exists in tree");
		return 1;
	}

	node = gds_rbtree_node_new(data, alloc_cb);
	*node_p = node;

	if(parent != NULL) {
		if(cmp < 0) {
			parent->left = node;
		} else {
			parent->right = node;
		}
		node->parent = parent;
	} else {
		*root = node;
	}
	
	return 0;
}

void gds_rbtree_rebalance_after_insert(gds_rbtree_node_t **root,
	gds_rbtree_node_t *node)
{
	gds_rbtree_node_t *u, *g;
	bool valid = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(node);

	while(!valid) {
		if (node->parent == NULL) {
			/* node is the root node */
			node->red = false;
			*root = node;
			valid = 1;
		} else if (node->parent->red == false) {
			valid = 1;
		} else {
			u = gds_rbtree_node_uncle(node);
			if ((u != NULL) && (u->red == true)) {
				/* Parent and uncle are both red */
				node->parent->red = false;
				u->red = false;
				g = gds_rbtree_node_grandparent(node);
				g->red = true;
				node = g;
			} else {
				break;
			}
		}
	}
	if(!valid) {
		/* Parent is red, but uncle is black */
		g = gds_rbtree_node_grandparent(node);

		if ((node == node->parent->right)
		&& (node->parent == g->left)) {
			gds_rbtree_rotate_left(node->parent);
			node = node->left;
		} else if ((node == node->parent->left)
		&& (node->parent == g->right)) {
			gds_rbtree_rotate_right(node->parent);
			node = node->right;
		}

		g = gds_rbtree_node_grandparent(node);
		node->parent->red = false;
		g->red = true;
		if (node == node->parent->left) {
			node = gds_rbtree_rotate_right(g);
		} else {
			node = gds_rbtree_rotate_left(g);
		}
		if(node->parent == NULL)
			*root = node;
	}
}

int8_t gds_rbtree_add(gds_rbtree_node_t **root, void *data,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb)
{
	gds_rbtree_node_t *node = NULL;
	int8_t ret;

	ret = gds_rbtree_insert_bottom(root, data, getkey_cb, cmpkey_cb,
		alloc_cb, &node);
	if (node != NULL) {
		gds_rbtree_rebalance_after_insert(root, node);
	}

	return ret;
}

gds_rbtree_node_t * gds_rbtree_get_node(gds_rbtree_node_t *root,
	void *key, gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb)
{
	gds_rbtree_node_t *node = root;
	int32_t cmp;

	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	while(node != NULL) {
		cmp = cmpkey_cb(key, getkey_cb(node->data));
		if(cmp < 0) {
			node = node->left;
		} else if(cmp > 0) {
			node = node->right;
		} else {
			break;
		}
	}
	
	return node;
}

void * gds_rbtree_get(gds_rbtree_node_t *root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_alloc_cb alloc_cb)
{
	gds_rbtree_node_t *n;

	GDS_CHECK_ARG_NOT_NULL(root);

	n = gds_rbtree_get_node(root, key, getkey_cb, cmpkey_cb);
	return gds_rbtree_node_get_data(n, alloc_cb);
}

void gds_rbtree_rebalance_after_delete(gds_rbtree_node_t **root,
	gds_rbtree_node_t *parent, gds_rbtree_node_t *child)
{
	gds_rbtree_node_t *sibling, *tmp;
	bool valid = false;

	GDS_CHECK_ARG_NOT_NULL(root);

	/* If we are here, we removed a black node which had one black child
	 * or no child at all */

	while (!valid) {
		if (parent == NULL) {
			/* Case 1: child is the new root */
			valid = true;
			break;
		}

		sibling = (child == parent->left) ? parent->right : parent->left;
		if (gds_rbtree_node_is_red(sibling)) {
			/* Case 2: sibling is red */
			parent->red = true;
			sibling->red = false;
			if (child == parent->left) {
				tmp = gds_rbtree_rotate_left(parent);
				sibling = parent->right;
			} else {
				tmp = gds_rbtree_rotate_right(parent);
				sibling = parent->left;
			}
			if(tmp->parent == NULL)
				*root = tmp;
		}

		if ((parent->red == false) && (sibling->red == false)
		&& (!gds_rbtree_node_is_red(sibling->left))
		&& (!gds_rbtree_node_is_red(sibling->right)) ) {
			/* Case 3:
			 * parent, sibling and sibling's children are black */
			sibling->red = true;
			child = parent;
			parent = parent->parent;
		} else {
			break;
		}
	}

	/* Just to save one indentation level */
	if (valid) return;

	if ((parent->red == true) && (sibling->red == false)
	&& (!gds_rbtree_node_is_red(sibling->left))
	&& (!gds_rbtree_node_is_red(sibling->right)) ) {
		/* Case 4: sibling and sibling's children are black,
		 * but parent is red. */
		sibling->red = true;
		parent->red = false;
	} else {
		if ( (child == parent->left)
		&& (!gds_rbtree_node_is_red(sibling->right)) ) {
			/* Case 5: sibling is black, sibling's left child is
			 * red, sibling's right child is black, and child is the
			 * left child of its parent. */
			/* We continue to case 6 */
			sibling->red = true;
			sibling->left->red = false;
			sibling = gds_rbtree_rotate_right(sibling);
		} else if ( (child == parent->right)
		&& (!gds_rbtree_node_is_red(sibling->left)) ) {
			/* Case 5bis: sibling is black, sibling's left child is
			 * black, sibling's right child is red, and child is the
			 * right child of its parent. */
			/* Same as above, but replace 'left' by 'right' and
			 * 'right' by 'left'. We continue to case 6bis. */
			sibling->red = true;
			sibling->right->red = false;
			sibling = gds_rbtree_rotate_left(sibling);
		}

		sibling->red = parent->red;
		parent->red = false;
		if (child == parent->left) {
			/* Case 6: sibling is black, sibling's right child is
			 * red and child is the left child of its parent */
			sibling->right->red = false;
			tmp = gds_rbtree_rotate_left(parent);
			if(tmp->parent == NULL)
				*root = tmp;
		} else {
			/* Case 6bis: sibling is black, sibling's left child is
			 * red and child is the right child of its parent */
			/* Same as above, but replace 'left' by 'right' and
			 * 'right' by 'left' */
			sibling->left->red = false;
			tmp = gds_rbtree_rotate_right(parent);
			if(tmp->parent == NULL)
				*root = tmp;
		}
	}
}

void gds_rbtree_replace_with_child(gds_rbtree_node_t **root,
	gds_rbtree_node_t *node, gds_rbtree_node_t *child)
{
	if (root != NULL && node != NULL) {
		if (node->parent != NULL) {
			if(node->parent->left == node) {
				node->parent->left = child;
			} else {
				node->parent->right = child;
			}
		} else {
			*root = child;
		}
		if (child != NULL) {
			child->parent = node->parent;
		}
	}
}

int8_t gds_rbtree_del(gds_rbtree_node_t **root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb,
	gds_free_cb free_cb)
{
	gds_rbtree_node_t *node, *child;
	bool data_freed = false;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(getkey_cb);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	node = gds_rbtree_get_node(*root, key, getkey_cb, cmpkey_cb);
	if(node == NULL) {
		GDS_LOG_WARNING("key doesn't exist in tree");
		return 1;
	}

	if(node->left != NULL && node->right != NULL) {
		/* node has two children */
		/* we retrieve the right-most child of its left subtree and
		 * replace its value in node, so we'll have to delete a node
		 * which have at most one child */
		child = node->left;
		while(child->right != NULL) {
			child = child->right;
		}
		if (free_cb != NULL) {
			free_cb(node->data);
			data_freed = true;
		}
		node->data = child->data;
		node = child;
	}

	/* Node has at most one child */
	/* Replace node by its child */
	child = (node->left != NULL) ? node->left : node->right;
	gds_rbtree_replace_with_child(root, node, child);

	/* Rebalance tree */
	if (node->red == false) {
		if (child != NULL && child->red == true) {
			child->red = false;
		} else {
			gds_rbtree_rebalance_after_delete(root, node->parent, child);
		}
	}

	gds_rbtree_node_free(node, data_freed ? NULL : free_cb);
	return 0;
}

void gds_rbtree_free(gds_rbtree_node_t *root, gds_free_cb free_cb)
{
	if (root != NULL) {
		gds_rbtree_free(root->left, free_cb);
		gds_rbtree_free(root->right, free_cb);
		gds_rbtree_node_free(root, free_cb);
	}
}

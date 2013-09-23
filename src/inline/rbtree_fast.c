/*
 * Copyright 2012-2013 Julian Maurice
 *
 * This file is part of libgends
 *
 * libgends is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libgends is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgends.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "iterator.h"
#include "slist.h"
#include "../check_arg.h"
#include "inline/rbtree_fast.h"

_Bool gds_inline_rbtree_fast_node_is_red(gds_inline_rbtree_fast_node_t *n)
{
	if (n != NULL && n->red == true) {
		return true;
	}

	return false;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_node_grandparent(
	gds_inline_rbtree_fast_node_t *n)
{
	gds_inline_rbtree_fast_node_t *g = NULL;

	if(n != NULL && n->parent != NULL)
		g = n->parent->parent;

	return g;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_node_uncle(
	gds_inline_rbtree_fast_node_t *n)
{
	gds_inline_rbtree_fast_node_t *g, *u = NULL;

	g = gds_inline_rbtree_fast_node_grandparent(n);
	if(g != NULL) {
		if(g->left == n->parent)
			u = g->right;
		else
			u = g->left;
	}

	return u;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_node_sibling(
	gds_inline_rbtree_fast_node_t *n)
{
	gds_inline_rbtree_fast_node_t *s = NULL;

	if(n != NULL && n->parent != NULL) {
		if(n->parent->left == n)
			s = n->parent->right;
		else
			s = n->parent->left;
	}

	return s;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_rotate_left(
	gds_inline_rbtree_fast_node_t *n)
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

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_rotate_right(
	gds_inline_rbtree_fast_node_t *n)
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

void gds_inline_rbtree_fast_node_init(gds_inline_rbtree_fast_node_t *node)
{
	node->red = true;
	node->parent = node->left = node->right = NULL;
}

int gds_inline_rbtree_fast_insert_bottom(gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node, gds_rbtf_cmp_cb rbtf_cmp_cb,
	void *rbtf_cmp_data)
{
	gds_inline_rbtree_fast_node_t *tmp, *parent = NULL;
	int32_t cmp;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(rbtf_cmp_cb);

	tmp = *root;
	while (tmp != NULL) {
		parent = tmp;
		cmp = rbtf_cmp_cb(node, tmp, rbtf_cmp_data);
		if (cmp < 0) {
			tmp = tmp->left;
		} else if (cmp > 0) {
			tmp = tmp->right;
		} else {
			break;
		}
	}

	if (tmp != NULL) {
		GDS_LOG_WARNING("Node key already exists in tree");
		return 1;
	}

	node->left = node->right = NULL;
	node->parent = parent;

	if (parent != NULL) {
		if (cmp < 0) {
			parent->left = node;
		} else {
			parent->right = node;
		}
	} else {
		*root = node;
	}

	return 0;
}

void gds_inline_rbtree_fast_rebalance_after_insert(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node)
{
	gds_inline_rbtree_fast_node_t *u, *g;
	_Bool valid = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(node);

	while (!valid) {
		if (node->parent == NULL) {
			/* node is the root node */
			node->red = false;
			*root = node;
			valid = 1;
		} else if (node->parent->red == false) {
			valid = 1;
		} else {
			u = gds_inline_rbtree_fast_node_uncle(node);
			if ((u != NULL) && (u->red == true)) {
				/* Parent and uncle are both red */
				node->parent->red = false;
				u->red = false;
				g = gds_inline_rbtree_fast_node_grandparent(node);
				g->red = true;
				node = g;
			} else {
				break;
			}
		}
	}
	if (!valid) {
		/* Parent is red, but uncle is black */
		g = gds_inline_rbtree_fast_node_grandparent(node);

		if ((node == node->parent->right)
		&& (node->parent == g->left)) {
			gds_inline_rbtree_fast_rotate_left(node->parent);
			node = node->left;
		} else if ((node == node->parent->left)
		&& (node->parent == g->right)) {
			gds_inline_rbtree_fast_rotate_right(node->parent);
			node = node->right;
		}

		g = gds_inline_rbtree_fast_node_grandparent(node);
		node->parent->red = false;
		g->red = true;
		if (node == node->parent->left) {
			node = gds_inline_rbtree_fast_rotate_right(g);
		} else {
			node = gds_inline_rbtree_fast_rotate_left(g);
		}
		if (node->parent == NULL)
			*root = node;
	}
}

int gds_inline_rbtree_fast_add(gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node, gds_rbtf_cmp_cb rbtf_cmp_cb,
	void *rbtf_cmp_data)
{
	int rc;

	rc = gds_inline_rbtree_fast_insert_bottom(root, node, rbtf_cmp_cb,
		rbtf_cmp_data);
	if (rc == 0) {
		gds_inline_rbtree_fast_rebalance_after_insert(root, node);
	}

	return (node != NULL) ? 0 : 1;
}

void gds_inline_rbtree_fast_swap_nodes(gds_inline_rbtree_fast_node_t *node1,
	gds_inline_rbtree_fast_node_t *node2)
{
	gds_inline_rbtree_fast_node_t *node1_p, *node1_l, *node1_r;
	_Bool node1_red;

	GDS_CHECK_ARG_NOT_NULL(node1);
	GDS_CHECK_ARG_NOT_NULL(node2);

	node1_p = node1->parent;
	node1_l = node1->left;
	node1_r = node1->right;
	node1_red = node1->red;

	node1->parent = node2->parent;
	node1->left = node2->left;
	node1->right = node2->right;
	node1->red = node2->red;

	node2->parent = node1_p;
	node2->left = node1_l;
	node2->right = node1_r;
	node2->red = node1_red;

	/* If node1 was a parent or a child of node2, a link was broken.
	 * We must repair it. */
	if (node1->parent == node1) node1->parent = node2;
	if (node1->left == node1) node1->left = node2;
	if (node1->right == node1) node1->right = node2;
	if (node2->parent == node2) node2->parent = node1;
	if (node2->left == node2) node2->left = node1;
	if (node2->right == node2) node2->right = node1;

	if (node1->parent != NULL && node1->parent != node2) {
		if (node1->parent->left == node2)
			node1->parent->left = node1;
		else
			node1->parent->right = node1;
	}
	if (node1->left != NULL) node1->left->parent = node1;
	if (node1->right != NULL) node1->right->parent = node1;

	if (node2->parent != NULL && node2->parent != node1) {
		if (node2->parent->left == node1)
			node2->parent->left = node2;
		else
			node2->parent->right = node2;
	}
	if (node2->left != NULL) node2->left->parent = node2;
	if (node2->right != NULL) node2->right->parent = node2;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_replace_or_insert_bottom(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	gds_rbtf_cmp_cb rbtf_cmp_cb, void *rbtf_cmp_data)
{
	gds_inline_rbtree_fast_node_t *tmp, *parent = NULL;
	gds_inline_rbtree_fast_node_t *removed_node = NULL;
	int32_t cmp;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(rbtf_cmp_cb);

	tmp = *root;
	while (tmp != NULL) {
		parent = tmp;
		cmp = rbtf_cmp_cb(node, tmp, rbtf_cmp_data);
		if (cmp < 0) {
			tmp = tmp->left;
		} else if (cmp > 0) {
			tmp = tmp->right;
		} else {
			break;
		}
	}

	if (tmp != NULL) {
		gds_inline_rbtree_fast_swap_nodes(tmp, node);
		if (node->parent == NULL) *root = node;
		removed_node = tmp;
	} else {
		node->left = node->right = NULL;
		node->parent = parent;
		if (parent != NULL) {
			if (cmp < 0) {
				parent->left = node;
			} else {
				parent->right = node;
			}
		} else {
			*root = node;
		}
	}

	return removed_node;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_set(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	gds_rbtf_cmp_cb rbtf_cmp_cb, void *rbtf_cmp_data)
{
	gds_inline_rbtree_fast_node_t *removed_node;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(node);

	removed_node = gds_inline_rbtree_fast_replace_or_insert_bottom(root,
		node, rbtf_cmp_cb, rbtf_cmp_data);
	gds_inline_rbtree_fast_rebalance_after_insert(root, node);

	return removed_node;
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_get_node(
	gds_inline_rbtree_fast_node_t *root, void *key,
	gds_rbtf_cmp_with_key_cb rbtf_cmp_with_key_cb,
	void *rbtf_cmp_with_key_data)
{
	gds_inline_rbtree_fast_node_t *node = root;
	int32_t cmp;

	while (node != NULL) {
		cmp = rbtf_cmp_with_key_cb(node, key, rbtf_cmp_with_key_data);
		if (cmp < 0) {
			node = node->left;
		} else if(cmp > 0) {
			node = node->right;
		} else {
			break;
		}
	}
	
	return node;
}

void gds_inline_rbtree_fast_rebalance_after_delete(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *parent,
	gds_inline_rbtree_fast_node_t *child)
{
	gds_inline_rbtree_fast_node_t *sibling, *tmp;
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
		if (gds_inline_rbtree_fast_node_is_red(sibling)) {
			/* Case 2: sibling is red */
			parent->red = true;
			sibling->red = false;
			if (child == parent->left) {
				tmp = gds_inline_rbtree_fast_rotate_left(parent);
				sibling = parent->right;
			} else {
				tmp = gds_inline_rbtree_fast_rotate_right(parent);
				sibling = parent->left;
			}
			if(tmp->parent == NULL)
				*root = tmp;
		}

		if ((parent->red == false) && (sibling->red == false)
		&& (!gds_inline_rbtree_fast_node_is_red(sibling->left))
		&& (!gds_inline_rbtree_fast_node_is_red(sibling->right)) ) {
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
	&& (!gds_inline_rbtree_fast_node_is_red(sibling->left))
	&& (!gds_inline_rbtree_fast_node_is_red(sibling->right)) ) {
		/* Case 4: sibling and sibling's children are black,
		 * but parent is red. */
		sibling->red = true;
		parent->red = false;
	} else {
		if ( (child == parent->left)
		&& (!gds_inline_rbtree_fast_node_is_red(sibling->right)) ) {
			/* Case 5: sibling is black, sibling's left child is
			 * red, sibling's right child is black, and child is the
			 * left child of its parent. */
			/* We continue to case 6 */
			sibling->red = true;
			sibling->left->red = false;
			sibling = gds_inline_rbtree_fast_rotate_right(sibling);
		} else if ( (child == parent->right)
		&& (!gds_inline_rbtree_fast_node_is_red(sibling->left)) ) {
			/* Case 5bis: sibling is black, sibling's left child is
			 * black, sibling's right child is red, and child is the
			 * right child of its parent. */
			/* Same as above, but replace 'left' by 'right' and
			 * 'right' by 'left'. We continue to case 6bis. */
			sibling->red = true;
			sibling->right->red = false;
			sibling = gds_inline_rbtree_fast_rotate_left(sibling);
		}

		sibling->red = parent->red;
		parent->red = false;
		if (child == parent->left) {
			/* Case 6: sibling is black, sibling's right child is
			 * red and child is the left child of its parent */
			sibling->right->red = false;
			tmp = gds_inline_rbtree_fast_rotate_left(parent);
			if(tmp->parent == NULL)
				*root = tmp;
		} else {
			/* Case 6bis: sibling is black, sibling's left child is
			 * red and child is the right child of its parent */
			/* Same as above, but replace 'left' by 'right' and
			 * 'right' by 'left' */
			sibling->left->red = false;
			tmp = gds_inline_rbtree_fast_rotate_right(parent);
			if(tmp->parent == NULL)
				*root = tmp;
		}
	}
}

void gds_inline_rbtree_fast_replace_with_child(
	gds_inline_rbtree_fast_node_t **root,
	gds_inline_rbtree_fast_node_t *node,
	gds_inline_rbtree_fast_node_t *child)
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

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_del(
	gds_inline_rbtree_fast_node_t **root,
	void *key, gds_rbtf_cmp_with_key_cb rbtf_cmp_with_key_cb,
	void *rbtf_cmp_with_key_data)
{
	gds_inline_rbtree_fast_node_t *node, *child;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(rbtf_cmp_with_key_cb);

	node = gds_inline_rbtree_fast_get_node(*root, key, rbtf_cmp_with_key_cb,
		rbtf_cmp_with_key_data);
	if(node == NULL) {
		GDS_LOG_WARNING("key doesn't exist in tree");
		return NULL;
	}

	if(node->left != NULL && node->right != NULL) {
		/* node has two children */
		/* we retrieve the right-most child of its left subtree and
		 * replace its value in node, so we'll have to delete a node
		 * which have at most one child */
		child = node->left;
		while (child->right != NULL) {
			child = child->right;
		}

		gds_inline_rbtree_fast_swap_nodes(node, child);
		if (child->parent == NULL) *root = child;
	}

	/* Node has at most one child */
	/* Replace node by its child */
	child = (node->left != NULL)
		? node->left
		: node->right;
	gds_inline_rbtree_fast_replace_with_child(root, node, child);

	/* Rebalance tree */
	if (node->red == false) {
		if (child != NULL && child->red == true) {
			child->red = false;
		} else {
			gds_inline_rbtree_fast_rebalance_after_delete(root,
				node->parent, child);
		}
	}

	return node;
}

typedef struct {
	gds_inline_rbtree_fast_node_t *root;
	gds_slist_t *nodes;
	gds_iterator_t *slist_it;
} gds_inline_rbtree_fast_iterator_data_t;

void gds_inline_rbtree_fast_iterator_fill_list(gds_slist_t *nodes,
	gds_inline_rbtree_fast_node_t *root)
{
	if (root != NULL) {
		gds_inline_rbtree_fast_iterator_fill_list(nodes, root->left);
		gds_slist_push(nodes, root);
		gds_inline_rbtree_fast_iterator_fill_list(nodes, root->right);
	}
}

int gds_inline_rbtree_fast_iterator_reset(
	gds_inline_rbtree_fast_iterator_data_t *data)
{
	gds_iterator_free(data->slist_it);
	gds_slist_free(data->nodes, NULL, NULL);

	data->nodes = gds_slist_new();
	gds_inline_rbtree_fast_iterator_fill_list(data->nodes, data->root);
	data->slist_it = gds_slist_iterator_new(data->nodes);

	return 0;
}

int gds_inline_rbtree_fast_iterator_step(
	gds_inline_rbtree_fast_iterator_data_t *data)
{
	return gds_iterator_step(data->slist_it);
}

gds_inline_rbtree_fast_node_t * gds_inline_rbtree_fast_iterator_get(
	gds_inline_rbtree_fast_iterator_data_t *data)
{
	return gds_iterator_get(data->slist_it);
}

const void * gds_inline_rbtree_fast_iterator_getkey(
	gds_inline_rbtree_fast_iterator_data_t *data)
{
	return gds_iterator_getkey(data->slist_it);
}

void gds_inline_rbtree_fast_iterator_data_free(
	gds_inline_rbtree_fast_iterator_data_t *data)
{
	gds_iterator_free(data->slist_it);
	gds_slist_free(data->nodes, NULL, NULL);
	free(data);
}

gds_iterator_t * gds_inline_rbtree_fast_iterator_new(
	gds_inline_rbtree_fast_node_t *root)
{
	gds_inline_rbtree_fast_iterator_data_t *it_data;
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(root);

	it_data = malloc(sizeof(gds_inline_rbtree_fast_iterator_data_t));
	it_data->root = root;
	it_data->nodes = NULL;
	it_data->slist_it = NULL;

	it = gds_iterator_new(it_data,
		(gds_iterator_reset_cb) gds_inline_rbtree_fast_iterator_reset,
		(gds_iterator_step_cb) gds_inline_rbtree_fast_iterator_step,
		(gds_iterator_get_cb) gds_inline_rbtree_fast_iterator_get,
		(gds_iterator_getkey_cb) gds_inline_rbtree_fast_iterator_getkey,
		(gds_free_cb) gds_inline_rbtree_fast_iterator_data_free);

	return it;
}

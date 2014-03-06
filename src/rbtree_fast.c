/*
 * Copyright 2012-2014 Julian Maurice
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

#include <stdlib.h>
#include <stdio.h>
#include "malloc.h"
#include "assert.h"
#include "container_of.h"
#include "log.h"
#include "slist.h"
#include "inline/rbtree_fast.h"
#include "rbtree_fast.h"

gds_rbtree_fast_node_t * gds_rbtree_fast_node_new(void *key, void *data)
{
	gds_rbtree_fast_node_t *n;

	n = gds_malloc(sizeof(gds_rbtree_fast_node_t));

	n->key = key;
	n->data = data;

	gds_inline_rbtree_fast_node_init(&(n->rbtree));

	return n;
}

void gds_rbtree_fast_node_free(gds_rbtree_fast_node_t *node,
	void *key_free_cb, void *free_cb)
{
	void (*key_free_callback)(void *) = key_free_cb;
	void (*free_callback)(void *) = free_cb;

	if (node) {
		if (key_free_callback != NULL) {
			key_free_callback(node->key);
		}
		if (free_callback != NULL) {
			free_callback(node->data);
		}
		free(node);
	}
}

#define rbt_containerof(ptr) \
	((ptr) ? container_of(ptr, gds_rbtree_fast_node_t, rbtree) : NULL)

int gds_rbtree_fast_node_cmp(gds_inline_rbtree_fast_node_t *inode1,
	gds_inline_rbtree_fast_node_t *inode2, int (*cmpkey_cb)(void *, void *))
{
	gds_rbtree_fast_node_t *node1, *node2;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);

	return cmpkey_cb(node1->key, node2->key);
}

int gds_rbtree_fast_node_cmp_with_key(gds_inline_rbtree_fast_node_t *inode,
	void *key, int (*cmpkey_cb)(void *, void *))
{
	gds_rbtree_fast_node_t *node;

	node = rbt_containerof(inode);

	return cmpkey_cb(key, node->key);
}

int gds_rbtree_fast_add(gds_rbtree_fast_node_t **root, void *key, void *data,
	void *cmpkey_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_fast_node_t *node;
	int rc;

	gds_assert(root != NULL, -1);
	gds_assert(cmpkey_cb != NULL, -1);

	iroot = (*root != NULL) ? &((*root)->rbtree) : NULL;
	node = gds_rbtree_fast_node_new(key, data);
	inode = &(node->rbtree);
	rc = gds_inline_rbtree_fast_add(&iroot, inode, gds_rbtree_fast_node_cmp,
		cmpkey_cb);
	*root = rbt_containerof(iroot);

	if (rc > 0) {
		gds_rbtree_fast_node_free(node, NULL, NULL);
	}

	return rc;
}

gds_rbtree_fast_node_t * gds_rbtree_fast_get_node(gds_rbtree_fast_node_t *root,
	const void *key, void *cmpkey_cb)
{
	gds_inline_rbtree_fast_node_t *inode = NULL;

	gds_assert(cmpkey_cb != NULL, NULL);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_inline_rbtree_fast_get_node(&(root->rbtree), key,
		gds_rbtree_fast_node_cmp_with_key, cmpkey_cb);

	return rbt_containerof(inode);
}

int gds_rbtree_fast_set(gds_rbtree_fast_node_t **root, void *key, void *data,
	void *cmpkey_cb, void *key_free_cb, void *free_cb)
{
	gds_rbtree_fast_node_t *node, *removed;
	gds_inline_rbtree_fast_node_t *iroot, *iremoved = NULL;
	int rc;

	gds_assert(root != NULL, -1);
	gds_assert(cmpkey_cb != NULL, -1);

	node = gds_rbtree_fast_node_new(key, data);
	iroot = (*root != NULL) ? &((*root)->rbtree) : NULL;
	rc = gds_inline_rbtree_fast_set(&iroot, &(node->rbtree),
		gds_rbtree_fast_node_cmp, cmpkey_cb, &iremoved);
	*root = rbt_containerof(iroot);

	if (rc == 1 && iremoved != NULL) {
		removed = rbt_containerof(iremoved);
		key_free_cb = (removed->key != key) ? key_free_cb : NULL;
		free_cb = (removed->data != data) ? free_cb : NULL;
		gds_rbtree_fast_node_free(removed, key_free_cb, free_cb);
	}

	return rc;
}

void * gds_rbtree_fast_get(gds_rbtree_fast_node_t *root, const void *key,
	void *cmpkey_cb)
{
	gds_rbtree_fast_node_t *n;

	n = gds_rbtree_fast_get_node(root, key, cmpkey_cb);

	if (n == NULL) {
		return NULL;
	} else {
		return n->data;
	}
}

int gds_rbtree_fast_del(gds_rbtree_fast_node_t **root, const void *key,
	void *cmpkey_cb, void *key_free_cb, void *free_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_fast_node_t *node;
	int not_in_tree = 1;

	gds_assert(root != NULL, -1)
	gds_assert(cmpkey_cb != NULL, -1);

	if (*root != NULL) {
		iroot = &((*root)->rbtree);

		inode = gds_inline_rbtree_fast_del(&iroot, key,
			gds_rbtree_fast_node_cmp_with_key, cmpkey_cb);
		if (inode != NULL) {
			node = rbt_containerof(inode);
			gds_rbtree_fast_node_free(node, key_free_cb, free_cb);
			not_in_tree = 0;
		}

		*root = rbt_containerof(iroot);
	}

	return not_in_tree;
}

void * gds_rbtree_fast_pop(gds_rbtree_fast_node_t **root, const void *key,
	void *cmpkey_cb, void *key_free_cb)
{
	gds_inline_rbtree_fast_node_t *iroot, *inode;
	gds_rbtree_fast_node_t *node;
	void *data = NULL;

	gds_assert(root != NULL, NULL);
	gds_assert(cmpkey_cb != NULL, NULL);

	if(*root == NULL) {
		gds_log_warning("Tree is empty");
		return NULL;
	}

	iroot = &((*root)->rbtree);
	inode = gds_inline_rbtree_fast_del(&iroot, key,
		gds_rbtree_fast_node_cmp_with_key, cmpkey_cb);
	if (inode != NULL) {
		node = rbt_containerof(inode);
		data = node->data;
		gds_rbtree_fast_node_free(node, key_free_cb, NULL);
	}
	*root = rbt_containerof(iroot);

	return data;
}

void gds_rbtree_fast_free(gds_rbtree_fast_node_t *root, void *key_free_cb,
	void *free_cb)
{
	gds_rbtree_fast_node_t *node;

	if (root != NULL) {
		node = rbt_containerof(root->rbtree.left);
		gds_rbtree_fast_free(node, key_free_cb, free_cb);

		node = rbt_containerof(root->rbtree.right);
		gds_rbtree_fast_free(node, key_free_cb, free_cb);

		gds_rbtree_fast_node_free(root, key_free_cb, free_cb);
	}
}

typedef struct {
	gds_rbtree_fast_node_t *root;
	gds_iterator_t *inline_rbtree_fast_it;
} gds_rbtree_fast_iterator_data_t;

int gds_rbtree_fast_iterator_reset(gds_rbtree_fast_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_fast_it);
	data->inline_rbtree_fast_it =
		gds_inline_rbtree_fast_iterator_new(&(data->root->rbtree));

	return 0;
}

int gds_rbtree_fast_iterator_step(gds_rbtree_fast_iterator_data_t *data)
{
	return gds_iterator_step(data->inline_rbtree_fast_it);
}

void * gds_rbtree_fast_iterator_get(gds_rbtree_fast_iterator_data_t *data)
{
	gds_inline_rbtree_fast_node_t *inline_node;
	gds_rbtree_fast_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_fast_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->data : NULL;
}

const void * gds_rbtree_fast_iterator_getkey(gds_rbtree_fast_iterator_data_t *data)
{
	gds_inline_rbtree_fast_node_t *inline_node;
	gds_rbtree_fast_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_fast_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->key : NULL;
}

void gds_rbtree_fast_iterator_data_free(gds_rbtree_fast_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_fast_it);
	free(data);
}

gds_iterator_t * gds_rbtree_fast_iterator_new(gds_rbtree_fast_node_t *root)
{
	gds_rbtree_fast_iterator_data_t *data;
	gds_iterator_t *it;

	gds_assert(root != NULL, NULL);

	data = gds_malloc(sizeof(gds_rbtree_fast_iterator_data_t));

	data->root = root;
	data->inline_rbtree_fast_it = NULL;

	it = gds_iterator_new(data, gds_rbtree_fast_iterator_reset,
		gds_rbtree_fast_iterator_step, gds_rbtree_fast_iterator_get,
		gds_rbtree_fast_iterator_getkey,
		gds_rbtree_fast_iterator_data_free);

	return it;
}

void gds_rbtree_fast_build_keys_list(gds_rbtree_fast_node_t *root,
	gds_slist_t *list)
{
	if (root != NULL) {
		gds_rbtree_fast_build_keys_list(rbt_containerof(
			root->rbtree.right), list);
		gds_slist_unshift(list, root->key);
		gds_rbtree_fast_build_keys_list(rbt_containerof(
			root->rbtree.left), list);
	}
}

gds_slist_t * gds_rbtree_fast_keys(gds_rbtree_fast_node_t *root)
{
	gds_slist_t *list = gds_slist_new(NULL);

	gds_rbtree_fast_build_keys_list(root, list);

	return list;
}

void gds_rbtree_fast_build_values_list(gds_rbtree_fast_node_t *root,
	gds_slist_t *list)
{
	if (root != NULL) {
		gds_rbtree_fast_build_values_list(
			rbt_containerof(root->rbtree.right), list);
		gds_slist_unshift(list, root->data);
		gds_rbtree_fast_build_values_list(
			rbt_containerof(root->rbtree.left), list);
	}
}

gds_slist_t * gds_rbtree_fast_values(gds_rbtree_fast_node_t *root)
{
	gds_slist_t *list = gds_slist_new(NULL);

	gds_rbtree_fast_build_values_list(root, list);

	return list;
}

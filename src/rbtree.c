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
#include "exception.h"
#include "check_arg.h"
#include "log.h"
#include "inline/rbtree.h"
#include "rbtree.h"
#include "slist.h"

gds_rbtree_node_t * gds_rbtree_node_new(void *key, void *data)
{
	gds_rbtree_node_t *node;
	
	node = malloc(sizeof(gds_rbtree_node_t));

	if(node == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_node_t));
	}

	node->key = key;
	node->data = data;

	gds_inline_rbtree_node_init(&(node->rbtree));

	return node;
}

void * gds_rbtree_node_get_key(gds_rbtree_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->key;
}

void gds_rbtree_node_set_key(gds_rbtree_node_t *node, void *key,
	void *key_free_cb)
{
	void (*key_free_callback)(void *) = key_free_cb;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (key_free_callback != NULL) {
		key_free_callback(node->key);
	}
	node->key = key;
}

void * gds_rbtree_node_get_data(gds_rbtree_node_t *node)
{
	GDS_CHECK_ARG_NOT_NULL(node);

	return node->data;
}

void gds_rbtree_node_set_data(gds_rbtree_node_t *node, void *data,
	void *free_cb)
{
	void (*free_callback)(void *) = free_cb;

	GDS_CHECK_ARG_NOT_NULL(node);

	if (free_callback != NULL) {
		free_callback(node->data);
	}
	node->data = data;
}

void gds_rbtree_node_free(gds_rbtree_node_t *node, void *key_free_cb, void *free_cb)
{
	void (*key_free_callback)(void *) = key_free_cb;
	void (*free_callback)(void *) = free_cb;

	if(node) {
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
	((ptr) != NULL) \
	? (gds_rbtree_node_t *)((char *)ptr - offsetof(gds_rbtree_node_t, rbtree)) \
	: NULL

int gds_rbtree_node_cmp_with_key(gds_inline_rbtree_node_t *inode, void *key,
	int (*cmpkey_cb)(void *, void *))
{
	gds_rbtree_node_t *node;
	int cmp;

	node = rbt_containerof(inode);
	cmp = cmpkey_cb(key, node->key);
	return cmp;
}

int gds_rbtree_node_cmp(gds_inline_rbtree_node_t *inode1,
	gds_inline_rbtree_node_t *inode2, int (*cmpkey_cb)(void *, void *))
{
	gds_rbtree_node_t *node1, *node2;
	int cmp;

	node1 = rbt_containerof(inode1);
	node2 = rbt_containerof(inode2);

	cmp = cmpkey_cb(node1->key, node2->key);

	return cmp;
}

int gds_rbtree_add(gds_rbtree_node_t **root, void *key, void *data,
	void *cmpkey_cb)
{
	gds_rbtree_node_t *node;
	gds_inline_rbtree_node_t *iroot;
	int rc = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	node = gds_rbtree_node_new(key, data);
	iroot = (*root != NULL) ? &((*root)->rbtree) : NULL;
	rc = gds_inline_rbtree_add(&iroot, &(node->rbtree),
		gds_rbtree_node_cmp, cmpkey_cb);
	*root = rbt_containerof(iroot);

	if (rc > 0) {
		gds_rbtree_node_free(node, NULL, NULL);
	}

	return rc;
}

gds_rbtree_node_t * gds_rbtree_get_node(gds_rbtree_node_t *root,
	const void *key, void *cmpkey_cb)
{
	gds_inline_rbtree_node_t *inode = NULL;

	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if (root == NULL) {
		return NULL;
	}

	inode = gds_inline_rbtree_get_node(&(root->rbtree), key,
		gds_rbtree_node_cmp_with_key, cmpkey_cb);

	return rbt_containerof(inode);
}

void * gds_rbtree_get(gds_rbtree_node_t *root, const void *key, void *cmpkey_cb)
{
	gds_rbtree_node_t *node;

	node = gds_rbtree_get_node(root, key, cmpkey_cb);

	if (node == NULL) {
		return NULL;
	} else {
		return gds_rbtree_node_get_data(node);
	}
}

int gds_rbtree_set(gds_rbtree_node_t **root, void *key, void *data,
	void *cmpkey_cb, void *key_free_cb, void *free_cb)
{
	gds_rbtree_node_t *node, *removed;
	gds_inline_rbtree_node_t *iroot, *iremoved = NULL;
	int rc;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	node = gds_rbtree_node_new(key, data);
	iroot = (*root != NULL) ? &((*root)->rbtree) : NULL;
	rc = gds_inline_rbtree_set(&iroot, &(node->rbtree),
		gds_rbtree_node_cmp, cmpkey_cb, &iremoved);
	*root = rbt_containerof(iroot);

	if (rc == 2 && iremoved != NULL) {
		rc = 1;
		removed = rbt_containerof(iremoved);
		key_free_cb = (removed->key != key) ? key_free_cb : NULL;
		free_cb = (removed->data != data) ? free_cb : NULL;
		gds_rbtree_node_free(removed, key_free_cb, free_cb);
	}

	return rc;
}

int gds_rbtree_del(gds_rbtree_node_t **root, const void *key,
	void *cmpkey_cb, void *key_free_cb, void *free_cb)
{
	gds_inline_rbtree_node_t *iroot, *inode;
	gds_rbtree_node_t *node;
	int deleted = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(cmpkey_cb);

	if(*root == NULL) {
		GDS_LOG_WARNING("Tree is empty");
		return 1;
	}

	iroot = &((*root)->rbtree);
	inode = gds_inline_rbtree_del(&iroot, key,
		gds_rbtree_node_cmp_with_key, cmpkey_cb);
	if (inode != NULL) {
		node = rbt_containerof(inode);
		gds_rbtree_node_free(node, key_free_cb, free_cb);
		deleted = 1;
	}
	*root = rbt_containerof(iroot);

	return deleted ? 0 : 1;
}

void gds_rbtree_free(gds_rbtree_node_t *root, void *key_free_cb, void *free_cb)
{
	gds_rbtree_node_t *node;

	if (root != NULL) {
		node = rbt_containerof(root->rbtree.son[0]);
		gds_rbtree_free(node, key_free_cb, free_cb);

		node = rbt_containerof(root->rbtree.son[1]);
		gds_rbtree_free(node, key_free_cb, free_cb);

		gds_rbtree_node_free(root, key_free_cb, free_cb);
	}
}

typedef struct {
	gds_rbtree_node_t *root;
	gds_iterator_t *inline_rbtree_it;
} gds_rbtree_iterator_data_t;

int gds_rbtree_iterator_reset(gds_rbtree_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_it);
	data->inline_rbtree_it =
		gds_inline_rbtree_iterator_new(&(data->root->rbtree));

	return 0;
}

int gds_rbtree_iterator_step(gds_rbtree_iterator_data_t *data)
{
	return gds_iterator_step(data->inline_rbtree_it);
}

void * gds_rbtree_iterator_get(gds_rbtree_iterator_data_t *data)
{
	gds_inline_rbtree_node_t *inline_node;
	gds_rbtree_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->data : NULL;
}

const void * gds_rbtree_iterator_getkey(gds_rbtree_iterator_data_t *data)
{
	gds_inline_rbtree_node_t *inline_node;
	gds_rbtree_node_t *node;

	inline_node = gds_iterator_get(data->inline_rbtree_it);
	node = rbt_containerof(inline_node);

	return (node != NULL) ? node->key : NULL;
}

void gds_rbtree_iterator_data_free(gds_rbtree_iterator_data_t *data)
{
	gds_iterator_free(data->inline_rbtree_it);
	free(data);
}

gds_iterator_t * gds_rbtree_iterator_new(gds_rbtree_node_t *root)
{
	gds_rbtree_iterator_data_t *data;
	gds_iterator_t *it;

	GDS_CHECK_ARG_NOT_NULL(root);

	data = malloc(sizeof(gds_rbtree_iterator_data_t));
	if (data == NULL) {
		GDS_THROW_ALLOC_ERROR(sizeof(gds_rbtree_iterator_data_t));
	}

	data->root = root;
	data->inline_rbtree_it = NULL;

	it = gds_iterator_new(data, gds_rbtree_iterator_reset,
		gds_rbtree_iterator_step, gds_rbtree_iterator_get,
		gds_rbtree_iterator_getkey, gds_rbtree_iterator_data_free);

	return it;
}

void gds_rbtree_build_keys_list(gds_rbtree_node_t *root,
	gds_slist_t *list)
{
	if (root != NULL) {
		gds_rbtree_build_keys_list(rbt_containerof(
			root->rbtree.son[1]), list);
		gds_slist_unshift(list, root->key);
		gds_rbtree_build_keys_list(rbt_containerof(
			root->rbtree.son[0]), list);
	}
}

gds_slist_t * gds_rbtree_keys(gds_rbtree_node_t *root)
{
	gds_slist_t *list = gds_slist_new();

	gds_rbtree_build_keys_list(root, list);

	return list;
}

void gds_rbtree_build_values_list(gds_rbtree_node_t *root,
	gds_slist_t *list)
{
	if (root != NULL) {
		gds_rbtree_build_values_list(
			rbt_containerof(root->rbtree.son[1]), list);
		gds_slist_unshift(list, root->data);
		gds_rbtree_build_values_list(
			rbt_containerof(root->rbtree.son[0]), list);
	}
}

gds_slist_t * gds_rbtree_values(gds_rbtree_node_t *root)
{
	gds_slist_t *list = gds_slist_new();

	gds_rbtree_build_values_list(root, list);

	return list;
}


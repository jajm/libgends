#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "log.h"
#include "compact_rbtree_node.h"
#include "compact_rbtree.h"
#include "callbacks.h"

gds_compact_rbtree_node_t * gds_compact_rbtree_rotate(
	gds_compact_rbtree_node_t *node, uint8_t dir)
{
	gds_compact_rbtree_node_t *tmp;

	dir = (dir) ? 1 : 0;
	tmp = node->son[!dir];

	node->son[!dir] = tmp->son[dir];
	tmp->son[dir] = node;

	node->red = true;
	tmp->red = false;

	return tmp;
}

gds_compact_rbtree_node_t * gds_compact_rbtree_rotate_twice(
	gds_compact_rbtree_node_t *node, uint8_t dir)
{
	node->son[!dir] = gds_compact_rbtree_rotate(node->son[!dir], !dir);
	return gds_compact_rbtree_rotate(node, dir);
}

int8_t gds_compact_rbtree_add(gds_compact_rbtree_node_t **root, void *data,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_compact_rbtree_node_t *node;
	gds_compact_rbtree_node_t head;      /* False tree root */
	gds_compact_rbtree_node_t *g, *t;    /* Grandparent & parent */
	gds_compact_rbtree_node_t *p, *q;    /* Iterator & parent */
	uint8_t dir = 0, last = 0;
	void *key;
	int32_t cmp;
	bool node_added = false;

	if (root == NULL || getkey_cb == NULL || cmpkey_cb == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}

	if (*root == NULL) {
		/* Empty tree case */
		node = gds_compact_rbtree_node_new(data, alloc_cb);
		if (node == NULL) {
			GDS_LOG_ERROR("Failed to create node");
			return -1;
		}
		*root = node;
		(*root)->red = false;
		return 0;
	}

	/* Set up helpers */
	t = &head;
	g = p = NULL;
	q = t->son[1] = *root;
	key = getkey_cb(data);

	/* Search down the tree */
	while (true) {
		if (q == NULL) {
			/* Insert new node at the bottom */
			node = gds_compact_rbtree_node_new(data, alloc_cb);
			if (node == NULL) {
				GDS_LOG_ERROR("Failed to create node");
				return -1;
			}
			p->son[dir] = q = node;
			node_added = true;
		}
		else if (gds_compact_rbtree_node_is_red(q->son[0])
		&& gds_compact_rbtree_node_is_red(q->son[1])) {
			/* Color flip */
			q->red = true;
			q->son[0]->red = false;
			q->son[1]->red = false;
		}

		/* Fix red violation */
		if (gds_compact_rbtree_node_is_red(q)
		&& gds_compact_rbtree_node_is_red(p)) {
			uint8_t dir2 = (t->son[1] == g) ? 1 : 0;

			if (q == p->son[last])
				t->son[dir2] = gds_compact_rbtree_rotate(g, !last);
			else
				t->son[dir2] = gds_compact_rbtree_rotate_twice(g, !last);
		}

		/* Stop if found */
		cmp = cmpkey_cb(key, getkey_cb(q->data));
		if (cmp == 0)
			break;

		last = dir;
		dir = (cmp > 0) ? 1 : 0;

		/* Update helpers */
		if (g != NULL)
			t = g;
		g = p, p = q;
		q = q->son[dir];
	}

	/* Update root */
	*root = head.son[1];
	(*root)->red = false;

	return (node_added) ? 0 : 1;
}

void * gds_compact_rbtree_get(gds_compact_rbtree_node_t *root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_alloc_cb alloc_cb)
{
	gds_compact_rbtree_node_t *node;
	uint8_t dir;
	int32_t cmp;

	if (getkey_cb == NULL || cmpkey_cb == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return NULL;
	}

	node = root;
	while (node != NULL) {
		cmp = cmpkey_cb(key, getkey_cb(node->data));

		if (cmp == 0)
			break;

		dir = (cmp > 0) ? 1 : 0;
		node = node->son[dir];
	}

	return gds_compact_rbtree_node_get_data(node, alloc_cb);
}

int8_t gds_compact_rbtree_del(gds_compact_rbtree_node_t **root, void *key,
	gds_getkey_cb getkey_cb, gds_cmpkey_cb cmpkey_cb, gds_free_cb free_cb)
{
	gds_compact_rbtree_node_t head;       /* False tree root */
	gds_compact_rbtree_node_t *q, *p, *g; /* Helpers */
	gds_compact_rbtree_node_t *f = NULL;  /* Found item */
	uint8_t dir = 1;
	bool node_deleted = false;

	if(root == NULL || getkey_cb == NULL || cmpkey_cb == NULL) {
		GDS_LOG_ERROR("Bad parameters");
		return -1;
	}
	
	if(*root == NULL) {
		GDS_LOG_WARNING("Tree is empty");
		return 1;
	}

	/* Set up helpers */
	q = &head;
	g = p = NULL;
	q->son[0] = NULL;
	q->son[1] = *root;

	/* Search and push a red down */
	while (q->son[dir] != NULL) {
		uint8_t last = dir;
		int32_t cmp;

		/* Update helpers */
		g = p, p = q;
		q = q->son[dir];
		cmp = cmpkey_cb(key, getkey_cb(q->data));
		dir = (cmp > 0) ? 1 : 0;

		/* Save found node */
		if (cmp == 0)
			f = q;

		if (gds_compact_rbtree_node_is_red(q)
		|| gds_compact_rbtree_node_is_red(q->son[dir]))
			continue;

		/* Push the red node down */
		if (gds_compact_rbtree_node_is_red(q->son[!dir])) {
			p = p->son[last] = gds_compact_rbtree_rotate(q, dir);
		}
		else if (!gds_compact_rbtree_node_is_red(q->son[!dir])) {
			gds_compact_rbtree_node_t *s = p->son[!last];

			if (s == NULL)
				continue;

			if (!gds_compact_rbtree_node_is_red(s->son[!last])
			&& !gds_compact_rbtree_node_is_red(s->son[last])) {
				/* Color flip */
				p->red = false;
				s->red = true;
				q->red = true;
			} else {
				uint8_t dir2 = (g->son[1] == p) ? 1 : 0;

				if ( gds_compact_rbtree_node_is_red ( s->son[last] ) )
					g->son[dir2] = gds_compact_rbtree_rotate_twice(p, last);
				else if (gds_compact_rbtree_node_is_red(s->son[!last]))
					g->son[dir2] = gds_compact_rbtree_rotate(p, last);

				/* Ensure correct coloring */
				q->red = g->son[dir2]->red = true;
				g->son[dir2]->son[0]->red = false;
				g->son[dir2]->son[1]->red = false;
			}
		}
	}

	/* Replace and remove if found */
	if (f != NULL) {
		uint8_t dir1, dir2;
		if (free_cb != NULL) {
			free_cb(f->data);
		}
		f->data = q->data;
		dir1 = (p->son[1] == q) ? 1 : 0;
		dir2 = (q->son[0] == NULL) ? 1 : 0;
		p->son[dir1] = q->son[dir2];
		gds_compact_rbtree_node_free(q, NULL);
		node_deleted = true;
	}

	/* Update root and make it black */
	*root = head.son[1];
	if (*root != NULL)
		(*root)->red = false;

	return node_deleted ? 0 : 1;
}

void gds_compact_rbtree_free(gds_compact_rbtree_node_t *root,
	gds_free_cb free_cb)
{
	if (root != NULL) {
		gds_compact_rbtree_free(root->son[0], free_cb);
		gds_compact_rbtree_free(root->son[1], free_cb);
		gds_compact_rbtree_node_free(root, free_cb);
	}
}

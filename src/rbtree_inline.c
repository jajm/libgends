#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "rbtree_inline.h"
#include "check_arg.h"

void gds_rbtree_inline_print_dbg_r(gds_rbtree_inline_node_t *root, uint8_t d)
{
	if (root != NULL) {
		gds_rbtree_inline_print_dbg_r(root->son[1], d+1);
		for (uint8_t i=0; i<d; i++)
			printf("- ");
		printf("rbtree %p [%s] (0: %p, 1: %p)\n", root,
			root->red ? "red" : "black", root->son[0], root->son[1]);
		gds_rbtree_inline_print_dbg_r(root->son[0], d+1);
	}
}
void gds_rbtree_inline_print_dbg(gds_rbtree_inline_node_t *root)
{
	printf("\n---------------------------------------------------------\n");
	gds_rbtree_inline_print_dbg_r(root, 0);
	printf("---------------------------------------------------------\n");
}

_Bool gds_rbtree_inline_node_is_red(gds_rbtree_inline_node_t *node)
{
	return (node && node->red);
}

gds_rbtree_inline_node_t * gds_rbtree_inline_rotate(
	gds_rbtree_inline_node_t *node, uint8_t dir)
{
	gds_rbtree_inline_node_t *tmp;

	dir = (dir) ? 1 : 0;
	tmp = node->son[!dir];

	node->son[!dir] = tmp->son[dir];
	tmp->son[dir] = node;

	node->red = true;
	tmp->red = false;

	return tmp;
}

gds_rbtree_inline_node_t * gds_rbtree_inline_rotate_twice(
	gds_rbtree_inline_node_t *node, uint8_t dir)
{
	node->son[!dir] = gds_rbtree_inline_rotate(node->son[!dir], !dir);
	return gds_rbtree_inline_rotate(node, dir);
}

void gds_rbtree_inline_node_init(gds_rbtree_inline_node_t *node)
{
	node->red = true;
	node->son[0] = node->son[1] = NULL;
}

int8_t gds_rbtree_inline_add(gds_rbtree_inline_node_t **root,
	gds_rbtree_inline_node_t *node, gds_rbt_cmp_cb rbt_cmp_cb,
	void *rbt_cmp_data)
{
	gds_rbtree_inline_node_t head;      /* False tree root */
	gds_rbtree_inline_node_t *g, *t;    /* Grandparent & parent */
	gds_rbtree_inline_node_t *p, *q;    /* Iterator & parent */
	uint8_t dir = 0, last = 0;
	int32_t cmp;
	int8_t added = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(*root);
	GDS_CHECK_ARG_NOT_NULL(node);
	GDS_CHECK_ARG_NOT_NULL(rbt_cmp_cb);

	/* Set up helpers */
	t = &head;
	g = p = NULL;
	q = t->son[1] = *root;

	/* Search down the tree */
	while (true) {
		if (q == NULL) {
			/* Insert new node at the bottom */
			p->son[dir] = q = node;
			added = 1;
		}
		else if (gds_rbtree_inline_node_is_red(q->son[0])
		&& gds_rbtree_inline_node_is_red(q->son[1])) {
			/* Color flip */
			q->red = true;
			q->son[0]->red = false;
			q->son[1]->red = false;
		}

		/* Fix red violation */
		if (gds_rbtree_inline_node_is_red(q)
		&& gds_rbtree_inline_node_is_red(p)) {
			uint8_t dir2 = (t->son[1] == g) ? 1 : 0;

			if (q == p->son[last]) {
				t->son[dir2] = gds_rbtree_inline_rotate(g, !last);
			} else {
				t->son[dir2] = gds_rbtree_inline_rotate_twice(g, !last);
			}
		}

		/* Stop if found */
		cmp = rbt_cmp_cb(node, q, rbt_cmp_data);
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

	return added;
}

gds_rbtree_inline_node_t * gds_rbtree_inline_get_node(
	gds_rbtree_inline_node_t *root, void *key,
	gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb, void *rbt_cmp_data)
{
	gds_rbtree_inline_node_t *node;
	int32_t cmp;
	int8_t dir;

	GDS_CHECK_ARG_NOT_NULL(rbt_cmp_with_key_cb);

	node = root;
	while (node != NULL) {
		cmp = rbt_cmp_with_key_cb(node, key, rbt_cmp_data);
		if (cmp == 0)
			break;
		dir = (cmp > 0) ? 1 : 0;
		node = node->son[dir];
	}

	return node;
}

int8_t gds_rbtree_inline_del(gds_rbtree_inline_node_t **root,
	void *key, gds_rbt_cmp_with_key_cb rbt_cmp_with_key_cb,
	void *rbt_cmp_data, gds_rbt_replace_cb rbt_replace_cb,
	void *rbt_replace_data)
{
	gds_rbtree_inline_node_t head;       /* False tree root */
	gds_rbtree_inline_node_t *q, *p, *g; /* Helpers */
	gds_rbtree_inline_node_t *f = NULL;  /* Found item */
	uint8_t dir = 1;
	int8_t deleted = 0;

	GDS_CHECK_ARG_NOT_NULL(root);
	GDS_CHECK_ARG_NOT_NULL(*root);
	GDS_CHECK_ARG_NOT_NULL(rbt_cmp_with_key_cb);
	GDS_CHECK_ARG_NOT_NULL(rbt_replace_cb);

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
		cmp = rbt_cmp_with_key_cb(q, key, rbt_cmp_data);
		dir = (cmp > 0) ? 1 : 0;

		/* Save found node */
		if (cmp == 0)
			f = q;

		if (gds_rbtree_inline_node_is_red(q)
		|| gds_rbtree_inline_node_is_red(q->son[dir]))
			continue;

		/* Push the red node down */
		if (gds_rbtree_inline_node_is_red(q->son[!dir])) {
			p = p->son[last] = gds_rbtree_inline_rotate(q, dir);
		}
		else if (!gds_rbtree_inline_node_is_red(q->son[!dir])) {
			gds_rbtree_inline_node_t *s = p->son[!last];

			if (s == NULL)
				continue;

			if (!gds_rbtree_inline_node_is_red(s->son[!last])
			&& !gds_rbtree_inline_node_is_red(s->son[last])) {
				/* Color flip */
				p->red = false;
				s->red = true;
				q->red = true;
			} else {
				uint8_t dir2 = (g->son[1] == p) ? 1 : 0;

				if ( gds_rbtree_inline_node_is_red ( s->son[last] ) )
					g->son[dir2] = gds_rbtree_inline_rotate_twice(p, last);
				else if (gds_rbtree_inline_node_is_red(s->son[!last]))
					g->son[dir2] = gds_rbtree_inline_rotate(p, last);

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

		dir1 = (p->son[1] == q) ? 1 : 0;
		dir2 = (q->son[0] == NULL) ? 1 : 0;
		p->son[dir1] = q->son[dir2];

		rbt_replace_cb(f, q, rbt_replace_data);

		deleted = 1;
	}

	/* Update root and make it black */
	*root = head.son[1];
	if (*root != NULL)
		(*root)->red = false;

	return deleted;
}

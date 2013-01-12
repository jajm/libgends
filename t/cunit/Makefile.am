AM_CPPFLAGS = -I.. -I../../include -I../../src \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = -lcunit `pkg-config --libs libexception lll`

noinst_PROGRAMS = slist_node dlist_node \
	rbtree_keyin_node rbtree_keyin \
	rbtree_node rbtree \
	rbtree_keyin_fast_node rbtree_keyin_fast

slist_node_SOURCES = slist_node.c
slist_node_LDADD = ../../src/libgends.la

dlist_node_SOURCES = dlist_node.c
dlist_node_LDADD = ../../src/libgends.la

rbtree_keyin_node_SOURCES = rbtree_keyin_node.c ../test.c
rbtree_keyin_node_LDADD = ../../src/libgends.la

rbtree_keyin_SOURCES = rbtree_keyin.c ../test.c
rbtree_keyin_LDADD = ../../src/libgends.la

rbtree_node_SOURCES = rbtree_node.c ../test.c
rbtree_node_LDADD = ../../src/libgends.la

rbtree_SOURCES = rbtree.c ../test.c
rbtree_LDADD = ../../src/libgends.la

rbtree_keyin_fast_node_SOURCES = rbtree_keyin_fast_node.c ../test.c
rbtree_keyin_fast_node_LDADD = ../../src/libgends.la

rbtree_keyin_fast_SOURCES = rbtree_keyin_fast.c ../test.c
rbtree_keyin_fast_LDADD = ../../src/libgends.la

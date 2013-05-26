AM_CPPFLAGS = -I../include
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

lib_LTLIBRARIES = libgends.la

libgends_la_SOURCES = \
	inline/slist.c inline/dlist.c inline/rbtree.c inline/rbtree_fast.c \
	iterator.c \
	slist_node.c slist.c \
	dlist_node.c dlist.c \
	rbtree.c rbtree_node.c \
	rbtree_keyin.c rbtree_keyin_node.c \
	rbtree_keyin_fast.c rbtree_keyin_fast_node.c \
	hash_map.c hash_map_keyin.c

libgends_la_LDFLAGS = -lexception -llll \
	-version-info ${LIBGENDS_CUR}:${LIBGENDS_REV}:${LIBGENDS_AGE}

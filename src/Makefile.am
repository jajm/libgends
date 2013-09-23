AM_CPPFLAGS = -I../include
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

lib_LTLIBRARIES = libgends.la

libgends_la_SOURCES = \
	inline/slist.c inline/dlist.c inline/rbtree.c inline/rbtree_fast.c \
	slist.c dlist.c rbtree.c rbtree_keyin.c rbtree_keyin_fast.c \
	hash_map.c hash_map_keyin.c \
	iterator.c

libgends_la_LDFLAGS = -lexception -llll \
	-version-info ${LIBGENDS_CUR}:${LIBGENDS_REV}:${LIBGENDS_AGE}

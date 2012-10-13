AM_CPPFLAGS = -I../include/core -I../include
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

SUBDIRS = core

lib_LTLIBRARIES = libgends.la

libgends_la_SOURCES = slist_node.c slist.c \
	dlist_node.c \
	rbtree_inline.c \
	rbtree.c rbtree_node.c \
	rbtree_keyin.c rbtree_keyin_node.c \
	rbtree_keyin_fast.c rbtree_keyin_fast_node.c \
	hash_map.c hash_map_keyin.c \
	func_list.c types.c \
	typed_container.c typed_slist.c typed_dlist.c typed_stack.c \
	typed_hash_map.c

libgends_la_LIBADD = core/libcore.la
libgends_la_LDFLAGS = -lexception -llll \
	-version-info ${LIBGENDS_CUR}:${LIBGENDS_REV}:${LIBGENDS_AGE}

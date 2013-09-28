AM_CPPFLAGS = -I.. -I../../include -I../../src \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = `pkg-config --libs libexception lll`

noinst_PROGRAMS = slist.t dlist.t rbtree.t rbtree_fast.t

slist_t_SOURCES = slist.t.c
slist_t_LDADD = ../../src/libgends.la

dlist_t_SOURCES = dlist.t.c
dlist_t_LDADD = ../../src/libgends.la

rbtree_t_SOURCES = rbtree.t.c
rbtree_t_LDADD = ../../src/libgends.la

rbtree_fast_t_SOURCES = rbtree_fast.t.c
rbtree_fast_t_LDADD = ../../src/libgends.la

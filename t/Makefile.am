AM_CPPFLAGS = -I.. -I../include -I../src \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = `pkg-config --libs libexception lll`

SUBDIRS = inline

noinst_PROGRAMS = rbtree_keyin.t rbtree.t rbtree_fast.t rbtree_keyin_fast.t \
	slist.t dlist.t hash_map.t hash_map_fast.t

rbtree_keyin_t_SOURCES = rbtree_keyin.t.c test.c
rbtree_keyin_t_LDADD = ../src/libgends.la

rbtree_t_SOURCES = rbtree.t.c test.c
rbtree_t_LDADD = ../src/libgends.la

rbtree_fast_t_SOURCES = rbtree_fast.t.c test.c
rbtree_fast_t_LDADD = ../src/libgends.la

rbtree_keyin_fast_t_SOURCES = rbtree_keyin_fast.t.c test.c
rbtree_keyin_fast_t_LDADD = ../src/libgends.la

slist_t_SOURCES = slist.t.c
slist_t_LDADD = ../src/libgends.la

dlist_t_SOURCES = dlist.t.c
dlist_t_LDADD = ../src/libgends.la

hash_map_t_SOURCES = hash_map.t.c
hash_map_t_LDADD = ../src/libgends.la

hash_map_fast_t_SOURCES = hash_map_fast.t.c
hash_map_fast_t_LDADD = ../src/libgends.la

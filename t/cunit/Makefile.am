AM_CPPFLAGS = -I.. -I../../include -I../../src \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = -lcunit `pkg-config --libs libexception lll`

SUBDIRS = inline

noinst_PROGRAMS = rbtree_keyin rbtree rbtree_keyin_fast \
	slist dlist hash_map

rbtree_keyin_SOURCES = rbtree_keyin.c ../test.c
rbtree_keyin_LDADD = ../../src/libgends.la

rbtree_SOURCES = rbtree.c ../test.c
rbtree_LDADD = ../../src/libgends.la

rbtree_keyin_fast_SOURCES = rbtree_keyin_fast.c ../test.c
rbtree_keyin_fast_LDADD = ../../src/libgends.la

slist_SOURCES = slist.c
slist_LDADD = ../../src/libgends.la

dlist_SOURCES = dlist.c
dlist_LDADD = ../../src/libgends.la

hash_map_SOURCES = hash_map.c
hash_map_LDADD = ../../src/libgends.la

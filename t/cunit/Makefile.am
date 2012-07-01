AM_CPPFLAGS = -I.. -I../../include/core -I../../include
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = container slist_node slist dlist_node dlist stack \
	compact_rbtree_node compact_rbtree rbtree_node rbtree types func_list

container_SOURCES = container.c
container_LDADD = ../../src/libgends.la
container_LDFLAGS = -lcunit

slist_node_SOURCES = slist_node.c
slist_node_LDADD = ../../src/libgends.la
slist_node_LDFLAGS = -lcunit

slist_SOURCES = slist.c
slist_LDADD = ../../src/libgends.la
slist_LDFLAGS = -lcunit

dlist_node_SOURCES = dlist_node.c
dlist_node_LDADD = ../../src/libgends.la
dlist_node_LDFLAGS = -lcunit

dlist_SOURCES = dlist.c
dlist_LDADD = ../../src/libgends.la
dlist_LDFLAGS = -lcunit

stack_SOURCES = stack.c
stack_LDADD = ../../src/libgends.la
stack_LDFLAGS = -lcunit

compact_rbtree_node_SOURCES = compact_rbtree_node.c ../test.c
compact_rbtree_node_LDADD = ../../src/libgends.la
compact_rbtree_node_LDFLAGS = -lcunit

compact_rbtree_SOURCES = compact_rbtree.c ../test.c
compact_rbtree_LDADD = ../../src/libgends.la
compact_rbtree_LDFLAGS = -lcunit

rbtree_node_SOURCES = rbtree_node.c ../test.c
rbtree_node_LDADD = ../../src/libgends.la
rbtree_node_LDFLAGS = -lcunit

rbtree_SOURCES = rbtree.c ../test.c
rbtree_LDADD = ../../src/libgends.la
rbtree_LDFLAGS = -lcunit

types_SOURCES = types.c
types_LDADD = ../../src/libgends.la
types_LDFLAGS = -lcunit

func_list_SOURCES = func_list.c
func_list_LDADD = ../../src/libgends.la
func_list_LDFLAGS = -lcunit

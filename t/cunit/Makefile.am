SUBDIRS = core

AM_CPPFLAGS = -I../../src -I../../src/core
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = container slist_node slist dlist_node dlist stack

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

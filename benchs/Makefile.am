AM_CPPFLAGS = -I../src/core -I../src -I/usr/local/include/eina-1 -I/usr/local/include/eina-1/eina -I/usr/include/valgrind
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
noinst_PROGRAMS = rbtree

rbtree_SOURCES = rbtree.c
rbtree_LDADD = ../src/libgends.la
rbtree_LDFLAGS = -pthread -L/usr/local/lib -leina

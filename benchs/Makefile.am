AM_CPPFLAGS = -I../include/core -I../include `pkg-config --cflags eina lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
noinst_PROGRAMS = rbtree

rbtree_SOURCES = rbtree.c
rbtree_LDADD = ../src/libgends.la
rbtree_LDFLAGS = `pkg-config --libs eina lll`

AM_CPPFLAGS = -Ilist
AM_CFLAGS = -Wall -Wextra -std=c99

SUBDIRS = list

lib_LTLIBRARIES = libgenty.la
noinst_PROGRAMS = test

libgenty_la_SOURCES = types.c types.h \
			error.c error.h \
			generic.c generic.h

libgenty_la_LIBADD = list/liblist.la

test_SOURCES = test.c
test_LDADD = libgenty.la

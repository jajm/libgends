AM_CPPFLAGS = -Ilist
AM_CFLAGS = -Wall -Wextra -std=c99

SUBDIRS = list

lib_LTLIBRARIES = libgenty.la
noinst_PROGRAMS = test

libgenty_la_SOURCES = types.c error.c generic.c \
			types.h error.h generic.h

libgenty_la_LIBADD = list/liblist.la
libgenty_la_LDFLAGS = -version-info ${LIBGENTY_CUR}:${LIBGENTY_REV}:${LIBGENTY_AGE}

test_SOURCES = test.c
test_LDADD = libgenty.la

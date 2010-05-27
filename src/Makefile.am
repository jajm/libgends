AM_CPPFLAGS = -Ilist
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

SUBDIRS = list

lib_LTLIBRARIES = libgends.la
noinst_PROGRAMS = test

libgends_la_SOURCES = types.c error.c generic.c funcs.c \
			types.h error.h generic.h funcs.h

libgends_la_LIBADD = list/liblist.la
libgends_la_LDFLAGS = -version-info ${LIBGENDS_CUR}:${LIBGENDS_REV}:${LIBGENDS_AGE}

test_SOURCES = test.c
test_LDADD = libgends.la

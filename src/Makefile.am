AM_CFLAGS = -Wall -Wextra -Werror -std=c99

SUBDIRS = core

nobase_pkginclude_HEADERS = core/types.h core/error.h core/funcs.h \
			generic.h slist.h dlist.h
lib_LTLIBRARIES = libgends.la
noinst_PROGRAMS = test

libgends_la_SOURCES = generic.c slist.c dlist.c

libgends_la_LIBADD = core/libcore.la
libgends_la_LDFLAGS = -version-info ${LIBGENDS_CUR}:${LIBGENDS_REV}:${LIBGENDS_AGE}

test_SOURCES = test.c
test_LDADD = libgends.la

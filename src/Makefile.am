AM_CPPFLAGS = -Ilist
AM_CFLAGS = -Wall -Wextra -std=c99

nobase_pkginclude_HEADERS = list/fw_sllist.h list/vw_sllist.h \
			list/llist.h list/SLList.h \
			types.h error.h generic.h logger.h stack.h

SUBDIRS = list

lib_LTLIBRARIES = libagds.la
noinst_PROGRAMS = test

libagds_la_SOURCES = types.c types.h \
			error.c error.h \
			generic.c generic.h \
			logger.c logger.h \
			stack.c stack.h
libagds_la_LIBADD = list/liblist.la

test_SOURCES = test.c
test_LDADD = libagds.la

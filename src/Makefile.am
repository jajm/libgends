AM_CPPFLAGS = -Ilist
AM_CFLAGS = -Wall -Wextra -std=c99

SUBDIRS = list

bin_PROGRAMS = test

test_SOURCES = types.c types.h \
		error.c error.h \
		generic.c generic.h \
		logger.c logger.h \
		stack.c stack.h \
		test.c

test_LDADD = list/liblist.a

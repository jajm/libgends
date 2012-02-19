AM_CPPFLAGS = -Icore
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

SUBDIRS = core

nobase_pkginclude_HEADERS = core/types.h core/func_list.h core/func_ptr.h \
	core/log.h container.h slist_node.h slist.h dlist_node.h dlist.h \
	stack.h hash_map.h
lib_LTLIBRARIES = libgends.la

libgends_la_SOURCES = container.c slist_node.c slist.c dlist_node.c dlist.c \
	stack.c hash_map.c

libgends_la_LIBADD = core/libcore.la
libgends_la_LDFLAGS = -version-info ${LIBGENDS_CUR}:${LIBGENDS_REV}:${LIBGENDS_AGE}

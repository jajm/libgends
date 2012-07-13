if CUNIT_TESTS
SUBDIRS = cunit
endif

AM_CPPFLAGS = -I../include/core -I../include
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = hash_map ll_hash_map

hash_map_SOURCES = hash_map.c
hash_map_LDADD = ../src/libgends.la

ll_hash_map_SOURCES = ll_hash_map.c
ll_hash_map_LDADD = ../src/libgends.la

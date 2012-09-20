SUBDIRS = core
if CUNIT_TESTS
SUBDIRS += cunit
endif

AM_CPPFLAGS = -I../include/core -I../include \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = `pkg-config --libs libexception lll`

noinst_PROGRAMS = hash_map typed_hash_map

hash_map_SOURCES = hash_map.c
hash_map_LDADD = ../src/libgends.la

typed_hash_map_SOURCES = typed_hash_map.c
typed_hash_map_LDADD = ../src/libgends.la

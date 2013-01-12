if CUNIT_TESTS
SUBDIRS = cunit
endif

AM_CPPFLAGS = -I../include -I../src \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = `pkg-config --libs libexception lll`

noinst_PROGRAMS = hash_map log

hash_map_SOURCES = hash_map.c
hash_map_LDADD = ../src/libgends.la

log_SOURCES = log.c
log_LDADD = ../src/libgends.la

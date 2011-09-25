SUBDIRS = core

AM_CPPFLAGS = -I../src/core -I../src
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = hash_map

hash_map_SOURCES = hash_map.c
hash_map_LDADD = ../src/libgends.la

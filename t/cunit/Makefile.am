SUBDIRS = core

AM_CPPFLAGS = -I../../src -I../../src/core
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = container

container_SOURCES = container.c
container_LDADD = ../../src/libgends.la
container_LDFLAGS = -lcunit

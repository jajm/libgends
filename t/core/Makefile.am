AM_CPPFLAGS = -I../../include -I../../include/core `pkg-config --cflags lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = log

log_SOURCES = log.c
log_LDADD = ../../src/libgends.la
log_LDFLAGS = `pkg-config --libs lll`

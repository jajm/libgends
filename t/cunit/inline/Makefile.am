AM_CPPFLAGS = -I.. -I../../../include -I../../../src \
	`pkg-config --cflags libexception lll`
AM_CFLAGS = -Wall -Wextra -Werror -std=c99
AM_LDFLAGS = -lcunit `pkg-config --libs libexception lll`

noinst_PROGRAMS = slist

slist_SOURCES = slist.c
slist_LDADD = ../../../src/libgends.la

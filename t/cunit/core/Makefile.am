AM_CPPFLAGS = -I../../../src/core
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = types func_list

types_SOURCES = types.c
types_LDADD = ../../../src/core/libcore.la
types_LDFLAGS = -lcunit

func_list_SOURCES = func_list.c
func_list_LDADD = ../../../src/core/libcore.la
func_list_LDFLAGS = -lcunit

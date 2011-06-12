AM_CPPFLAGS = -I../../src/core
AM_CFLAGS = -Wall -Wextra -Werror -std=c99

noinst_PROGRAMS = types funcs

types_SOURCES = types.c
types_LDADD = ../../src/core/libcore.la
types_LDFLAGS = -lcunit

funcs_SOURCES = funcs.c
funcs_LDADD = ../../src/core/libcore.la
funcs_LDFLAGS = -lcunit

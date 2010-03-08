AC_PREREQ([2.65])
AC_INIT([utils], [0.01], [diwan@frankiz.eu])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AC_PROG_AWK
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_CHECK_HEADERS([stddef.h stdint.h stdio.h stdlib.h string.h])

AC_TYPE_SIZE_T

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/list/Makefile
])

AC_OUTPUT

AC_PREREQ([2.65])
AC_INIT([libgends], [0.1], [diwan@frankiz.eu])
AM_INIT_AUTOMAKE([-Wall -Werror silent-rules subdir-objects])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Libtool version info
LIBGENDS_CUR=0
LIBGENDS_REV=0
LIBGENDS_AGE=0
AC_SUBST(LIBGENDS_CUR)
AC_SUBST(LIBGENDS_REV)
AC_SUBST(LIBGENDS_AGE)

# Check for programs
AC_PROG_AWK
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_LIBTOOL
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
	libgends.pc
	src/Makefile
	src/core/Makefile
])

AM_SILENT_RULES

AC_OUTPUT

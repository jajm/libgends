AC_INIT([libgends], [0.1], [jajm@jajm.org])

LIBTOOL_CURRENT=0
LIBTOOL_REVISION=0
LIBTOOL_AGE=0
AC_SUBST(LIBTOOL_CURRENT)
AC_SUBST(LIBTOOL_REVISION)
AC_SUBST(LIBTOOL_AGE)

# Check for programs
AC_PROG_CC

AC_CHECK_HEADERS([stddef.h stdint.h stdio.h stdlib.h string.h])

AC_TYPE_SIZE_T

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

AC_CHECK_HEADER([libexception/exception.h], , AC_MSG_ERROR([libexception/exception.h not found]))
AC_CHECK_LIB([exception], [exception_throw], , AC_MSG_ERROR([required library libexception not found!]))
AC_CHECK_HEADER([lll/lll.h], , AC_MSG_ERROR([lll/lll.h not found]))
AC_CHECK_LIB([lll], [lll_vfprint], , AC_MSG_ERROR([required library lll not found!]))

AC_CONFIG_FILES([
	config.mk
	Makefile
	libgends.pc
	src/Makefile
	include/Makefile
	t/Makefile
	benchs/Makefile
])

AC_OUTPUT

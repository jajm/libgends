AC_PREREQ([2.67])
LT_PREREQ([2.2])

AC_INIT([libgends], [2.0.2], [jajm@jajm.org])
LT_INIT()

AC_SUBST(LIBTOOL_CURRENT, 1)
AC_SUBST(LIBTOOL_REVISION, 0)
AC_SUBST(LIBTOOL_AGE, 0)

# Check for programs
AC_PROG_CC

AC_CHECK_HEADERS([stddef.h stdint.h stdio.h stdlib.h string.h])

AC_TYPE_SIZE_T

AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memmove])

PKG_CHECK_MODULES(LIBEXCEPTION, [libexception])
PKG_CHECK_MODULES(LLL, [lll])

AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [enable debugging, default: no]), [], [enable_debug=no])
AS_IF([test "$enable_debug" = "yes"], AC_SUBST(DEBUG, 1))

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

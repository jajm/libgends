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

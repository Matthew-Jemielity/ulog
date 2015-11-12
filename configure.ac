#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.64])
AC_INIT([ulog], [2.0], [matthew.jemielity@gmail.com])
AC_CONFIG_SRCDIR([src/ulog.c])
AC_CONFIG_FILES([Makefile])

AM_INIT_AUTOMAKE([-Wall])

# Checks for programs.
AM_PROG_AR
AM_PROG_CC_C_O
AC_PROG_CC_C99
# AC_PROG_CC_C11

LT_INIT

# Checks for libraries.
AC_CHECK_LIB(pthread, pthread_mutex_init, [], [AC_MSG_ERROR([cannot find pthread shared library])])

# Checks for header files.
AC_CHECK_HEADERS([assert.h errno.h pthread.h stdarg.h stdbool.h stddef.h stdint.h stdio.h stdlib.h string.h time.h], [], [AC_MSG_ERROR([cannot find or include prerequisite header])])

# Checks for library functions.
AC_CHECK_FUNCS([clock_gettime], [], [AC_MSG_ERROR([cannot find clock_gettime function])])

AC_OUTPUT

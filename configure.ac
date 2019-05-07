#                                               -*- Autoconf -*-
# Copyright (C) 2019 Jo\u00EBl Kr\u00E4hemann
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([monolight],[1.0.0-PRERELEASE],[jkraehemann@gmail.com])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([monolight/config.h.in])
AC_CONFIG_HEADERS([monolight/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL

# libtool
LT_PREREQ([2.4.2])
LT_INIT
AC_SUBST([LIBTOOL_DEPS])
AC_ENABLE_SHARED

AC_SUBST([MONOLIGHT_TEST_CFLAGS])
AC_SUBST([MONOLIGHT_TEST_LDFLAGS])

AC_SUBST([MONOLIGHT_LED_CFLAGS])
AC_SUBST([MONOLIGHT_LED_LDFLAGS])

# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main], , AC_MSG_ERROR("No dl library present"))

# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main], , AC_MSG_ERROR("No math library present"))

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

# check for gtk-doc
m4_ifdef([GTK_DOC_CHECK], [
GTK_DOC_CHECK([1.14],[--flavour no-tmpl])
],[
AM_CONDITIONAL([ENABLE_GTK_DOC], false)
])

AM_EXTRA_RECURSIVE_TARGETS([monolight-docs])
AC_CONFIG_FILES([
Makefile
m4/Makefile
po/Makefile.in])

AC_OUTPUT
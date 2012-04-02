dnl $Id$
dnl config.m4 for extension c-array

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(c-array, for c-array support,
dnl Make sure that the comment is aligned:
dnl [  --with-c-array             Include c-array support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(c-array, whether to enable c-array support,
dnl Make sure that the comment is aligned:
[  --enable-c-array           Enable c-array support])

if test "$PHP_C_ARRAY" != "no"; then
  dnl # --with-c-array -> add include path
  dnl PHP_ADD_INCLUDE($C-ARRAY_DIR/include)

  dnl # --with-c-array -> check for lib and symbol presence
  dnl LIBNAME=c-array # you may want to change this
  dnl LIBSYMBOL=c-array # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $C-ARRAY_DIR/lib, C-ARRAY_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_C-ARRAYLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong c-array lib version or lib not found])
  dnl ],[
  dnl   -L$C-ARRAY_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(C-ARRAY_SHARED_LIBADD)

  PHP_NEW_EXTENSION(c_array, c_array.c, $ext_shared)
fi

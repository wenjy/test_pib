dnl config.m4 for extension test_pib

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([test_pib],
dnl   [for test_pib support],
dnl   [AS_HELP_STRING([--with-test_pib],
dnl     [Include test_pib support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([test_pib],
  [whether to enable test_pib support],
  [AS_HELP_STRING([--enable-test_pib],
    [Enable test_pib support])],
  [no])

if test "$PHP_TEST_PIB" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, TEST_PIB_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-test_pib -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/test_pib.h"  # you most likely want to change this
  dnl if test -r $PHP_TEST_PIB/$SEARCH_FOR; then # path given as parameter
  dnl   TEST_PIB_DIR=$PHP_TEST_PIB
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for test_pib files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       TEST_PIB_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$TEST_PIB_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the test_pib distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-test_pib -> add include path
  dnl PHP_ADD_INCLUDE($TEST_PIB_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-test_pib -> check for lib and symbol presence
  dnl LIBNAME=TEST_PIB # you may want to change this
  dnl LIBSYMBOL=TEST_PIB # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_TEST_PIB_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your test_pib library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $TEST_PIB_DIR/$PHP_LIBDIR, TEST_PIB_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_TEST_PIB_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your test_pib library.])
  dnl ],[
  dnl   -L$TEST_PIB_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(TEST_PIB_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_TEST_PIB, 1, [ Have test_pib support ])

  PHP_NEW_EXTENSION(test_pib, test_pib.c, $ext_shared)
fi

dnl $Id$
dnl config.m4 for extension signal_handler

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(signal_handler, for signal_handler support,
dnl Make sure that the comment is aligned:
[  --with-signal-handler             Include signal_handler support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(signal_handler, whether to enable signal_handler support,
dnl Make sure that the comment is aligned:
dnl [  --enable-signal_handler           Enable signal_handler support])

if test "$PHP_SIGNAL_HANDLER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-signal_handler -> check with-path
  SEARCH_PATH="/usr/local /usr"     # you might want to change this
  SEARCH_FOR="/include/signal_handler.h"  # you most likely want to change this
  if test -r $PHP_SIGNAL_HANDLER/$SEARCH_FOR; then # path given as parameter
     SIGNAL_HANDLER_DIR=$PHP_SIGNAL_HANDLER
  else # search default path list
    AC_MSG_CHECKING([for signal_handler files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
         SIGNAL_HANDLER_DIR=$i
         AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  dnl if test -z "$SIGNAL_HANDLER_DIR"; then
  dnl  AC_MSG_RESULT([not found])
  dnl  AC_MSG_ERROR([Please reinstall the signal_handler distribution])
  dnl fi

  dnl # --with-signal_handler -> add include path
  dnl PHP_ADD_INCLUDE($SIGNAL_HANDLER_DIR/include)

  dnl # --with-signal_handler -> check for lib and symbol presence
  dnl LIBNAME=signal_handler # you may want to change this
  dnl LIBSYMBOL=signal_handler # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SIGNAL_HANDLER_DIR/lib, SIGNAL_HANDLER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SIGNAL_HANDLERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong signal_handler lib version or lib not found])
  dnl ],[
  dnl   -L$SIGNAL_HANDLER_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SIGNAL_HANDLER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(signal_handler, signal_handler.c, $ext_shared)
fi

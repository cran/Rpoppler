AC_INIT(Rpoppler, 0.0, Kurt.Hornik@R-project.org)
AC_CONFIG_SRCDIR([src/Rpoppler.c])
AC_PATH_PROG([PKG_CONFIG], [pkg-config])
if test -z "${PKG_CONFIG}"; then
  AC_MSG_ERROR([cannot find pkg-config])
fi
AC_MSG_CHECKING([whether pkg-config knows about poppler-glib >= 0.18.0])
if "${PKG_CONFIG}" --exists 'poppler-glib >= 0.18.0'; then
  AC_MSG_RESULT([yes])
  PKG_CFLAGS=`"${PKG_CONFIG}" --cflags poppler-glib`
  PKG_CXXFLAGS=`"${PKG_CONFIG}" --cflags poppler-glib`
  PKG_LIBS=`"${PKG_CONFIG}" --libs poppler-glib`
  AC_SUBST(PKG_CFLAGS)
  AC_SUBST(PKG_CXXFLAGS)
  AC_SUBST(PKG_LIBS)
else
  AC_MSG_RESULT([no])
  AC_MSG_ERROR([cannot determine poppler-glib compile/link flags])
fi

: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
exit 1
fi
CXX=`"${R_HOME}/bin/R" CMD config CXX`
CXXFLAGS=`"${R_HOME}/bin/R" CMD config CXXFLAGS`

AC_PROG_CXX

CXXFLAGS="${CXXFLAGS} ${PKG_CXXFLAGS}"
LIBS="${LIBS} ${PKG_LIBS} -lpoppler"

AC_LANG(C++)
AC_MSG_CHECKING([whether we can use setErrorFunction])
AC_LINK_IFELSE([AC_LANG_PROGRAM(
[[#include <poppler.h>
extern void setErrorFunction(void (* f)(int , char *, va_list args));]],
[[setErrorFunction(0);]])],
               [have_setErrorFunction=yes],
               [have_setErrorFunction=no])
AC_MSG_RESULT([${have_setErrorFunction}])
if test "${have_setErrorFunction}" = "yes"; then
  AC_DEFINE([HAVE_SET_ERROR_FUNCTION], [1],
            [Define if we can use setErrorFunction])
  PKG_LIBS="${PKG_LIBS} -lpoppler"
else
  AC_MSG_CHECKING([whether we can use setErrorCallback])
  AC_LINK_IFELSE([AC_LANG_PROGRAM(
[[enum ErrorCategory { errSyntaxWarning };
extern void setErrorCallback(void (*cbk)(void *data, ErrorCategory category,
                                         int pos, char *msg),
                             void *data);]],
[[setErrorCallback(0,0);]])],
                 [have_setErrorCallback=yes],
                 [have_setErrorCallback=no])
  AC_MSG_RESULT([${have_setErrorCallback}])
  if test "${have_setErrorCallback}" = "yes"; then
    AC_DEFINE([HAVE_SET_ERROR_CALLBACK], [1],
              [Define if we can use setErrorCallback])
    PKG_LIBS="${PKG_LIBS} -lpoppler"    
  fi
fi

AC_CONFIG_FILES([src/Makevars])
AC_CONFIG_HEADERS([src/config.h])
AC_OUTPUT

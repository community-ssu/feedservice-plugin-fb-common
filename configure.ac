AC_INIT([feedservice-plugin-fb-common], [0.0.0], [])

# NOTE: Must the LT version must be changed when code is released:
#  - If interfaces changed/added/removed: lt_current++, lt_rev=0
#  - If interfaces added: lt_age++
#  - If Interfaces removed: lt_age=0
#  - If only code changes (interface untouched): lt_rev++
facebookcommon_lt_current=0
facebookcommon_lt_rev=0
facebookcommon_lt_age=0
AC_SUBST([FACEBOOKCOMMON_LT_VERSION],[$facebookcommon_lt_current:$facebookcommon_lt_re$facebookcommon_lt_age])

AM_INIT_AUTOMAKE

# AC_CONFIG_SRCDIR([src/facebookcommon.c])
# FIXME: start to use config.h
#AC_CONFIG_HEADERS([config.h])

AC_DEFINE(_GNU_SOURCE)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_LIBTOOL

# Specify dir for local macros
AC_CONFIG_MACRO_DIR([m4])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T

PKG_CHECK_MODULES(DEPS, [glib-2.0 gconf-2.0 >= 2.16 libxml-2.0 >= 2.6.32 conic feedservice2])
PKG_CHECK_MODULES(QT4, [QtCore QtGui QtWebKit >= 4.4.0], [
  AC_PATH_PROGS(MOC, [moc-qt4 moc], moc,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
  AC_PATH_PROG(RCC, rcc, rcc,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
  AC_PATH_PROGS(UIC, [uic-qt4 uic], uic,`eval $PKG_CONFIG --variable=exec_prefix QtCore`/bin)
], [
  AC_MSG_ERROR([Qt 4 library not found])
])

AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

AC_CONFIG_FILES([
Makefile 
src/Makefile
src/common/Makefile
src/oauth/Makefile
src/common/facebookcommon.pc])

AC_OUTPUT

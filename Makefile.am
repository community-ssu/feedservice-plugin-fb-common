ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -Wall -fPIC -I@top_srcdir@/include $(DEPS_CFLAGS)

pkgconfigdir = @libdir@/pkgconfig
facebookincludedir = @includedir@/facebook

lib_LTLIBRARIES = libfacebookcommon.la

libfacebookcommon_la_SOURCES = src/facebookcommon.c
libfacebookcommon_la_LDFLAGS = -version-info $(FACEBOOKCOMMON_LT_VERSION)
libfacebookcommon_la_LIBADD = $(DEPS_LIBS)

facebookinclude_HEADERS = include/facebook/common.h

pkgconfig_DATA = facebookcommon.pc

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = facebookcommonoauth

QMAKE_TMP=oauth.pro
QT4_QMAKE_DEFINES=$(shell touch $(QMAKE_TMP) && qmake $(QMAKE_TMP) -o - CONFIG+=debug | grep DEFINES | grep -v CFLAGS | grep -v CXXFLAGS | cut -d= -f2- && rm $(QMAKE_TMP))

facebookcommonoauth_CPPFLAGS = $(QT4_QMAKE_DEFINES) $(QT4_CFLAGS)
facebookcommonoauth_LDFLAGS  = $(QT4_LIBS)

facebookcommonoauth_SOURCES = oauth.cpp qfacebooklogin.cpp qfacebooklogin.h moc_qfacebooklogin.cpp

moc_%.cpp : %.h
	$(MOC) $(QT4_CFLAGS) $(QT4_QMAKE_DEFINES) -I. -o "$@" "$<"

ui_%.h : %.ui
	$(UIC) -o "$@" "$<"

clean-qt:
	rm -vf moc_*.cpp
	rm -vf ui_*.h
	rm -rf .libs _libs
	rm -f *.o
	rm -f *.lo
	rm -f $(bin_PROGRAMS)

clean-am: clean-qt


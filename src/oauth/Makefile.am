ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = facebookcommonoauth

facebookcommonoauth_CPPFLAGS = $(QT4_CFLAGS)
facebookcommonoauth_LDFLAGS  = $(QT4_LIBS)

facebookcommonoauth_SOURCES = oauth.cpp qfacebooklogin.cpp qfacebooklogin.h moc_qfacebooklogin.cpp

moc_%.cpp : %.h
	$(MOC) $(QT4_CFLAGS) -I. -o "$@" "$<"

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


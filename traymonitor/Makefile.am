
AM_CFLAGS = $(JWMT_CFLAGS)
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS)

bin_PROGRAMS = traymonitor

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = traymonitor

traymonitor_SOURCES = traymonitor.c

dist_pkgdata_DATA = traymonitor.glade

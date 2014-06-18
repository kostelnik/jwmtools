
AM_CFLAGS = $(JWMT_CFLAGS)
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = -lmetar $(JWMT_LIBS)

bin_PROGRAMS = trayweather

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = trayweather

trayweather_SOURCES = trayweather.c

dist_pkgdata_DATA = trayweather.glade

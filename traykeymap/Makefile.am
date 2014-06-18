
AM_CFLAGS = $(JWMT_CFLAGS)
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS)

bin_PROGRAMS = traykeymap

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = traykeymap

traykeymap_SOURCES = traykeymap.c

dist_pkgdata_DATA = traykeymap.glade


AM_CFLAGS = $(JWMT_CFLAGS)
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS)

bin_PROGRAMS = trayscript

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = trayscript

trayscript_SOURCES = trayscript.c

dist_pkgdata_DATA = trayscript.glade

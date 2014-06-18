
AM_CFLAGS = $(JWMT_CFLAGS) -I$(top_srcdir)/libonce
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS)

bin_PROGRAMS = xidletime xidlerun

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = xidletime xidlerun

xidletime_SOURCES = xidletime.c
xidlerun_SOURCES = xidlerun.c

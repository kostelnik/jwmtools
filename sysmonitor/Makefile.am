
AM_CFLAGS = $(JWMT_CFLAGS)
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS)

bin_PROGRAMS = sysmonitor

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = sysmonitor

sysmonitor_SOURCES = sysmonitor.c sysmonitor-data.h

EXTRA_DIST = sysmonitor-data.c

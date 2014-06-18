
AM_CFLAGS = $(JWMT_CFLAGS) -I$(top_srcdir)/libonce
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS) -lcrypt

bin_PROGRAMS = slock

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = slock

slock_SOURCES = slock.c

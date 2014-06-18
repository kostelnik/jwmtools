
AM_CFLAGS = $(JWMT_CFLAGS)
AM_CPPFLAGS = -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
AM_LDFLAGS = $(JWMT_LIBS)

EXTRA_PROGRAMS = scandir_demo

AM_INSTALLCHECK_STD_OPTIONS_EXEMPT = scandir_demo

scandir_demo_SOURCES = scandir_demo.c scandirstr.h

EXTRA_DIST = scandirstr.c

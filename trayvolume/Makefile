include ../config.mk

PROGS=pavolumeup pavolumedown alsavolumeup alsavolumedown trayvolume-alsa trayvolume-pulse

CC = gcc
CFLAGS  = -I. -ggdb
LIBS    = -L. -L/usr/lib
CFLAGS += -Wall -Werror -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -fno-strict-aliasing
CFLAGS += `pkg-config --cflags cairo pango pangocairo gtk+-2.0 gmodule-2.0 gmodule-export-2.0 libglade-2.0`
CFLAGS += -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
LIBS   += `pkg-config --libs cairo pango pangocairo gtk+-2.0 gmodule-2.0 gmodule-export-2.0 libglade-2.0`
LIBS2   =

all: $(PROGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LIBS) $^ $(LIBS2) -o $@
#	strip $@

clean:
	rm -f $(PROGS) *.o



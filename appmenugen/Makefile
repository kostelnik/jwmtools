include ../config.mk

PROGS=appmenugen

CC = g++
CFLAGS  = -I. -ggdb
LIBS    = -L. -L/usr/lib
CFLAGS += -Wall -Wmissing-declarations -fno-strict-aliasing
CFLAGS += `pkg-config --cflags glib-2.0 x11`
CFLAGS += -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
LIBS   += `pkg-config --libs glib-2.0 x11`
LIBS2   =

all: $(PROGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LIBS) $(LIBS2) -o $@
#	strip $@

clean:
	rm -f $(PROGS) *.o



include ../config.mk

PROGS=movemousecursor

CC = gcc
CFLAGS  = -I. -ggdb
LIBS    = -L. -L/usr/lib
CFLAGS += -Wall -Werror -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -fno-strict-aliasing
CFLAGS += `pkg-config --cflags x11`
CFLAGS += -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\"
LIBS   += `pkg-config --libs x11`
LIBS2   =

all: $(PROGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LIBS) $(LIBS2) -o $@
#	strip $@

clean:
	rm -f $(PROGS) *.o



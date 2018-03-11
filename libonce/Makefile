include ../config.mk

PROGS=demo

CC = gcc
CFLAGS  = -I. -ggdb
LIBS    = -L. -L/usr/lib
CFLAGS += -Wall -Werror -Wpointer-arith -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -fno-strict-aliasing
CFLAGS += `pkg-config --cflags glib-2.0`
CFLAGS += -DVERSION=\"${VERSION}\" -DPREFIX=\"${PREFIX}\" -I../../jwmtools/s
LIBS   += `pkg-config --libs glib-2.0`
LIBS2   =

all: $(PROGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LIBS) $^ $(LIBS2) -o $@
#	strip $@

clean:
	rm -f $(PROGS) *.o

install:
#	cp $(PROGS) ${PREFIX}/bin
	cp *.so ${PREFIX}/share/lib/
	cp *.a ${PREFIX}/share/lib/
  

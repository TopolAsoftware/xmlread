SRCNAME=xmlread
BINNAME=$(SRCNAME)

CC=gcc
CFLAGS= -O2 -Wall -I/usr/include/libxml2
LIBS= -lxml2

SOURCES := $(wildcard *.c)
OBJ := $(patsubst %.c, %.o, $(SOURCES))

.c.o:
	$(CC) -c $(CFLAGS) $<

all: $(BINNAME)

$(BINNAME): $(OBJ) Makefile
	$(CC) -o $(BINNAME) $(CFLAGS) $(LIBS) $(OBJ)

clean:
	rm -f *.o *.cgi *~ core *.b $(BINNAME)

dep: clean

check: test
test: all
	@./xmlread payment.xml

v2_iconv.o: v2_iconv.c
xmlread.o: xmlread.c v2_iconv.h

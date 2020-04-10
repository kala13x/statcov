CFLAGS = -g -O2 -Wall -D_XSOCK_USE_SSL
LIBS = -lssl -lcrypto

OBJS = version.o \
	nbg.o \
	sock.o \
	statcov.o \
	http.o \
	covid.o

BUILD = bin
INSTALL = /usr/bin/
VPATH = src

.c.o:
	$(CC) $(CFLAGS) -c $< $(LIBS)

statcov: $(OBJS)
	$(CC) $(CFLAGS) -o statcov $(OBJS) $(LIBS)
	@test -d $(BUILD) || mkdir $(BUILD)
	@install -m 0755 statcov $(BUILD)/

.PHONY: install
install:
	@test -d $(INSTALL) || mkdir $(INSTALL)
	@install -m 0755 statcov $(INSTALL)/

.PHONY: clean
clean:
	$(RM) statcov $(OBJS)

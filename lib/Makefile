CFLAGS=-fPIC

all: libpiimg.a piimg.h

libpiimg.a: command.o loopdev.o partition.o fstr.o dd.o privileges.o
	ar rs $@ $^

piimg.h: command.h loopdev.h partition.h fstr.h dd.h privileges.h
	cat $^ > $@

command.o: command.c
loopdev.o: loopdev.c
partition.o: partition.c
fstr.o: fstr.c
dd.o: dd.c
privileges.o: privileges.c

clean:
	\rm *.o *.a piimg.h

.PHONY: clean all

# vim: set noexpandtab:

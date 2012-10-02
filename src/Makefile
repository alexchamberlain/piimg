LIBS=../lib/libpiimg.a -lparted
INC=-I../lib
CFLAGS=-g $(INC)

all: piimg

piimg.o: piimg.c

piimg-list.o: piimg-list.c
piimg-loopdev.o: piimg-loopdev.c
piimg-mount.o: piimg-mount.c
piimg-umount.o: piimg-umount.c
piimg-grow.o: piimg-grow.c
piimg-dd.o: piimg-dd.c

piimg: piimg.o piimg-list.o piimg-loopdev.o piimg-mount.o piimg-umount.o piimg-grow.o piimg-dd.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm *.o piimg

install:
	install -oroot -gusers -m4750 ./piimg /usr/local/bin

# vim: set noexpandtab:

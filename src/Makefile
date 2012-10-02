LIBS=-lparted
CFLAGS=-g

all: piimg

piimg.o: piimg.c

piimg-list.o: piimg-list.c
piimg-loopdev.o: piimg-loopdev.c
piimg-mount.o: piimg-mount.c
piimg-umount.o: piimg-umount.c
piimg-grow.o: piimg-grow.c
piimg-dd.o: piimg-dd.c

# Future library
command.o: command.c
loopdev.o: loopdev.c
partition.o: partition.c
fstr.o: fstr.c
dd.o: dd.c

piimg: piimg.o piimg-list.o piimg-loopdev.o piimg-mount.o piimg-umount.o piimg-grow.o piimg-dd.o command.o loopdev.o partition.o fstr.o dd.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm *.o piimg

install:
	cp ./piimg /usr/local/bin

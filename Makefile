LIBS=-lparted
CFLAGS=-g

all: piimg

piimg.o: piimg.c

piimg-list.o: piimg-list.c
piimg-loopdev.o: piimg-loopdev.c
piimg-mount.o: piimg-mount.c
piimg-grow.o: piimg-grow.c

# Future library
command.o: command.c
loopdev.o: loopdev.c
partition.o: partition.c
fstr.o: fstr.c

piimg: piimg.o piimg-list.o piimg-loopdev.o piimg-mount.o piimg-grow.o command.o loopdev.o partition.o fstr.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm *.o piimg

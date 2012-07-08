LIBS=-lparted
CFLAGS=-g

all: piimg

piimg.o: piimg.c
piimg-list.o: piimg-list.c
piimg-loopdev.o: piimg-loopdev.c
command.o: command.c
loopdev.o: loopdev.c

piimg: piimg.o piimg-list.o piimg-loopdev.o command.o loopdev.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

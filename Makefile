LIBS=-lparted
CFLAGS=-g

piimg.o: piimg.c
piimg-list.o: piimg-list.c
piimg-loopdev.o: piimg-loopdev.c
loopdev.o: loopdev.c

piimg: piimg.o piimg-list.o piimg-loopdev.o loopdev.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

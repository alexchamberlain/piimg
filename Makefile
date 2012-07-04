LIBS=-lparted
CFLAGS=-g

piimg.o: piimg.c
piimg-list.o: piimg-list.c
loopdev.o: loopdev.c

piimg: piimg.o piimg-list.o loopdev.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

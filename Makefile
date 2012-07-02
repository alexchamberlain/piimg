LIBS=-lparted
CFLAGS=-g

piimg.o: piimg.c
piimg-list.o: piimg-list.c

piimg: piimg.o piimg-list.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

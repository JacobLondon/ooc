TARGET=test
CC=gcc
CFLAGS=-pipe -march=native -O2 -lm -ggdb
OBJS=\
	boolean.o \
	class.o \
	dict.o \
	integer.o \
	iterator.o \
	json.o \
	list.o \
	main.o \
	real.o \
	string.o \
	util.o \
	vector.o

.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o *.out $(TARGET) $(OBJS)

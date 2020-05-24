TARGET=test
CC=gcc
CFLAGS=-pipe -march=native -O2 -lm
OBJS=\
	boolean.o \
	class.o \
	dict.o \
	integer.o \
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

TARGET=test
CC=gcc
CFLAGS=-pipe -march=native -O2
OBJS=\
	class.o \
	dict.o \
	main.o \
	string.o \
	util.o

.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o *.out $(TARGET) $(OBJS)

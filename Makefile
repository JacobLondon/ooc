TARGET=ooctest
CC=gcc
CFLAGS=-pipe -march=native -Iinclude -O0 -lm -ggdb
OBJS=\
	src/boolean.o \
	src/class.o \
	src/dict.o \
	src/integer.o \
	src/iterator.o \
	src/json.o \
	src/list.o \
	src/main.o \
	src/os.o \
	src/real.o \
	src/string.o \
	src/util.o \
	src/vector.o

.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf *.o *.out $(TARGET) $(OBJS)

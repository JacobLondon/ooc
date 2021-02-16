TARGET=libooc.a
TEST=otest
CC=gcc
#CFLAGS=-pipe -march=native -Iinclude -O2 -lm
CFLAGS=-pipe -march=native -Iinclude -O0 -lm -ggdb
OBJS=\
	src/boolean.o \
	src/class.o \
	src/dict.o \
	src/integer.o \
	src/iterator.o \
	src/json.o \
	src/list.o \
	src/os.o \
	src/real.o \
	src/string.o \
	src/util.o \
	src/vector.o
TESTOBJS=\
	src/main.o

.PHONY: clean

$(TARGET): $(OBJS)
	ar rcs $@ $^

test: $(TESTOBJS) $(TARGET)
	$(CC) -o $(TEST) $^

clean:
	rm -rf *.a *.o *.out $(TARGET) $(TEST) $(OBJS) $(TESTOBJS)

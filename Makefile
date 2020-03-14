TARGET=test
CC=gcc
OBJS=\
	class.o \
	dict.o \
	main.o \
	string.o

.PHONY: clean

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

clean:
	rm -rf *.o *.out $(TARGET) $(OBJS)

SOURCES=$(wildcard *.c)
OBJS=$(SOURCES:.c=.o)

TARGET=../lib/libmock.a

all: $(TARGET)

%.o:%.c
	gcc -std=c99 -Wall -c -o $@ $^

$(TARGET): $(OBJS) ../lib
	ar rcs $(TARGET) $(OBJS)

../lib:
	mkdir ../lib

clean:
	rm -rf $(OBJS) $(TARGET)

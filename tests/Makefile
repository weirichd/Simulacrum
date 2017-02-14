TARGET = test_suite
SRCS = test_suite.c acceptance_test_library.c
OBJS = $(SRCS:.c=.o)
LIB=../lib/libmock.a

CFLAGS=-std=c99 -Wall

all: $(TARGET)

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $^

$(TARGET): $(OBJS) $(LIB)
	gcc $(CFLAGS) -o $@ $^ `pkg-config --cflags --libs check` -Wl,--wrap=malloc

$(LIB):
	$(MAKE) -C ../lib

clean:
	rm -f $(TARGET) $(OBJS)

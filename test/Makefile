OUT:=test
SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))

CC:=gcc
$(OUT):$(OBJS)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) -c -g $^ -o $@ -pthread
clean:
	$(RM) $(OBJS) $(OUT)
rebuild: clean main

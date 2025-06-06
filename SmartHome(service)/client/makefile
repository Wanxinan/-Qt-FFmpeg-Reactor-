SRCS := $(wildcard *.c)
OUTS := $(patsubst %.c,%,$(SRCS))
CC := gcc
COM_OP := -Wall -g -pthread

.PHONY: clean rebuild all

all: $(OUTS)                      
% : %.c
	$(CC) $^ -o $@ $(COM_OP)

clean:
	$(RM) $(OUTS)

rebuild: clean all

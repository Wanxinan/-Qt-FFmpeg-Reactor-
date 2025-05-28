
# 定义变量
SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc,%, $(SRCS))

all:$(OBJS)

%:%.cc
	g++ -o $@ $^

clean:
	rm -rf $(OBJS)

# 打印变量的信息
echo:
	@echo $(SRCS)
	@echo $(OBJS)

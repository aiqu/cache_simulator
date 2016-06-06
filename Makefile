EXTRA_CFLAGS = 
#RUN = ./cache astar 64 1024 1 1 4
#RUN = ./cache astar 64 1 1024 1 4
#RUN = ./cache astar 64 8 128
#RUN = ./cache astar 64 2 2048
RUN = 

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
DEPFILE=.dep
TARGET=cache
CC=g++

all:	$(TARGET)
	@$(RUN)

$(TARGET): $(OBJS)
	@$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $(OBJS)

dep:
	$(CC) -M $(SRCS) > $(DEPFILE)

clean:
	rm -rf $(TARGET) $(OBJS)

debug:	EXTRA_CFLAGS+=-DDEBUG
debug: 	cache
	@$(RUN)


.PHONY: all debug clean

ifeq ($(DEPFILE), $(wildcard $(DEPFILE)))
include $(DEPFILE)
endif

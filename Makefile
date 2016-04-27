EXTRA_CFLAGS = 
RUN = ./cache astar 64 1024 1
#RUN = ./cache astar 64 1 1024 1 4
#RUN = ./cache astar 64 8 128
#RUN = ./cache astar 64 2 512
#RUN = 

all:	cache
	@$(RUN)

cache:	cache.o
	@gcc $(CFLAGS) $(EXTRA_CFLAGS) -o cache cache.c

clean:
	rm -rf cache cache.o

debug:	EXTRA_CFLAGS+=-DDEBUG
debug: 	cache
	@$(RUN)

lru:	EXTRA_CFLAGS+=-DLRU
lru:	cache
	@$(RUN)


.PHONY: all lru debug clean cache

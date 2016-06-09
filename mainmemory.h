#include "cache.h"
#include <set>

#ifndef MAINMEMORY_H
#define MAINMEMORY_H

const int PAGE_SIZE = 4 * 1024;
const int DRAM_L = PAGE_SIZE;
const int DRAM_K = 1;
const int DRAM_NS = 1;
const int DRAM_NV = 4;

const int DRAM_SIZE = 40 * 1024 * 1024;
const int NAND_SIZE = 160 * 1024 * 1024;
const int WRITEBUFFER_SIZE = 4;

typedef struct nand_t {
	int write_time;
	int read_time;
	std::set<uint64_t> mem;
} nand_t;

typedef struct mainmemory {
	cache dram;
	nand_t nand;
	int init;
} mainmemory;

int init_mainmemory(mainmemory*, int, int, int);
void free_mainmemory(mainmemory*);
void print_mainmemory(mainmemory*);

int mainmemory_access(mainmemory*, uint64_t, int);

void fetch_nand(mainmemory*, uint64_t, int);
void put_nand(mainmemory*, uint64_t);

#endif

#include "mainmemory.h"
#include "main.h"
#include "cache.h"

#include <stdio.h>
#include <stdlib.h>

int PAGE_SIZE = 0;
int DRAM_L = DEFAULT_PAGE_SIZE;

//size in byte
int init_mainmemory(mainmemory* target, int dram_size, int nand_size, int writebuffer_size){
	if(PAGE_SIZE){
		DRAM_L = PAGE_SIZE;
	}else{
		PAGE_SIZE = DEFAULT_PAGE_SIZE;
	}
	if(flag_debug)
		printf("PAGE SIZE is %d\n", PAGE_SIZE);
	if(flag_fullassociative){
		int dram_k = dram_size / DRAM_L;
		initialize_cache(&target->dram, DRAM, DRAM_L, dram_k, 1);
	}else{
		if(dram_size % (DRAM_L * DRAM_K)){
			printf("%s fail, dram size fault\n", __func__);
			return 1;
		}
		int dram_n = dram_size / (DRAM_L * DRAM_K);
		dram_n -= DRAM_NS + DRAM_NV;
		if( dram_n % DRAM_K ){
			printf("%s fail, dram_n size fault\n", __func__);
			return 1;
		}
		initialize_cache(&(target->dram), DRAM, DRAM_L, DRAM_K, dram_n, DRAM_NS, DRAM_NV);
	}
	target->dram.name = "DRAM";
	target->dram.lower_cache = NULL;
	target->dram.writebuffer_size = writebuffer_size;

	target->nand.write_time = 200000;
	target->nand.read_time = 25000;

	target->init = 0;
	return 0;
}

void free_mainmemory(mainmemory* target){
	if(target->init)
		return;
	free_cache(&(target->dram));
}

void print_mainmemory(mainmemory* target){
	if(target->init)
		return;
	print_cache(&target->dram);

	check_nand(target);
}

int mainmemory_access(mainmemory* target, uint64_t addr, int op){
	if(target->init)
		return 1;
	cache* c = &target->dram;
	cache_access(c, addr, op);
	return 0;	
}

void fetch_nand(mainmemory *target, uint64_t addr, int op){
	if(target->init)
		return;
	execution_time += target->nand.read_time;
}

void put_nand(mainmemory* target, uint64_t addr){
	if(target->init)
		return;
	target->nand.mem.insert(addr);
}

void check_nand(mainmemory* target){
	if(target->init)
		return;
	printf("%lu page, %luKB(%luMB) used\n", target->nand.mem.size(), target->nand.mem.size() * PAGE_SIZE / 1024, target->nand.mem.size() * PAGE_SIZE / 1024 / 1024);
}

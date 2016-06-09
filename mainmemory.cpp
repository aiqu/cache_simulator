#include "mainmemory.h"
#include "main.h"
#include "cache.h"

#include <stdio.h>
#include <stdlib.h>

//size in byte
int init_mainmemory(mainmemory* target, int dram_size, int nand_size){
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
	return 0;
}

void free_mainmemory(mainmemory* target){
	free_cache(&(target->dram));
}

void print_mainmemory(mainmemory* target){
	print_cache(&target->dram);
}

int mainmemory_access(mainmemory* target, int64_t addr, int op){
	cache* c = &target->dram;
	cache_access(c, addr, op);
	return 0;	
}

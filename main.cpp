#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "main.h"
#include "cache.h"
#include "mainmemory.h"

using namespace std;

int flag_debug = 0;
int flag_fullassociative = 0;
int flag_streambuffer = 0;
int flag_writel2 = 0;
int flag_writedram = 0;
int flag_printdram = 0;
int flag_nandsize = 0;
char* outfile_name = NULL;
int64_t execution_time = 0;
unsigned int ir_count = 0;
unsigned int dr_count = 0;
unsigned int dw_count = 0;
unsigned int unknown_count = 0;

cache L1_dcache, L1_icache;
cache L2_cache;
mainmemory mm;

int main(int argc, char* argv[]){
	char* inputfile_name = NULL;
	int c;
	int dram_size, nand_size, writebuffer_size;
	dram_size = DRAM_SIZE;
	nand_size = NAND_SIZE;
	writebuffer_size = WRITEBUFFER_SIZE;
	while( (c=getopt(argc, argv, "f:dFwWD:P:Z:Mb:")) != -1 ){
		switch(c){
		case 'f':
			inputfile_name = optarg;
			break;
		case 'd':
			flag_debug = 1;
			break;
		case 'F':
			flag_fullassociative = 1;
			break;
		case 'S':
			flag_streambuffer = 1;
			break;
		case 'w':
			flag_writel2 = 1;
			break;
		case 'W':
			flag_writedram = 1;
			break;
		case 'D':// in mb
			dram_size = atoi(optarg) * 1024 * 1024;
			break;
		case 'P':// in kb
			PAGE_SIZE = atoi(optarg) * 1024;
			break;
		case 'Z':
			flag_printdram = 1;
			outfile_name = optarg;
			break;
		case 'M':
			flag_nandsize = 1;
			break;
		case 'b':
			writebuffer_size = atoi(optarg);
			break;
		default:
			printf("Unknown Parameter %c\n", c);
			break;
		}
	}

	srand(time(NULL));

	initialize_cache(&L1_icache, L1, L1_L, L1_K, L1_N, L1_NS, L1_NV);
	initialize_cache(&L1_dcache, L1, L1_L, L1_K, L1_N, L1_NS, L1_NV);
	initialize_cache(&L2_cache, L2, L2_L, L2_K, L2_N);
	L1_icache.lower_cache = &L2_cache;
	L1_dcache.lower_cache = &L2_cache;
	L2_cache.lower_cache = NULL;

	L1_icache.name = "L1_icache";
	L1_dcache.name = "L1_dcache";
	L2_cache.name = "L2_cache";

	mm.init = 1;
	if(init_mainmemory(&mm, dram_size, nand_size, writebuffer_size)){
		return 0;
	}

	do_simulation(inputfile_name);
	print_result();

	cleanup();

	return 0;
}
void cleanup(){
	free_cache(&L1_icache);
	free_cache(&L1_dcache);
	free_cache(&L2_cache);
	free_mainmemory(&mm);
}

void do_simulation(const char* input_file){
	FILE *input = fopen(input_file, "r");
	int type;
	uint64_t addr;
	if(!input){
		printf("Cannot open input file\n");
		return;
	}

	unsigned long count = 0;
	do{
		fscanf(input, "%d %lx\n", &type, &addr);
		//if(flag_debug)
			//printf("type: %d, addr: %lx\n", type, addr);
		//statistics(type);
		if(type == 2)
			cache_access(&L1_icache, addr, type);
		else
			cache_access(&L1_dcache, addr, type);
	}while(!feof(input));

	fclose(input);
}

void statistics(int type){
	switch(type){
	case 0:
		dr_count++;
		break;
	case 1:
		dw_count++;
		break;
	case 2:
		ir_count++;
		break;
	case 3:
		dw_count++;
		break;
	default:
		unknown_count++;
		break;
	}
}

void print_result(){
	//printf("\n----Operation Summary\n");
	//printf("Total Count: %u\n\tInstruction read: %u\tData Read: %u\tData Write: %u\n",
			//ir_count+dr_count+dw_count,ir_count, dr_count, dw_count);
	printf("\n Total Execution Time:\t%" PRId64 "\n", execution_time);
	//printf("\n-L1 data cache\n");
	print_cache(&L1_dcache);
	//printf("\n-L1 instruction cache\n");
	print_cache(&L1_icache);
	//printf("\n-L2 cache\n");
	print_cache(&L2_cache);

	//printf("\n-MainMemory\n");
	print_mainmemory(&mm);
}

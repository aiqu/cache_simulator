#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "main.h"
#include "cache.h"

using namespace std;

int flag_debug = 0;

cache L1_dcache, L1_icache;
cache L2_cache;

int main(int argc, char* argv[]){

	char* inputfile_name = NULL;
	int c;
	while( (c=getopt(argc, argv, "f:d")) != -1 ){
		switch(c){
		case 'f':
			inputfile_name = optarg;
			break;
		case 'd':
			flag_debug = 1;
			break;
		default:
			printf("Unknown Parameter %c\n", c);
			break;
		}
	}

	srand(time(NULL));

	initialize_cache(&L1_icache, L1_L, L1_K, L1_N, L1_NS, L1_NV);
	initialize_cache(&L1_dcache, L1_L, L1_K, L1_N, L1_NS, L1_NV);
	initialize_cache(&L2_cache, L2_L, L2_K, L2_N);
	L1_icache.lower_cache = &L2_cache;
	L1_dcache.lower_cache = &L2_cache;
	L2_cache.lower_cache = NULL;

	L1_icache.name = "L1_icache";
	L1_dcache.name = "L1_dcache";
	L2_cache.name = "L2_cache";

	do_simulation(inputfile_name);
	print_result();

	cleanup();

	return 0;
}
void cleanup(){
	free_cache(&L1_icache);
	free_cache(&L1_dcache);
	free_cache(&L2_cache);
}

void do_simulation(const char* input_file){
	FILE *input = fopen(input_file, "r");
	int type;
	uint64_t addr;
	if(!input){
		printf("Cannot open input file\n");
		return;
	}

	do{
		fscanf(input, "%d %lx\n", &type, &addr);
		if(flag_debug)
			printf("type: %d, addr: %lx\n", type, addr);
		statistics(type);
		if(type == 2)
			cache_access(&L1_icache, addr);
		else
			cache_access(&L1_dcache, addr);
	}while(!flag_debug && !feof(input));

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
	default:
		unknown_count++;
		break;
	}
}

void print_result(){
#ifdef DEBUG
	printf("\n----Operation Summary\n");
	printf("Total Count: %u\n\tInstruction read: %u\tData Read: %u\tData Write: %u\n",
			ir_count+dr_count+dw_count,ir_count, dr_count, dw_count);
#endif
	printf("\n-L1 data cache\n");
	print_cache(&L1_dcache);
	printf("\n-L1 instruction cache\n");
	print_cache(&L1_icache);
	printf("\n-L2 cache\n");
	print_cache(&L2_cache);
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "cache.h"

int main(int argc, char* argv[]){
	if(argc < 5){
input_error:
		printf("Usage : \n./cache INPUT_FILE L K N [streambuffer_entry_num] [victimcache_entry_num]\n");
		return 0;
	}

#ifndef LRU
	srand(time(NULL));
#endif

	byte_per_line = atoi(argv[2]);
	line_per_set = atoi(argv[3]);
	number_of_set = atoi(argv[4]);

#ifdef STREAMBUFFER
	if(argc > 5){
		number_of_streambuffer_entry = atoi(argv[5]);
		if(number_of_streambuffer_entry < 1)
			goto input_error;
	}else
		goto input_error;
#endif

#ifdef VICTIMCACHE
	if(argc > 6){
		number_of_victimcache_entry = atoi(argv[6]);
		if(number_of_victimcache_entry < 1)
			goto input_error;
	}else
		goto input_error;
#endif

#ifdef DEBUG
	printf("Initiating L(%u)K(%u)N(%u):%uKB cache..\n", byte_per_line, line_per_set, number_of_set, byte_per_line * line_per_set * number_of_set / 1024);
#ifdef STREAMBUFFER
	printf("Streambuffer size: %d\n", number_of_streambuffer_entry);
#endif
#ifdef VICTIMCACHE
	printf("Victim cache size; %d\n", number_of_victimcache_entry);
#endif
#endif

	initialize_cache(&L1_icache, 64, 1, 512);
	initialize_cache(&L1_dcache, 64, 1, 512);
	initialize_cache(&L2_cache, byte_per_line, line_per_set, number_of_set);
	L1_icache.lower_cache = &L2_cache;
	L1_dcache.lower_cache = &L2_cache;
	L2_cache.lower_cache = NULL;

	do_simulation(argv[1]);
	print_result();

	cleanup();

	return 0;
}
void cleanup(){
	free_cache(&L1_icache);
	free_cache(&L1_dcache);
	free_cache(&L2_cache);
}

//_L:byte per line
//_K:line per set
//_N:number of set
void initialize_cache(cache *target, unsigned int _L, unsigned int _K, unsigned int _N){

	unsigned int is_dcache = _K == 1 ? 1:0;
	
	target->L = _L;
	target->K = _K;
	target->N = _N;

	int i,j;
	int word_index_size = _L / WORD_SIZE;
	int nofset = _N;
	while(word_index_size = word_index_size >> 1)
		word_index_length++;
	while(nofset = nofset >> 1)
		set_index_length++;
	tag_length = ADDR_SIZE - set_index_length - word_index_length;
#ifdef DEBUG
	printf("tag: %u, set: %u, word: %u\n", tag_length, set_index_length, word_index_length);
#endif

	target->set = calloc(_N, sizeof(cset));
	for(i = 0;i < _N;i++){
		target->set[i].line = calloc(_K, sizeof(cline));
#ifdef LRU
		target->set[i].lru = calloc(_K, sizeof(int));
		memset(target->set[i].lru, -1, _K*sizeof(int));
#endif
	}

	for(i = 0;i < _N;i++){
		for(j = 0;j < _K;j++){
			target->set[i].line[j].data = malloc(_L);
			target->set[i].line[j].valid = 0;
		}
	}

#ifdef STREAMBUFFER
	if(is_dcache){
		streambuffer = calloc(number_of_streambuffer_entry, sizeof(cline));
		for(i = 0;i < number_of_streambuffer_entry;i++)
			streambuffer[i].data = malloc(_L);
		streambuffer_tag_length = ADDR_SIZE - word_index_length;
	}else{
		streambuffer = NULL;
	}
#endif
#ifdef VICTIMCACHE
	if(is_dcache){
		victimcache = calloc(number_of_victimcache_entry, sizeof(cline));
		for(i = 0;i < number_of_victimcache_entry;i++)
			victimcache[i].data = malloc(_L);
		victimcache_tag_length = ADDR_SIZE - word_index_length;
		victimcache_lru = calloc(number_of_victimcache_entry, sizeof(int));
		memset(victimcache_lru, -1, number_of_victimcache_entry*sizeof(int));
	}else{
		victimcache = NULL;
		victimcache_lru = NULL;
	}
#endif
}

void free_cache(cache *target){
	int i,j;

	for(i = 0;i < target->N;i++){
		for(j = 0;j < target->K;j++)
			free(target->set[i].line[j].data);
	}
	for(i = 0;i < target->N;i++){
		free(target->set[i].line);
#ifdef LRU
		free(target->set[i].lru);
#endif
	}
	free(target->set);
#ifdef STREAMBUFFER
	if(streambuffer){
		for(i = 0;i < number_of_streambuffer_entry;i++)
			free(streambuffer[i].data);
		free(streambuffer);
	}
#endif
#ifdef VICTIMCACHE
	if(victimcache){
		for(i = 0;i < number_of_victimcache_entry;i++)
			free(victimcache[i].data);
		free(victimcache);
		free(victimcache_lru);
	}
#endif
}

void do_simulation(const char* input_file){
	FILE *input = fopen(input_file, "r");
	int type;
	uint64_t addr;
	if(!input){
		printf("Cannot open input file\n");
		return;
	}

#ifndef DEBUG
	while(!feof(input)){
#endif
		fscanf(input, "%d %lx\n", &type, &addr);
#ifdef DEBUG
		printf("type: %d, addr: %lx\n", type, addr);
#endif
		statistics(type);
		if(type == 2)
			cache_access(&L1_icache, addr);
		else
			cache_access(&L1_dcache, addr);
#ifndef DEBUG
	}
#endif

	fclose(input);
}

void cache_access(cache *target, uint64_t addr){
	uint64_t tag = bitsplit(addr, ADDR_SIZE - tag_length, ADDR_SIZE-1);
	uint64_t word_index = bitsplit(addr, 0, word_index_length - 1);
	uint64_t set_index = 0;
	if(set_index_length)
		set_index = bitsplit(addr, ADDR_SIZE - tag_length - set_index_length, ADDR_SIZE - tag_length - 1);

	cache_access_impl(target, tag, word_index, set_index);
}

void cache_access_impl(cache *target, uint64_t tag, uint64_t word_index, uint64_t set_index){
#ifdef DEBUG
	printf("tag: %lx\tset index: %lx\tword index: %lx\n", tag, set_index, word_index);
#endif
	int i;
	int found = 0;
	for(i = 0;i < target->K;i++){
		cline *cur = &(target->set[set_index].line[i]);
		if(cur->valid){
			if(cur->tag == tag){
				cur->hit_count++;
#ifdef LRU
				update_lru(target->set[set_index].lru, i, target->K);
#endif
				found = 1;
				break;
			}
		}
	}
	if(!found){
#ifdef STREAMBUFFER
		if(do_streambuffer(tag, set_index, word_index))
			goto finish;
#endif
#ifdef VICTIMCACHE
		if(do_victimcache(tag, set_index, word_index)){
			goto finish;
		}
#endif
		target->set[set_index].miss_count++;
		fetch(target, tag, set_index, word_index);
	}
finish:
	return;
}
#ifdef STREAMBUFFER
int do_streambuffer(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
	if(streambuffer == NULL)
		return 0;

	uint64_t addr = bitmerge(tag, set_index, word_index);
	uint64_t _tag = bitsplit(addr, ADDR_SIZE-streambuffer_tag_length, ADDR_SIZE-1);
	if(streambuffer[0].tag == _tag){
		cline *cur = &target->set[set_index].line[0];
#ifdef VICTIMCACHE
		uint64_t cache_addr = bitmerge(cur->tag, set_index, word_index);
		put_victimcache(addr);
#endif
		cur->tag = tag;
		cur->valid = 1;
		fetch_streambuffer(addr, 0);
		cur->hit_count++;
		streambuffer_hit++;
		return 1;
	}else{
		streambuffer_miss++;
		return 0;
	}
}
void fetch_streambuffer(cache *target, uint64_t addr, int refresh_all){
	if(streambuffer == NULL)
		return;

	if(refresh_all){
		int i;
		uint64_t tag;
		for(i = 0;i < number_of_streambuffer_entry;i++){
			addr += target->L;
			tag = bitsplit(addr, ADDR_SIZE - streambuffer_tag_length, ADDR_SIZE-1);
			streambuffer[i].tag = tag;
		}
	}else{
		addr += target->L * number_of_streambuffer_entry;
		int i;
		for(i = 1;i < number_of_streambuffer_entry;i++)
			swap_cline(&streambuffer[i], &streambuffer[i-1]);

		uint64_t tag = bitsplit(addr, ADDR_SIZE - streambuffer_tag_length, ADDR_SIZE-1);
		streambuffer[number_of_streambuffer_entry-1].tag = tag;
	}
}
#endif
#ifdef VICTIMCACHE
int do_victimcache(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
	if(victimcache == NULL)
		return 0;

	uint64_t addr = bitmerge(tag, set_index, word_index);
	uint64_t v_tag = bitsplit(addr, ADDR_SIZE-victimcache_tag_length, ADDR_SIZE-1);
	int i;
	for(i = 0;i < number_of_victimcache_entry;i++){
		if(victimcache[i].tag == v_tag)
			break;
	}

	if(i == number_of_victimcache_entry){
		victimcache_miss++;
		return 0;
	}else{
		cline temp;
		cline *cur = &target->set[set_index].line[0];
		temp.tag = cur->tag;
		temp.data = cur->data;
		cur->tag = tag;
		cur->valid = 1;
		cur->data = victimcache[i].data;

		uint64_t tmp_addr = bitmerge(temp.tag, set_index, word_index);
		victimcache[i].tag = bitsplit(tmp_addr, ADDR_SIZE-victimcache_tag_length, ADDR_SIZE-1);
		victimcache[i].data = temp.data;		

		cur->hit_count++;
		victimcache_hit++;
		return 1;
	}
}
void put_victimcache(uint64_t addr){
	if(victimcache == NULL)
		return;

	int i, idx;
	for(i = 0;i < number_of_victimcache_entry;i++)
		if(victimcache[i].valid == 0)
			break;
	if(i == number_of_victimcache_entry){
		idx = victimcache_lru[0];
		if(idx == -1){
			printf("OPPS, on VICTIMCACHE, lru not full but there is no invalid entry!\n");
			cleanup();
			exit(0);
		}
	}else
		idx = i;
	uint64_t tag = bitsplit(addr, ADDR_SIZE - victimcache_tag_length, ADDR_SIZE-1);
	victimcache[idx].tag = tag;
	victimcache[idx].valid = 1;
	update_lru(victimcache_lru, idx, number_of_victimcache_entry);
}
#endif
void swap_cline(cline *from, cline *to){
	cline temp;
	temp.tag = to->tag;
	temp.data = to->data;
	to->tag = from->tag;
	to->data = from->data;
	from->tag = temp.tag;
	from->data = temp.data;
}

void update_lru(int *lru, int index, unsigned int length){
	int i;
	for(i = 0;i < length;i++){
		if(lru[i] == index)
			break;
	}
	if(i == length){
		for(i = 0;i < length;i++)
			if(lru[i] == -1)
				break;
		if(i == length){
			printf("OPPS, index error while updating lru\n");
			cleanup();
			exit(0);
		}
		lru[i] = index;
	}
	int temp = lru[i];
	//pick current used one and left shift lru
	for(;i < length -1;i++)
		lru[i] = lru[i+1];
	lru[length-1] = temp;
}
void fetch(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
	if(target->lower_cache)
		cache_access_impl(target->lower_cache, tag, set_index, word_index);

	int i;
	cset *cur = &(target->set[set_index]);
	if(target->K == 1){
		i = 0;
	}else{
		//default replacement is random
		for(i = 0;i < target->K;i++){
			if(!cur->line[i].valid)
				break;
		}
		if(i == target->K){
#ifdef LRU
			//select i by lru and update lru
			i = cur->lru[0];
			if(i < 0 || i >= target->K){
				printf("error on i %d\n", i);
				cleanup();
				exit(0);
			}
#else
			i = rand()%target->K;
#endif
		}
#ifdef LRU
		update_lru(cur->lru, i, target->K);
#endif
	}

#ifdef VICTIMCACHE
	uint64_t addr = bitmerge(cur->line[i].tag, set_index, word_index);
	put_victimcache(addr);
#endif
	cur->line[i].tag = tag;
	cur->line[i].valid = 1;
#ifdef STREAMBUFFER
	fetch_streambuffer(bitmerge(tag, set_index, word_index), 1);
#endif
}

uint64_t bitsplit(uint64_t value, int from, int to){
	uint64_t mask = 0, bit = 1 << from;
	int i;
	for(i = from;i <= to;i++){
		mask |= bit;
		bit = bit << 1;
	}
	return ( value & mask ) >> from;
}
uint64_t bitmerge(uint64_t tag, uint64_t set_index, uint64_t word_index){
	uint64_t addr = 0;
	addr |= tag << (set_index_length + word_index_length);
	addr |= set_index << (word_index_length);
	addr |= word_index;
	return addr;
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
	print_extra_component();
}

void print_cache(cache *target){
#ifdef DEBUG
	printf("\n----Cache Summary\n");
#endif
	int i,j;
	uint64_t hit_per_set, total_hit = 0, total_miss = 0;
	for(i = 0;i < target->N;i++){
		hit_per_set = 0;
#ifdef DETAIL_STATISTICS
		printf("\tSet %10u:\n", i);
#endif
		for(j = 0;j < target->K;j++){
			hit_per_set += target->set[i].line[j].hit_count;
#ifdef DETAIL_STATISTICS
			printf("\t\tline %10d: hit %10lu\n", j, target->set[i].line[j].hit_count);
#endif
		}
		total_hit += hit_per_set;
		total_miss += target->set[i].miss_count;
#if DETAIL_STATISTICS
		printf("\tmiss: %10lu\thit: %10lu\n", target->set[i].miss_count, hit_per_set);
#endif
	}
	printf("Total  miss: %10lu\thit: %10lu\n", total_miss, total_hit);
}

void print_extra_component(){
#ifdef STREAMBUFFER
	if(streambuffer)
		printf("Stream miss: %10u\thit: %10u\n", streambuffer_miss, streambuffer_hit);
#endif
#ifdef VICTIMCACHE
	if(victimcache)
		printf("Victim miss: %10u\thit: %10u\n", victimcache_miss, victimcache_hit);
#endif
}

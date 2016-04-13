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

	initialize_cache(byte_per_line, line_per_set, number_of_set);
	do_simulation(argv[1]);
	print_result();
	free_cache();

	return 0;
}

//_L:byte per line
//_K:line per set
//_N:number of set
void initialize_cache(unsigned int _L, unsigned int _K, unsigned int _N){
#ifdef STREAMBUFFER
	if(line_per_set != 1){
		printf("STREAMBUFFER cannot applied to N-way set associative cache\n");
		exit(0);
	}
#endif
#ifdef VICTIMCACHE
	if(line_per_set != 1){
		printf("VICTIMCACHE cannot applied to N-way set associative cache\n");
		exit(0);
	}
#endif
	int i,j;
	int word_index_size = _L / WORD_SIZE;
	int nofset = number_of_set;
	while(word_index_size = word_index_size >> 1)
		word_index_length++;
	while(nofset = nofset >> 1)
		set_index_length++;
	tag_length = ADDR_SIZE - set_index_length - word_index_length;
#ifdef DEBUG
	printf("tag: %u, set: %u, word: %u\n", tag_length, set_index_length, word_index_length);
#endif

	main_cache.set = calloc(_N, sizeof(cset));
	for(i = 0;i < _N;i++){
		main_cache.set[i].line = calloc(_K, sizeof(cline));
#ifdef LRU
		main_cache.set[i].lru = calloc(_K, sizeof(int));
		memset(main_cache.set[i].lru, -1, _K*sizeof(int));
#endif
	}

	for(i = 0;i < _N;i++){
		for(j = 0;j < _K;j++){
			main_cache.set[i].line[j].data = malloc(_L);
			main_cache.set[i].line[j].valid = 0;
		}
	}

#ifdef STREAMBUFFER
	streambuffer = calloc(number_of_streambuffer_entry, sizeof(cline));
	for(i = 0;i < number_of_streambuffer_entry;i++)
		streambuffer[i].data = malloc(byte_per_line);
	streambuffer_tag_length = ADDR_SIZE - word_index_length;
#endif
#ifdef VICTIMCACHE
	victimcache = calloc(number_of_victimcache_entry, sizeof(cline));
	for(i = 0;i < number_of_victimcache_entry;i++)
		victimcache[i].data = malloc(byte_per_line);
	victimcache_tag_length = ADDR_SIZE - word_index_length;
	victimcache_lru = calloc(number_of_victimcache_entry, sizeof(int));
	memset(victimcache_lru, -1, number_of_victimcache_entry*sizeof(int));
#endif
}

void free_cache(){
	int i,j;

	for(i = 0;i < number_of_set;i++){
		for(j = 0;j < line_per_set;j++)
			free(main_cache.set[i].line[j].data);
	}
	for(i = 0;i < number_of_set;i++){
		free(main_cache.set[i].line);
#ifdef LRU
		free(main_cache.set[i].lru);
#endif
	}
	free(main_cache.set);
#ifdef STREAMBUFFER
	for(i = 0;i < number_of_streambuffer_entry;i++)
		free(streambuffer[i].data);
	free(streambuffer);
#endif
#ifdef VICTIMCACHE
	for(i = 0;i < number_of_victimcache_entry;i++)
		free(victimcache[i].data);
	free(victimcache);
	free(victimcache_lru);
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
		cache_access(type, addr);
#ifndef DEBUG
	}
#endif

	fclose(input);
}

void cache_access(int types, uint64_t addr){
	uint64_t tag = bitsplit(addr, ADDR_SIZE - tag_length, ADDR_SIZE-1);
	uint64_t word_index = bitsplit(addr, 0, word_index_length - 1);
	uint64_t set_index = 0;
	if(set_index_length)
		set_index = bitsplit(addr, ADDR_SIZE - tag_length - set_index_length, ADDR_SIZE - tag_length - 1);
#ifdef DEBUG
	printf("tag: %lx\tset index: %lx\tword index: %lx\n", tag, set_index, word_index);
#endif
	int i;
	int found = 0;
	for(i = 0;i < line_per_set;i++){
		cline *cur = &(main_cache.set[set_index].line[i]);
		if(cur->valid){
			if(cur->tag == tag){
				cur->hit_count++;
#ifdef LRU
				update_lru(main_cache.set[set_index].lru, i, line_per_set);
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
		main_cache.set[set_index].miss_count++;
		fetch(tag, set_index, word_index);
	}
finish:
	return;
}
#ifdef STREAMBUFFER
int do_streambuffer(uint64_t tag, uint64_t set_index, uint64_t word_index){
	uint64_t addr = bitmerge(tag, set_index, word_index);
	uint64_t _tag = bitsplit(addr, ADDR_SIZE-streambuffer_tag_length, ADDR_SIZE-1);
	if(streambuffer[0].tag == _tag){
		cline *cur = &main_cache.set[set_index].line[0];
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
void fetch_streambuffer(uint64_t addr, int refresh_all){
	if(refresh_all){
		int i;
		uint64_t tag;
		for(i = 0;i < number_of_streambuffer_entry;i++){
			addr += byte_per_line;
			tag = bitsplit(addr, ADDR_SIZE - streambuffer_tag_length, ADDR_SIZE-1);
			streambuffer[i].tag = tag;
		}
	}else{
		addr += byte_per_line * number_of_streambuffer_entry;
		int i;
		for(i = 1;i < number_of_streambuffer_entry;i++)
			swap_cline(&streambuffer[i], &streambuffer[i-1]);

		uint64_t tag = bitsplit(addr, ADDR_SIZE - streambuffer_tag_length, ADDR_SIZE-1);
		streambuffer[number_of_streambuffer_entry-1].tag = tag;
	}
}
#endif
#ifdef VICTIMCACHE
int do_victimcache(uint64_t tag, uint64_t set_index, uint64_t word_index){
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
		cline *cur = &main_cache.set[set_index].line[0];
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
	int i, idx;
	for(i = 0;i < number_of_victimcache_entry;i++)
		if(victimcache[i].valid == 0)
			break;
	if(i == number_of_victimcache_entry){
		idx = victimcache_lru[0];
		if(idx == -1){
			printf("OPPS, on VICTIMCACHE, lru not full but there is no invalid entry!\n");
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
			free_cache();
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
void fetch(uint64_t tag, uint64_t set_index, uint64_t word_index){
	int i;
	cset *cur = &(main_cache.set[set_index]);
	if(line_per_set == 1){
		i = 0;
	}else{
		//default replacement is random
		for(i = 0;i < line_per_set;i++){
			if(!cur->line[i].valid)
				break;
		}
		if(i == line_per_set){
#ifdef LRU
			//select i by lru and update lru
			i = cur->lru[0];
			if(i < 0 || i >= line_per_set){
				printf("error on i %d\n", i);
				exit(0);
			}
#else
			i = rand()%line_per_set;
#endif
		}
#ifdef LRU
		update_lru(cur->lru, i, line_per_set);
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

	printf("\n----Cache Summary\n");
#endif
	int i,j;
	uint64_t hit_per_set, total_hit = 0, total_miss = 0;
	for(i = 0;i < number_of_set;i++){
		hit_per_set = 0;
#ifdef DETAIL_STATISTICS
		printf("\tSet %10u:\n", i);
#endif
		for(j = 0;j < line_per_set;j++){
			hit_per_set += main_cache.set[i].line[j].hit_count;
#ifdef DETAIL_STATISTICS
			printf("\t\tline %10d: hit %10lu\n", j, main_cache.set[i].line[j].hit_count);
#endif
		}
		total_hit += hit_per_set;
		total_miss += main_cache.set[i].miss_count;
#if DETAIL_STATISTICS
		printf("\tmiss: %10lu\thit: %10lu\n", main_cache.set[i].miss_count, hit_per_set);
#endif
	}
	printf("Total  miss: %10lu\thit: %10lu\n", total_miss, total_hit);
#ifdef STREAMBUFFER
	printf("Stream miss: %10u\thit: %10u\n", streambuffer_miss, streambuffer_hit);
#endif
#ifdef VICTIMCACHE
	printf("Victim miss: %10u\thit: %10u\n", victimcache_miss, victimcache_hit);
#endif
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <iostream>
#include <vector>

#include "cache.h"

//_L:byte per line
//_K:line per set
//_N:number of set
void initialize_cache(cache *target, unsigned int _L, unsigned int _K, unsigned int _N, size_t number_of_streambuffer_entry, size_t number_of_victimbuffer_entry){
	if(flag_debug){
		printf("Initiating L(%u)K(%u)N(%u):%uKB cache..\n", _L, _K, _N, _L*_K*_N / 1024);
		if(number_of_streambuffer_entry)
			printf("Streambuffer size: %zu\n", number_of_streambuffer_entry);
		if(number_of_victimbuffer_entry)
			printf("Victim cache size; %zu\n", number_of_victimbuffer_entry);
	}

	unsigned int is_dcache = _K == 1 ? 1:0;
	
	target->L = _L;
	target->K = _K;
	target->N = _N;
	target->word_index_length = 0;
	target->tag_length = 0;
	target->set_index_length = 0;

	int i,j;
	int word_index_size = _L / WORD_SIZE;
	int nofset = _N;
	while((word_index_size = word_index_size >> 1))
		target->word_index_length++;
	while((nofset = nofset >> 1))
		target->set_index_length++;
	target->tag_length = ADDR_SIZE - target->set_index_length - target->word_index_length;
	if(flag_debug)
		printf("tag: %u, set: %u, word: %u\n", target->tag_length, target->set_index_length, target->word_index_length);

	target->set = (cset*)calloc(_N, sizeof(cset));
	for(i = 0;i < _N;i++){
		target->set[i].line = (cline*)calloc(_K, sizeof(cline));
	}

	for(i = 0;i < _N;i++){
		for(j = 0;j < _K;j++){
			target->set[i].line[j].data = (uint64_t*)malloc(_L);
			target->set[i].line[j].valid = 0;
		}
	}

	if(is_dcache){
		target->streambuffer = (cline*)calloc(number_of_streambuffer_entry, sizeof(cline));
		target->streambuffer_size = number_of_streambuffer_entry;
		for(i = 0;i < number_of_streambuffer_entry;i++)
			target->streambuffer[i].data = (uint64_t*)malloc(_L);
		target->streambuffer_tag_length = ADDR_SIZE - target->word_index_length;
	}else{
		target->streambuffer = NULL;
	}
	if(is_dcache){
		target->victimbuffer = (cline*)calloc(number_of_victimbuffer_entry, sizeof(cline));
		target->victimbuffer_size = number_of_victimbuffer_entry;
		for(i = 0;i < number_of_victimbuffer_entry;i++)
			target->victimbuffer[i].data = (uint64_t*)malloc(_L);
		target->victimbuffer_tag_length = ADDR_SIZE - target->word_index_length;
		target->victimbuffer_lru = (int*)calloc(number_of_victimbuffer_entry, sizeof(int));
		memset(target->victimbuffer_lru, -1, number_of_victimbuffer_entry*sizeof(int));
	}else{
		target->victimbuffer = NULL;
		target->victimbuffer_lru = NULL;
	}
}

void free_cache(cache *target){
	int i,j;

	for(i = 0;i < target->N;i++){
		for(j = 0;j < target->K;j++)
			free(target->set[i].line[j].data);
	}
	for(i = 0;i < target->N;i++){
		free(target->set[i].line);
	}
	free(target->set);
	if(target->streambuffer){
		for(i = 0;i < target->streambuffer_size;i++)
			free(target->streambuffer[i].data);
		free(target->streambuffer);
	}
	if(target->victimbuffer){
		for(i = 0;i < target->victimbuffer_size;i++)
			free(target->victimbuffer[i].data);
		free(target->victimbuffer);
		free(target->victimbuffer_lru);
	}
}

void cache_access(cache *target, uint64_t addr){
	uint64_t tag = bitsplit(addr, ADDR_SIZE - target->tag_length, ADDR_SIZE-1);
	uint64_t word_index = bitsplit(addr, 0, target->word_index_length - 1);
	uint64_t set_index = 0;
	if(target->set_index_length)
		set_index = bitsplit(addr, ADDR_SIZE - target->tag_length - target->set_index_length, ADDR_SIZE - target->tag_length - 1);

	cache_access_impl(target, tag, set_index, word_index);
}

void cache_access_impl(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
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
				found = 1;
				break;
			}
		}
	}
	if(!found){
		if(do_streambuffer(target, tag, set_index, word_index))
			goto finish;
		if(do_victimbuffer(target, tag, set_index, word_index)){
			goto finish;
		}
		target->set[set_index].miss_count++;
		fetch(target, tag, set_index, word_index);
	}
finish:
	return;
}
int do_streambuffer(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
	if(target->streambuffer == NULL)
		return 0;

	uint64_t addr = bitmerge(target, tag, set_index, word_index);
	uint64_t _tag = bitsplit(addr, ADDR_SIZE-target->streambuffer_tag_length, ADDR_SIZE-1);
	if(target->streambuffer[0].tag == _tag){
		cline *cur = &target->set[set_index].line[0];
		uint64_t cache_addr = bitmerge(target, cur->tag, set_index, word_index);
		put_victimbuffer(target, addr);
		cur->tag = tag;
		cur->valid = 1;
		fetch_streambuffer(target, addr, 0);
		cur->hit_count++;
		target->streambuffer_hit++;
		return 1;
	}else{
		target->streambuffer_miss++;
		return 0;
	}
}
void fetch_streambuffer(cache *target, uint64_t addr, int refresh_all){
	if(target->streambuffer == NULL)
		return;

	if(refresh_all){
		int i;
		uint64_t tag;
		for(i = 0;i < target->streambuffer_size;i++){
			addr += target->L;
			tag = bitsplit(addr, ADDR_SIZE - target->streambuffer_tag_length, ADDR_SIZE-1);
			target->streambuffer[i].tag = tag;
		}
	}else{
		addr += target->L * target->streambuffer_size;
		int i;
		for(i = 1;i < target->streambuffer_size;i++)
			swap_cline(&target->streambuffer[i], &target->streambuffer[i-1]);

		uint64_t tag = bitsplit(addr, ADDR_SIZE - target->streambuffer_tag_length, ADDR_SIZE-1);
		target->streambuffer[target->streambuffer_size-1].tag = tag;
	}
}
int do_victimbuffer(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
	if(target->victimbuffer == NULL)
		return 0;

	uint64_t addr = bitmerge(target, tag, set_index, word_index);
	uint64_t v_tag = bitsplit(addr, ADDR_SIZE-target->victimbuffer_tag_length, ADDR_SIZE-1);
	int i;
	for(i = 0;i < target->victimbuffer_size;i++){
		if(target->victimbuffer[i].tag == v_tag)
			break;
	}

	if(i == target->victimbuffer_size){
		target->victimbuffer_miss++;
		return 0;
	}else{
		cline temp;
		cline *cur = &target->set[set_index].line[0];
		temp.tag = cur->tag;
		temp.data = cur->data;
		cur->tag = tag;
		cur->valid = 1;
		cur->data = target->victimbuffer[i].data;

		uint64_t tmp_addr = bitmerge(target, temp.tag, set_index, word_index);
		target->victimbuffer[i].tag = bitsplit(tmp_addr, ADDR_SIZE-target->victimbuffer_tag_length, ADDR_SIZE-1);
		target->victimbuffer[i].data = temp.data;		

		cur->hit_count++;
		target->victimbuffer_hit++;
		return 1;
	}
}
void put_victimbuffer(cache *target, uint64_t addr){
	if(target->victimbuffer == NULL)
		return;

	int i, idx;
	for(i = 0;i < target->victimbuffer_size;i++)
		if(target->victimbuffer[i].valid == 0)
			break;
	if(i == target->victimbuffer_size){
		idx = target->victimbuffer_lru[0];
		if(idx == -1){
			printf("OPPS, on victimbuffer, lru not full but there is no invalid entry!\n");
		}
	}else
		idx = i;
	uint64_t tag = bitsplit(addr, ADDR_SIZE - target->victimbuffer_tag_length, ADDR_SIZE-1);
	target->victimbuffer[idx].tag = tag;
	target->victimbuffer[idx].valid = 1;
	update_lru(target->victimbuffer_lru, idx, target->victimbuffer_size);
}
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
		cache_access((cache*)target->lower_cache, bitmerge(target, tag, set_index, word_index));

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
			i = rand()%target->K;
		}
	}

	uint64_t addr = bitmerge(target, cur->line[i].tag, set_index, word_index);
	put_victimbuffer(target, addr);
	cur->line[i].tag = tag;
	cur->line[i].valid = 1;
	fetch_streambuffer(target, bitmerge(target, tag, set_index, word_index), 1);
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
uint64_t bitmerge(cache *target, uint64_t tag, uint64_t set_index, uint64_t word_index){
	uint64_t addr = 0;
	addr |= tag << (target->set_index_length + target->word_index_length);
	addr |= set_index << (target->word_index_length);
	addr |= word_index;
	return addr;
}
void print_cache(cache *target){
	printf("\n----%s Summary\n", target->name);
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
	print_extra_component(target);
}

void print_extra_component(cache *target){
	if(target->streambuffer)
		printf("Stream miss: %10u\thit: %10u\n", target->streambuffer_miss, target->streambuffer_hit);
	if(target->victimbuffer)
		printf("Victim miss: %10u\thit: %10u\n", target->victimbuffer_miss, target->victimbuffer_hit);
}

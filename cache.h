#ifndef CACHE_H
#define CACHE_H

#include<inttypes.h>

#define ADDR_SIZE	64	//bit
#define WORD_SIZE	1	//byte, basic block size
#define INPUT_FILE	"astar"

unsigned int byte_per_line = 0;
unsigned int line_per_set = 0;
unsigned int number_of_set = 0;
unsigned int word_index_length = 0;
unsigned int tag_length = 0;
unsigned int set_index_length = 0;

//statistics
unsigned int ir_count = 0;
unsigned int dr_count = 0;
unsigned int dw_count = 0;
unsigned int unknown_count = 0;

typedef struct cache_line{
	uint64_t valid;
	uint64_t tag;
	uint64_t *data;
	uint64_t hit_count;
} cline;

typedef struct cache_set{
	cline *line;
	uint64_t miss_count;
#ifdef LRU
	int *lru;
#endif
} cset;

typedef struct cache_impl{
	cset *set;
} cache;

cache main_cache;

void initialize_cache(unsigned int _L, unsigned int _K, unsigned int _N);
void free_cache();

void do_simulation(const char*);
void statistics(int types);
void cache_access(int types, uint64_t addr);
void fetch(uint64_t, uint64_t, uint64_t);
uint64_t bitsplit(uint64_t value, int from, int to);
uint64_t bitmerge(uint64_t tag, uint64_t set_index, uint64_t word_index);
void print_result();
void swap_cline(cline *from, cline *to);
void update_lru(int *lru, int index, unsigned int length);

#ifdef STREAMBUFFER
cline *streambuffer;
unsigned int number_of_streambuffer_entry = 0;
unsigned int streambuffer_tag_length = 0;
unsigned int streambuffer_hit = 0;
unsigned int streambuffer_miss = 0;
int do_streambuffer(uint64_t, uint64_t, uint64_t);
void fetch_streambuffer(uint64_t, int);
#endif

#ifdef VICTIMCACHE
cline *victimcache;
int *victimcache_lru;
unsigned int number_of_victimcache_entry = 0;
unsigned int victimcache_tag_length = 0;
unsigned int victimcache_hit = 0;
unsigned int victimcache_miss = 0;
int do_victimcache(uint64_t, uint64_t, uint64_t);
void put_victimcache(uint64_t);
#endif

#endif

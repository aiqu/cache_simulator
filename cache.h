#include <inttypes.h>
#include <unistd.h>
#include <list>
#include <map>
#include <vector>

#ifndef CACHE_H
#define CACHE_H

#define ADDR_SIZE	64	//bit
#define WORD_SIZE	1	//byte, basic block size
#define INPUT_FILE	"astar"

typedef enum cache_type {
	L1 = 0,
	L2,
	DRAM,
	CTYPE_MAX,
} cachy_type;

typedef struct cache_line{
	uint64_t valid;
	uint64_t dirty;
	uint64_t tag;
	uint64_t *data;
	uint64_t hit_count;
	uint64_t rhit;
	uint64_t whit;
	uint64_t addr;
} cline;

typedef struct cache_set{
	cline *line;
	uint64_t miss_count;
	uint64_t rmiss;
	uint64_t wmiss;
} cset;

typedef struct cache_impl{
	cset *set;
	unsigned int L;
	unsigned int K;
	unsigned int N;
	unsigned int word_index_length;
	unsigned int tag_length;
	unsigned int set_index_length;
	void *lower_cache;

	cline *streambuffer;
	unsigned int streambuffer_tag_length;
	unsigned int streambuffer_hit;
	unsigned int streambuffer_miss;
	size_t streambuffer_size;

	cline *victimbuffer;
	int *victimbuffer_lru;
	unsigned int victimbuffer_tag_length;
	unsigned int victimbuffer_hit;
	unsigned int victimbuffer_miss;
	size_t victimbuffer_size;

	std::list<std::pair<uint64_t, uint64_t> > writebuffer;
	int writebuffer_size;
	uint64_t wbwhit;
	uint64_t wbrhit;

	const char* name;
	cache_type type;

	int access_latency;
	int write_time;
	int read_time;

	std::map<int64_t, std::vector<int> > stat;
} cache;

void initialize_cache(cache *target, cache_type ctype, unsigned int _L, unsigned int _K, unsigned int _N, size_t ns = 0, size_t nv = 0);
void free_cache(cache *target);

void cache_access(cache *target, uint64_t addr, int);
void cache_access_impl(cache *target, uint64_t, uint64_t, uint64_t, int);
void fetch(cache*, uint64_t, uint64_t, uint64_t, int);
void fetch_dram(cache*, uint64_t, uint64_t, uint64_t, int);
void evict(cache*, cline*, uint64_t);
void evict_dram(cache*, cline*, uint64_t);
uint64_t bitsplit(uint64_t value, int from, int to);
uint64_t bitmerge(cache*, uint64_t tag, uint64_t set_index, uint64_t word_index);
void print_cache(cache*);
void print_extra_component(cache*);
void swap_cline(cline *from, cline *to);
void update_lru(int *lru, int index, unsigned int length);

int do_streambuffer(cache *target, uint64_t, uint64_t, uint64_t, int);
void fetch_streambuffer(cache*, uint64_t, int);

int do_victimbuffer(cache*, uint64_t, uint64_t, uint64_t, int);
int put_victimbuffer(cache*, uint64_t, int);

int put_writebuffer(cache*, uint64_t);
int do_writebuffer(cache*, uint64_t);
#endif

#include <inttypes.h>
#include "mainmemory.h"

#ifndef MAIN_H
#define MAIN_H

const unsigned int L1_L = 256;
const unsigned int L1_K = 1;
const unsigned int L1_N = 128;
const unsigned int L1_NS = 1;
const unsigned int L1_NV = 4;

const unsigned int L2_L = 256;
const unsigned int L2_K = 2;
const unsigned int L2_N = 1024;

//statistics
void do_simulation(const char*);
void statistics(int types);
void print_result();
void cleanup();

extern int flag_debug;
extern int flag_fullassociative;
extern int flag_streambuffer;
extern int flag_writel2;
extern int flag_writedram;
extern int PAGE_SIZE;
extern int64_t execution_time;
extern mainmemory mm;

#endif

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
unsigned int ir_count = 0;
unsigned int dr_count = 0;
unsigned int dw_count = 0;
unsigned int unknown_count = 0;
void do_simulation(const char*);
void statistics(int types);
void print_result();
void cleanup();

#endif

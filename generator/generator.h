#ifndef RL_GENERATOR
#define RL_GENERATOR

// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
typedef struct _GEN {
	unsigned long state[6];
	unsigned long (*GenerateNumber)(struct _GEN*);
} Random;

Random* NewRandom(int);
Random* NewRandomWithState(unsigned long arr[6]);
void DestroyRandom(Random*);
void RandomSave(Random* gen, const char *base_path);
Random* RandomLoad(const char *base_path);

#endif

#ifndef RL_GENERATOR
#define RL_GENERATOR

// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
typedef struct _RL_GEN {
	unsigned long Q[4096];
	unsigned long c;
	long long inital_carry;
	unsigned long (*GenerateNumber)(struct _RL_GEN*);
} RL_Generator;

RL_Generator* RL_GeneratorNew();
void RL_FreeGenerator(RL_Generator*);

#endif

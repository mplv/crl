#ifndef RL_GENERATOR
#define RL_GENERATOR

#ifdef __L64__

// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
typedef struct _RL_GEN {
	unsigned long Q[4096];
	unsigned long c;
	unsigned long (*GenerateNumber)(struct _RL_GEN*);
} RL_Generator;

unsigned long CMWC4096(RL_Generator* gen);

#else

// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
typedef struct _RL_GEN {
        unsigned int Q[4096];
        unsigned int c;
        unsigned int (*GenerateNumber)(struct _RL_GEN*);
} RL_Generator;

unsigned int CMWC4096(RL_Generator* gen);

#endif

RL_Generator* RL_GeneratorNew();
void RL_FreeGenerator(RL_Generator*);
void RL_NumGenSave(RL_Generator* gen, const char *base_path);
RL_Generator* RL_NumGenLoad(const char *base_path);

#endif

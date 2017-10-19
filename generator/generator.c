#include "generator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug/debug.h"

/*
// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
unsigned long CMWC4096(Random* gen) {
	// do random number generator stuff
	unsigned long long t, a=18782LL;
	unsigned long i=4095;
	unsigned long x,r=0xfffffffe;
	i=(i+1)&4095;
	t=a*gen->Q[i]+gen->c;
	gen->c=(t>>32);
	x=t+gen->c;
	if(x<gen->c){x++;gen->c++;}
	return(gen->Q[i]=r-x);
}
*/

// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
unsigned long xorshift(Random *gen) {
	gen->state[0] = ((gen->state[1])^((gen->state[1])>>7));
	gen->state[1] = gen->state[2];
	gen->state[2] = gen->state[3];
	gen->state[3] = gen->state[4];
	gen->state[4] = gen->state[5];
	gen->state[5] = ((gen->state[5])^((gen->state[5])>>6))^((gen->state[0])^((gen->state[0])<<13));
	return ((gen->state[2]) + (gen->state[2]) + 1)*(gen->state[5]);
}

Random* NewRandom(int gen){
	int i = 0;
	Random* g = calloc(1, sizeof(Random));
	srand(gen);
	for (; i < 6; i++) {
		g->state[i] = rand();
	}
	g->GenerateNumber = &xorshift;
	return g;
}

Random* NewRandomWithState(unsigned long arr[6]) {
	int i = 0;
	Random* g = calloc(1, sizeof(Random));
	for ( ; i < 6; i++) {
		g->state[i] = arr[i];
	}
	g->GenerateNumber = &xorshift;
	return g;
}

void DestroyRandom(Random* gen){
	free(gen);
}

Random* RandomLoad(const char *base_path)
{
	Random *gen = calloc(1,sizeof(Random));
	const char *load_loc = "data/save/gen.txt";
	int basePathLen = 0;
	int loadLocLen = 0;
	int i = 0;
	basePathLen = strnlen(base_path,256);
	loadLocLen = strnlen(load_loc,256);
	char *load_path = calloc((basePathLen + loadLocLen + 1), sizeof(char));
	for (i = 0; i < basePathLen; i++) {
		load_path[i] = base_path[i];
	}
	for (i = 0; i < loadLocLen; i++) {
		load_path[i + basePathLen] = load_loc[i];
	}
	load_path[basePathLen+loadLocLen] = '\0';
	DebugMessageStart(LOG);
	printf("%s", load_path);
    DebugMessageEnd();
	FILE *f = fopen(load_path, "r");
	if (f) {
		for (i = 0; i < 6; i++) {
			fscanf(f, "%lu\n", &(gen->state[i]));
		}
		fclose(f);
	}
	free(load_path);
	gen->GenerateNumber = &xorshift;
	return gen;
}

void RandomSave(Random* gen, const char *base_path) {
	const char *save_loc = "data/save/gen.txt";
	int basePathLen = 0;
	int saveLocLen = 0;
	int i = 0;
	basePathLen = strnlen(base_path,256);
	saveLocLen = strnlen(save_loc,256);
	char *save_path = calloc((basePathLen + saveLocLen + 1), sizeof(char));
	for (i = 0; i < basePathLen; i++) {
		save_path[i] = base_path[i];
	}
	for (i = 0; i < saveLocLen; i++) {
		save_path[i + basePathLen] = save_loc[i];
	}
	save_path[basePathLen+saveLocLen] = '\0';
	DebugMessageStart(LOG);
	printf("%s", save_path);
    DebugMessageEnd();
	FILE *f = fopen(save_path, "w");
	if (f) {
		for (i = 0; i < 6; i++) {
			fprintf(f, "%lu\n", gen->state[i]);
		}
		fclose(f);
	}
	free(save_path);
}

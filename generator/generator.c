#include "generator.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug/debug.h"

#ifdef __L64__
// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
unsigned long CMWC4096(RL_Generator* gen) {
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
#else
// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
unsigned int CMWC4096(RL_Generator* gen) {
	// do random number generator stuff
	unsigned int t, a=18782LL;
	unsigned int i=4095;
	unsigned int x,r=0xfffffffe;
	i=(i+1)&4095;
	t=a*gen->Q[i]+gen->c;
	gen->c=(t>>16);
	x=t+gen->c;
	if(x<gen->c){x++;gen->c++;}
	return(gen->Q[i]=r-x);
}
#endif

RL_Generator* RL_GeneratorNew(int gen){
	int i = 0;
	RL_Generator* g = calloc(1, sizeof(RL_Generator));
	g->c = 362436;
	srand(gen);
	for (i = 0; i < 4096; i++) {
		g->Q[i] = rand();
	}
	g->GenerateNumber = &CMWC4096;
	return g;
}

void RL_FreeGenerator(RL_Generator* gen){
	free(gen);
}

RL_Generator* RL_NumGenLoad(const char *base_path)
{
	RL_Generator *gen = calloc(1,sizeof(RL_Generator));
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
	RL_DebugMessage(LOG, load_path);
	FILE *f = fopen(load_path, "r");
	if (f) {
		for (i = 0; i < 4096; i++) {
			#ifdef __L64__
			fscanf(f, "%lu\n", &(gen->Q[i]));
			#else
			fscanf(f, "%u\n", &(gen->Q[i]));
			#endif
		}
		#ifdef __L64__
		fscanf(f, "%lu\n", &(gen->c));
		#else
		fscanf(f, "%u\n", &(gen->c));
		#endif
		fclose(f);
	}
	free(load_path);
	gen->GenerateNumber = &CMWC4096;
	return gen;
}

void RL_NumGenSave(RL_Generator* gen, const char *base_path) {
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
	RL_DebugMessage(LOG, save_path);
	FILE *f = fopen(save_path, "w");
	if (f) {
		for (i = 0; i < 4096; i++) {
			#ifdef __L64__
			fprintf(f, "%lu\n", gen->Q[i]);
			#else
			fprintf(f, "%u\n", gen->Q[i]);
			#endif
		}
		#ifdef __L64__
		fprintf(f, "%lu\n", gen->c);
		#else
		fprintf(f, "%u\n", gen->c);
		#endif
		fclose(f);
	}
	free(save_path);
}

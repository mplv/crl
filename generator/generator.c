#include "generator.h"
#include <stdlib.h>

// CMWC PRNG from
// http://school.anhb.uwa.edu.au/personalpages/kwessen/shared/Marsaglia03.html
unsigned long CMWC4096(RL_Generator* gen){
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

RL_Generator* RL_GeneratorNew(int gen){
	int i = 0;
	RL_Generator* g = calloc(sizeof(RL_Generator),1);
	g->c = 362436;
	srand(gen);
	for (i = 0; i < 4096; i++) {
		g->Q[i] = rand();
	}
	g->GenerateNumber = CMWC4096;
	return g;
}

void RL_FreeGenerator(RL_Generator* gen){
	free(gen);
}

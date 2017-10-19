#include "ai.h"

void AIRun(ArrayList* creatures, Map *m, Random* gen) {
	int i = 0;
	int size = ListSize(creatures);
	for(i = 0 ; i < size; i++)
	{
		Creature *c = ListGet(creatures,i);
		switch (c->ai) {
			case 0: // INDIVIDUAL
			// call INDIVIDUAL ai routine here
			AI_IndividualMove(c, m, gen);
			break;

			case 1: // group
			break;

			case 2: // stealth
			break;

			case 3: // no move till close
			break;

			default:
			// do nothing
			break;
		}
	}
}

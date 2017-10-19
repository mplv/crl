#include "ai.h"

void AIRun(ArrayList* creatures) {
	int i = 0;
	int size = ListSize(creatures);
	for(i = 0 ; i < size; i++)
	{
		Creature *c = ListGet(creatures,i);
		switch (c->ai) {
			case 'i':
			// call INDIVIDUAL ai routine here
			AIIndividualMove(c);
			break;

			case 'g': // group
			break;

			case 's': // stealth
			break;

			case 'n': // no move till close
			break;

			default:
			// do nothing
			break;
		}
	}
}

#ifndef AI
#define AI

#include "entity/creature/creature.h"

typedef enum {
	INDIVIDUAL,	// 0
	GROUP,		// 1
	SNEAK,		// 2
	NO_MOVE_TILL_CLOSE,	// 3
	NUM_AI
}ai_t;

void AIRun(ArrayList* creatures, Map *m, Random* gen);

// below here is the functions for each type of ai movement
void AI_IndividualMove(Creature* c, Map* m, Random* gen);

#endif

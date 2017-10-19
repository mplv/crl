#ifndef AI
#define AI

#include "entity/creature/creature.h"

typedef enum {
	INDIVIDUAL,	// 'i'
	GROUP,		// 'g'
	SNEAK,		// 's'
	NO_MOVE_TILL_CLOSE,	// 'n'
	NUM_AI
}ai_t;

typedef void (*AIMove)(Creature* c, Map* m) ai_f;

#endif

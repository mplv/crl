#ifndef debug_h
#define debug_h

#include "arraylist/arraylist.h"

typedef enum {
	LOG,
	WARNING,
	FATAL,
	NUM_LEVELS
}DebugLevel_t;

void RL_DebugMessage(DebugLevel_t, const char *c);

#endif /* debug_h */

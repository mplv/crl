#ifndef debug_h
#define debug_h

#include "arraylist/arraylist.h"

typedef enum {
	LOG,
	WARNING,
	FATAL,
	NUM_LEVELS
}DebugLevel_t;

void DebugMessage(DebugLevel_t, const char *c);

void DebugMessageStart(DebugLevel_t);
void DebugMessageEnd();

#endif /* debug_h */

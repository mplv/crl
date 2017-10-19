#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "arraylist/arraylist.h"

// Prints a messages in a stylized format according to the logging level
void DebugMessage(DebugLevel_t t, const char *c)
{
	switch (t) {
		case NUM_LEVELS:
		case LOG:
		printf("%c[%dm",0x1B, 0);
		break;
		case WARNING:
		printf("%c[%d;1m",0x1B, 33);
		break;
		case FATAL:
		printf("%c[%d;1m",0x1B, 31);
		break;
	}
	printf("%s%c[%dm", c, 0x1B, 0);
}

// Starts printing following messages in a stylized format
// according to logging level
void DebugMessageStart(DebugLevel_t t)
{
	switch (t) {
		case NUM_LEVELS:
		case LOG:
		printf("%c[%dm",0x1B, 0);
		break;
		case WARNING:
		printf("%c[%d;1m",0x1B, 33);
		break;
		case FATAL:
		printf("%c[%d;1m",0x1B, 31);
		break;
	}
}

// DebugMessageEnd stops printing in stylized format.
// Also adds a newline for ease of logging
void DebugMessageEnd()
{
	printf("%c[%dm\n", 0x1B, 0);
}

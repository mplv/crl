#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug.h"
#include "arraylist/arraylist.h"

void RL_DebugMessage(DebugLevel_t t, const char *c)
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
	printf("%s%c[%dm\n", c, 0x1B, 0);
}

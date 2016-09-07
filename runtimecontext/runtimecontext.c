#include <stdlib.h>
#include "runtimecontext.h"

// Create the roguelike context
RL_RTContext *RL_CreateRTContext()
{
    RL_RTContext *rtc = malloc(sizeof(RL_RTContext));;
    return rtc;
}

// free it
void RL_FreeRTContext(RL_RTContext *rtc)
{
    free(rtc);
}

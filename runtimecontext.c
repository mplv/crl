#include <stdlib.h>
#include "runtimecontext.h"

RL_RTContext *RL_CreateRTContext()
{
    RL_RTContext *rtc = malloc(sizeof(RL_RTContext));;
    return rtc;
}

void RL_FreeRTContext(RL_RTContext *rtc)
{
    free(rtc);
}


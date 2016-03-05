#ifndef RL_RUNTIMECONTEXT
#define RL_RUNTIMECONTEXT

#include "config.h"
#include "creature.h"
#include "arrayList.h"
#include "player.h"

// This is the context that will be passed to most functions
typedef struct _RL_RTCONTEXT {
    // config from the config file
    RL_Config *conf;
    ArrayList *creatures;
    char **map;
    RL_Player* player;
} RL_RTContext;

RL_RTContext *RL_CreateRTContext();
void RL_FreeRTContext(RL_RTContext *rtc);
#endif

#ifndef RL_RUNTIMECONTEXT
#define RL_RUNTIMECONTEXT

#include "generator/generator.h"
#include "config/config.h"
#include "arraylist/arraylist.h"
#include "entity/player/player.h"
#include "textures/textures.h"
#include "map/map.h"

// This is the context that will be passed to most functions
// container for most of the roguelike state
typedef struct _RL_RTCONTEXT {
    // config from the config file
    RL_Config *conf;
    ArrayList *creatures;
	RL_MapGeneratorHolder* mapGenHolder;
	RL_Map *map;
	Textures *textures;
    RL_Player* player;
	RL_Generator* gen;
} RL_RTContext;

RL_RTContext *RL_CreateRTContext();
void RL_FreeRTContext(RL_RTContext *rtc);
void RL_Save(RL_RTContext *rtc);
void RL_LoadSave(RL_RTContext *rtc);
#endif

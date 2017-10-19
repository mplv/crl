#ifndef RUNTIMECONTEXT
#define RUNTIMECONTEXT

#include "generator/generator.h"
#include "config/config.h"
#include "arraylist/arraylist.h"
#include "entity/player/player.h"
#include "textures/textures.h"
#include "map/map.h"

// This is the context that will be passed to most functions
// container for most of the roguelike state
typedef struct _CONTEXT {
    // config from the config file
    Config *conf;
    ArrayList *creatures;
	MapGenerator* mapGenHolder;
	Map *map;
	Textures *textures;
    Player* player;
	Random* gen;
} Context;

Context *CreateRTContext();
void FreeRTContext(Context *rtc);
void Save(Context *rtc);
int LoadSave(Context *rtc);
#endif

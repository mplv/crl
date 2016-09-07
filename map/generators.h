#ifndef RL_MAP_GENERATORS
#define RL_MAP_GENERATORS

#include "map.h"

// all specific generators functions go below
RL_Map * RL_CreateForestMap(int w, int h, RL_Generator *gen);
RL_Map * RL_CreateCaveMap(int w, int h, RL_Generator *gen);

#endif

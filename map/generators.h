#ifndef MAP_GENERATORS
#define MAP_GENERATORS

#include "map.h"

// all specific generators functions go below
Map * CreateForestMap(int w, int h, Random *gen);
Map * CreateCaveMap(int w, int h, Random *gen);

#endif

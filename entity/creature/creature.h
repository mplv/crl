#ifndef CREATURE
#define CREATURE
#include <stdio.h>
#include "entity/entity.h"
#include "config/config.h"
#include "arraylist/arraylist.h"
#include "map/map.h"

typedef struct {
    // max name is 20 chars long
    char* name;
    // vers of game that can gen this enemy
    int pre;
    // stats
	Entity ent;
    // which table this enemy drops items from
    int droptable;
    // which climates this mob spawns in
    climates_t climate;

	// which ai to use
	// noted in ai.h
	int ai;

	int nameAllocd;
} Creature;

void FreeCreaturesList(ArrayList *l);
ArrayList* LoadAllCreatures(Config *c);
void PrintCreature(Creature *c);
void CreatureListSave(ArrayList* l, const char *base_path);
ArrayList* CreatureListLoad(const char *base_path);
#endif

#ifndef RL_CREATURE
#define RL_CREATURE
#include <stdio.h>
#include "entity/entity.h"
#include "config/config.h"
#include "arraylist/arraylist.h"

typedef struct {
    // max name is 20 chars long
    char* name;
    // vers of game that can gen this enemy
    int pre;
    // stats
	RL_Entity ent;
    // which table this enemy drops items from
    int droptable;
    // which climates this mob spawns in
    char climate;
} RL_Creature;

void RL_FreeCreaturesList(ArrayList *l);
ArrayList* RL_LoadAllCreatures(RL_Config *c);
void RL_PrintCreature(RL_Creature *c);
void RL_CreatureListSave(ArrayList* l, const char *base_path);
ArrayList* RL_CreatureListLoad(const char *base_path);
#endif
